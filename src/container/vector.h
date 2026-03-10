/**
 * @file vector.h
 * @brief vector 动态数组实现
 * 
 * 提供泛型动态数组，使用宏模板实现
 */

#ifndef CXC_CONTAINER_VECTOR_H
#define CXC_CONTAINER_VECTOR_H

#include "../core/core.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * vector 结构
 * ============================================================================ */

/**
 * @brief vector 结构
 */
typedef struct cxc_vector {
    cxc_ptr_t data;         /**< 数据指针 */
    cxc_size_t size;        /**< 当前元素数量 */
    cxc_size_t capacity;    /**< 容量 */
    cxc_size_t elem_size;   /**< 元素大小 */
} cxc_vector_t;

/* ============================================================================
 * vector 操作
 * ============================================================================ */

/**
 * @brief 创建 vector
 * @param elem_size 元素大小
 * @param initial_capacity 初始容量
 * @return vector 指针，失败返回 NULL
 */
cxc_vector_t* cxc_vector_create(cxc_size_t elem_size, cxc_size_t initial_capacity);

/**
 * @brief 销毁 vector
 * @param vec vector 指针
 */
void cxc_vector_destroy(cxc_vector_t* vec);

/**
 * @brief 清空 vector
 * @param vec vector 指针
 */
void cxc_vector_clear(cxc_vector_t* vec);

/**
 * @brief 获取 vector 大小
 * @param vec vector 指针
 * @return 元素数量
 */
static inline cxc_size_t cxc_vector_size(const cxc_vector_t* vec) {
    return vec != NULL ? vec->size : 0;
}

/**
 * @brief 获取 vector 容量
 * @param vec vector 指针
 * @return 容量
 */
static inline cxc_size_t cxc_vector_capacity(const cxc_vector_t* vec) {
    return vec != NULL ? vec->capacity : 0;
}

/**
 * @brief 检查 vector 是否为空
 * @param vec vector 指针
 * @return 如果为空返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_vector_empty(const cxc_vector_t* vec) {
    return cxc_vector_size(vec) == 0;
}

/**
 * @brief 预留容量
 * @param vec vector 指针
 * @param capacity 新容量
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_vector_reserve(cxc_vector_t* vec, cxc_size_t capacity);

/**
 * @brief 调整大小
 * @param vec vector 指针
 * @param new_size 新大小
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_vector_resize(cxc_vector_t* vec, cxc_size_t new_size);

/**
 * @brief 在末尾添加元素
 * @param vec vector 指针
 * @param elem 元素指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_vector_push_back(cxc_vector_t* vec, cxc_cptr_t elem);

/**
 * @brief 移除末尾元素
 * @param vec vector 指针
 */
void cxc_vector_pop_back(cxc_vector_t* vec);

/**
 * @brief 在指定位置插入元素
 * @param vec vector 指针
 * @param index 索引
 * @param elem 元素指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_vector_insert(cxc_vector_t* vec, cxc_size_t index, cxc_cptr_t elem);

/**
 * @brief 移除指定位置的元素
 * @param vec vector 指针
 * @param index 索引
 */
void cxc_vector_erase(cxc_vector_t* vec, cxc_size_t index);

/**
 * @brief 获取指定位置的元素
 * @param vec vector 指针
 * @param index 索引
 * @return 元素指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_vector_at(cxc_vector_t* vec, cxc_size_t index) {
    if (vec == NULL || index >= vec->size) {
        return NULL;
    }
    return (char*)vec->data + index * vec->elem_size;
}

/**
 * @brief 获取指定位置的元素（常量）
 * @param vec vector 指针
 * @param index 索引
 * @return 元素指针，失败返回 NULL
 */
static inline cxc_cptr_t cxc_vector_at_c(const cxc_vector_t* vec, cxc_size_t index) {
    if (vec == NULL || index >= vec->size) {
        return NULL;
    }
    return (const char*)vec->data + index * vec->elem_size;
}

/**
 * @brief 获取第一个元素
 * @param vec vector 指针
 * @return 元素指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_vector_front(cxc_vector_t* vec) {
    return cxc_vector_at(vec, 0);
}

/**
 * @brief 获取最后一个元素
 * @param vec vector 指针
 * @return 元素指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_vector_back(cxc_vector_t* vec) {
    if (vec == NULL || vec->size == 0) {
        return NULL;
    }
    return cxc_vector_at(vec, vec->size - 1);
}

/* ============================================================================
 * 泛型宏（类型安全）
 * ============================================================================ */

/**
 * @brief 定义类型安全的 vector
 * @param type 元素类型
 */
#define CXC_VECTOR_DEFINE(type) \
    typedef struct cxc_vector_##type { \
        cxc_vector_t base; \
    } cxc_vector_##type##_t; \
    \
    static inline cxc_vector_##type##_t* cxc_vector_##type##_create(cxc_size_t initial_capacity) { \
        cxc_vector_##type##_t* vec = (cxc_vector_##type##_t*)cxc_malloc(sizeof(cxc_vector_##type##_t)); \
        if (vec != NULL) { \
            vec->base = *cxc_vector_create(sizeof(type), initial_capacity); \
        } \
        return vec; \
    } \
    \
    static inline void cxc_vector_##type##_destroy(cxc_vector_##type##_t* vec) { \
        if (vec != NULL) { \
            cxc_vector_destroy(&vec->base); \
            cxc_free(vec); \
        } \
    } \
    \
    static inline cxc_bool_t cxc_vector_##type##_push_back(cxc_vector_##type##_t* vec, type elem) { \
        return cxc_vector_push_back(&vec->base, &elem); \
    } \
    \
    static inline type* cxc_vector_##type##_at(cxc_vector_##type##_t* vec, cxc_size_t index) { \
        return (type*)cxc_vector_at(&vec->base, index); \
    } \
    \
    static inline const type* cxc_vector_##type##_at_c(const cxc_vector_##type##_t* vec, cxc_size_t index) { \
        return (const type*)cxc_vector_at_c(&vec->base, index); \
    }

#ifdef __cplusplus
}
#endif

#endif /* CXC_CONTAINER_VECTOR_H */
