/**
 * @file raii.h
 * @brief RAII 基础
 * 
 * 提供自动清理宏和作用域守卫
 */

#ifndef CXC_SP_RAII_H
#define CXC_SP_RAII_H

#include "../core/core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 作用域守卫
 * ============================================================================ */

/**
 * @brief 清理函数指针类型
 */
typedef void (*cxc_cleanup_func_t)(cxc_ptr_t data);

/**
 * @brief 作用域守卫结构
 */
typedef struct cxc_scope_guard {
    cxc_ptr_t data;                    /**< 数据指针 */
    cxc_cleanup_func_t cleanup_func;   /**< 清理函数 */
    cxc_bool_t active;                 /**< 是否激活 */
} cxc_scope_guard_t;

/**
 * @brief 创建作用域守卫
 * @param data 数据指针
 * @param cleanup_func 清理函数
 * @return 作用域守卫结构
 */
static inline cxc_scope_guard_t cxc_scope_guard_create(cxc_ptr_t data, cxc_cleanup_func_t cleanup_func) {
    cxc_scope_guard_t guard = {
        .data = data,
        .cleanup_func = cleanup_func,
        .active = true
    };
    return guard;
}

/**
 * @brief 释放作用域守卫
 * @param guard 作用域守卫指针
 */
void cxc_scope_guard_release(cxc_scope_guard_t* guard);

/**
 * @brief 取消作用域守卫
 * @param guard 作用域守卫指针
 */
static inline void cxc_scope_guard_dismiss(cxc_scope_guard_t* guard) {
    if (guard != NULL) {
        guard->active = false;
    }
}

/* ============================================================================
 * DEFER 宏（类似 Go 的 defer）
 * ============================================================================ */

/**
 * @brief DEFER 宏实现（使用 GCC cleanup 属性）
 */
#if defined(__GNUC__) || defined(__clang__)
    /**
     * @brief 内部清理函数
     */
    static inline void cxc_defer_cleanup(cxc_scope_guard_t* guard) {
        if (guard != NULL && guard->active && guard->cleanup_func != NULL) {
            guard->cleanup_func(guard->data);
        }
    }
    
    /**
     * @brief DEFER 宏
     * @param cleanup_expr 清理表达式（如函数调用）
     * 
     * @example
     * FILE* f = fopen("file.txt", "r");
     * CXC_DEFER(fclose(f));
     */
    #define CXC_DEFER(cleanup_expr) \
        __attribute__((cleanup(cxc_defer_cleanup))) \
        cxc_scope_guard_t CXC_UNIQUE_ID(_defer_) = \
            cxc_scope_guard_create(NULL, (cxc_cleanup_func_t)(cleanup_expr))
    
    /**
     * @brief DEFER 宏（带数据）
     * @param data 数据指针
     * @param cleanup_func 清理函数
     * 
     * @example
     * void* ptr = malloc(100);
     * CXC_DEFER_DATA(ptr, free);
     */
    #define CXC_DEFER_DATA(data, cleanup_func) \
        __attribute__((cleanup(cxc_defer_cleanup))) \
        cxc_scope_guard_t CXC_UNIQUE_ID(_defer_) = \
            cxc_scope_guard_create((data), (cleanup_func))
#else
    /* 不支持 cleanup 属性的编译器，使用手动管理 */
    #define CXC_DEFER(cleanup_expr) \
        /* 需要手动调用清理函数 */
    
    #define CXC_DEFER_DATA(data, cleanup_func) \
        /* 需要手动调用清理函数 */
#endif

/* ============================================================================
 * 作用域块宏
 * ============================================================================ */

/**
 * @brief 作用域块开始
 */
#define CXC_SCOPE_BEGIN() \
    do {

/**
 * @brief 作用域块结束
 */
#define CXC_SCOPE_END() \
    } while (0)

/**
 * @brief 作用域块（自动清理）
 * @param cleanup_expr 清理表达式
 */
#define CXC_SCOPE(cleanup_expr) \
    CXC_SCOPE_BEGIN() \
    CXC_DEFER(cleanup_expr)

#ifdef __cplusplus
}
#endif

#endif /* CXC_SP_RAII_H */
