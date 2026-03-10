/**
 * @file function.h
 * @brief 函数对象和闭包模拟
 * 
 * 提供闭包模拟和回调封装
 */

#ifndef CXC_UTIL_FUNCTION_H
#define CXC_UTIL_FUNCTION_H

#include "../core/core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 函数指针类型
 * ============================================================================ */

/**
 * @brief 无参数无返回值函数指针类型
 */
typedef void (*cxc_function_void_t)(void);

/**
 * @brief 单参数无返回值函数指针类型
 */
typedef void (*cxc_function_unary_t)(cxc_ptr_t arg);

/**
 * @brief 双参数无返回值函数指针类型
 */
typedef void (*cxc_function_binary_t)(cxc_ptr_t arg1, cxc_ptr_t arg2);

/**
 * @brief 单参数返回值的函数指针类型
 */
typedef cxc_ptr_t (*cxc_function_map_t)(cxc_ptr_t arg);

/* ============================================================================
 * 闭包结构
 * ============================================================================ */

/**
 * @brief 闭包结构
 */
typedef struct cxc_closure {
    cxc_ptr_t data;                     /**< 捕获的数据 */
    void (*func)(cxc_ptr_t, cxc_ptr_t); /**< 函数指针 */
    void (*destroy)(cxc_ptr_t);         /**< 数据销毁函数 */
} cxc_closure_t;

/**
 * @brief 创建闭包
 * @param data 捕获的数据
 * @param func 函数指针
 * @param destroy 数据销毁函数（可为 NULL）
 * @return 闭包指针，失败返回 NULL
 */
cxc_closure_t* cxc_closure_create(
    cxc_ptr_t data,
    void (*func)(cxc_ptr_t, cxc_ptr_t),
    void (*destroy)(cxc_ptr_t)
);

/**
 * @brief 销毁闭包
 * @param closure 闭包指针
 */
void cxc_closure_destroy(cxc_closure_t* closure);

/**
 * @brief 调用闭包
 * @param closure 闭包指针
 * @param arg 参数
 */
static inline void cxc_closure_call(cxc_closure_t* closure, cxc_ptr_t arg) {
    if (closure != NULL && closure->func != NULL) {
        closure->func(closure->data, arg);
    }
}

/* ============================================================================
 * 回调封装
 * ============================================================================ */

/**
 * @brief 回调结构
 */
typedef struct cxc_callback {
    cxc_ptr_t context;                  /**< 上下文数据 */
    void (*callback)(cxc_ptr_t, cxc_ptr_t); /**< 回调函数 */
} cxc_callback_t;

/**
 * @brief 创建回调
 * @param context 上下文数据
 * @param callback 回调函数
 * @return 回调结构
 */
static inline cxc_callback_t cxc_callback_create(
    cxc_ptr_t context,
    void (*callback)(cxc_ptr_t, cxc_ptr_t)
) {
    cxc_callback_t cb = {
        .context = context,
        .callback = callback
    };
    return cb;
}

/**
 * @brief 调用回调
 * @param cb 回调指针
 * @param arg 参数
 */
static inline void cxc_callback_invoke(cxc_callback_t* cb, cxc_ptr_t arg) {
    if (cb != NULL && cb->callback != NULL) {
        cb->callback(cb->context, arg);
    }
}

/* ============================================================================
 * 函数对象（泛型）
 * ============================================================================ */

/**
 * @brief 函数对象结构
 */
typedef struct cxc_function {
    cxc_ptr_t data;                     /**< 数据 */
    cxc_ptr_t (*invoke)(cxc_ptr_t, cxc_ptr_t); /**< 调用函数 */
    void (*destroy)(cxc_ptr_t);         /**< 销毁函数 */
} cxc_function_t;

/**
 * @brief 创建函数对象
 * @param data 数据
 * @param invoke 调用函数
 * @param destroy 销毁函数（可为 NULL）
 * @return 函数对象指针，失败返回 NULL
 */
cxc_function_t* cxc_function_create(
    cxc_ptr_t data,
    cxc_ptr_t (*invoke)(cxc_ptr_t, cxc_ptr_t),
    void (*destroy)(cxc_ptr_t)
);

/**
 * @brief 销毁函数对象
 * @param func 函数对象指针
 */
void cxc_function_destroy(cxc_function_t* func);

/**
 * @brief 调用函数对象
 * @param func 函数对象指针
 * @param arg 参数
 * @return 返回值
 */
static inline cxc_ptr_t cxc_function_invoke(cxc_function_t* func, cxc_ptr_t arg) {
    if (func != NULL && func->invoke != NULL) {
        return func->invoke(func->data, arg);
    }
    return NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* CXC_UTIL_FUNCTION_H */
