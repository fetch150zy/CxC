/**
 * @file unique_ptr.h
 * @brief unique_ptr 实现
 * 
 * 提供独占所有权指针、移动语义模拟和自定义删除器
 */

#ifndef CXC_SP_UNIQUE_PTR_H
#define CXC_SP_UNIQUE_PTR_H

#include "../core/core.h"
#include "raii.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 删除器类型
 * ============================================================================ */

/**
 * @brief 默认删除器函数指针类型
 */
typedef void (*cxc_deleter_func_t)(cxc_ptr_t ptr);

/**
 * @brief 默认删除器（free）
 */
static inline void cxc_default_deleter(cxc_ptr_t ptr) {
    cxc_free(ptr);
}

/* ============================================================================
 * unique_ptr 结构
 * ============================================================================ */

/**
 * @brief unique_ptr 结构
 */
typedef struct cxc_unique_ptr {
    cxc_ptr_t ptr;                  /**< 原始指针 */
    cxc_deleter_func_t deleter;     /**< 删除器函数 */
} cxc_unique_ptr_t;

/* ============================================================================
 * unique_ptr 操作
 * ============================================================================ */

/**
 * @brief 创建 unique_ptr
 * @param ptr 原始指针
 * @param deleter 删除器函数（可为 NULL，使用默认删除器）
 * @return unique_ptr 结构
 */
static inline cxc_unique_ptr_t cxc_unique_ptr_create(cxc_ptr_t ptr, cxc_deleter_func_t deleter) {
    cxc_unique_ptr_t up = {
        .ptr = ptr,
        .deleter = deleter != NULL ? deleter : cxc_default_deleter
    };
    return up;
}

/**
 * @brief 创建 unique_ptr（指定类型）
 * @param type 类型
 * @param deleter 删除器函数（可为 NULL）
 */
#define CXC_UNIQUE_PTR_CREATE(type, deleter) \
    cxc_unique_ptr_create(NULL, deleter)

/**
 * @brief 创建 unique_ptr（从已分配的内存）
 * @param type 类型
 * @param ptr 原始指针
 * @param deleter 删除器函数（可为 NULL）
 */
#define CXC_UNIQUE_PTR_MAKE(type, ptr, deleter) \
    cxc_unique_ptr_create((ptr), deleter)

/**
 * @brief 释放 unique_ptr
 * @param up unique_ptr 指针
 */
void cxc_unique_ptr_release(cxc_unique_ptr_t* up);

/**
 * @brief 重置 unique_ptr
 * @param up unique_ptr 指针
 * @param ptr 新指针（可为 NULL）
 */
void cxc_unique_ptr_reset(cxc_unique_ptr_t* up, cxc_ptr_t ptr);

/**
 * @brief 获取原始指针
 * @param up unique_ptr 指针
 * @return 原始指针
 */
static inline cxc_ptr_t cxc_unique_ptr_get(const cxc_unique_ptr_t* up) {
    return up != NULL ? up->ptr : NULL;
}

/**
 * @brief 获取原始指针（指定类型）
 * @param up unique_ptr 指针
 * @param type 类型
 */
#define CXC_UNIQUE_PTR_GET(up, type) \
    ((type*)cxc_unique_ptr_get(up))

/**
 * @brief 检查是否为空
 * @param up unique_ptr 指针
 * @return 如果为空返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_unique_ptr_is_null(const cxc_unique_ptr_t* up) {
    return up == NULL || up->ptr == NULL;
}

/**
 * @brief 解引用 unique_ptr
 * @param up unique_ptr 指针
 * @param type 类型
 */
#define CXC_UNIQUE_PTR_DEREF(up, type) \
    (*(CXC_UNIQUE_PTR_GET(up, type)))

/* ============================================================================
 * 移动语义模拟
 * ============================================================================ */

/**
 * @brief 移动 unique_ptr
 * @param src 源 unique_ptr 指针
 * @return 新的 unique_ptr
 */
cxc_unique_ptr_t cxc_unique_ptr_move(cxc_unique_ptr_t* src);

/**
 * @brief 移动 unique_ptr（宏）
 * @param src 源 unique_ptr 指针
 */
#define CXC_UNIQUE_PTR_MOVE(src) \
    cxc_unique_ptr_move(src)

/* ============================================================================
 * 便捷宏
 * ============================================================================ */

/**
 * @brief 创建 unique_ptr（自动分配）
 * @param type 类型
 * @param count 数量
 * @param deleter 删除器函数（可为 NULL）
 */
#define CXC_MAKE_UNIQUE(type, count, deleter) \
    cxc_unique_ptr_create(CXC_MALLOC(type, count), deleter)

/**
 * @brief 创建 unique_ptr（自动分配并清零）
 * @param type 类型
 * @param count 数量
 * @param deleter 删除器函数（可为 NULL）
 */
#define CXC_MAKE_UNIQUE_ZEROED(type, count, deleter) \
    cxc_unique_ptr_create(CXC_CALLOC(type, count), deleter)

#ifdef __cplusplus
}
#endif

#endif /* CXC_SP_UNIQUE_PTR_H */
