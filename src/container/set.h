/**
 * @file set.h
 * @brief set 集合容器实现
 * 
 * 基于 map 实现的集合容器
 */

#ifndef CXC_CONTAINER_SET_H
#define CXC_CONTAINER_SET_H

#include "../core/core.h"
#include "map.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * set 结构（基于 map，值为空）
 * ============================================================================ */

/**
 * @brief set 结构
 */
typedef struct cxc_set {
    cxc_map_t* map;                 /**< 内部 map */
} cxc_set_t;

/* ============================================================================
 * set 操作
 * ============================================================================ */

/**
 * @brief 创建 set
 * @param elem_size 元素大小
 * @param initial_buckets 初始桶数量
 * @param hash_func 哈希函数（可为 NULL）
 * @param compare_func 比较函数（可为 NULL）
 * @param deleter 删除器函数（可为 NULL）
 * @return set 指针，失败返回 NULL
 */
cxc_set_t* cxc_set_create(
    cxc_size_t elem_size,
    cxc_size_t initial_buckets,
    cxc_hash_func_t hash_func,
    cxc_compare_func_t compare_func,
    void (*deleter)(cxc_ptr_t)
);

/**
 * @brief 销毁 set
 * @param set set 指针
 */
void cxc_set_destroy(cxc_set_t* set);

/**
 * @brief 清空 set
 * @param set set 指针
 */
void cxc_set_clear(cxc_set_t* set);

/**
 * @brief 获取 set 大小
 * @param set set 指针
 * @return 元素数量
 */
static inline cxc_size_t cxc_set_size(const cxc_set_t* set) {
    return set != NULL && set->map != NULL ? cxc_map_size(set->map) : 0;
}

/**
 * @brief 检查 set 是否为空
 * @param set set 指针
 * @return 如果为空返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_set_empty(const cxc_set_t* set) {
    return cxc_set_size(set) == 0;
}

/**
 * @brief 插入元素
 * @param set set 指针
 * @param elem 元素指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_set_insert(cxc_set_t* set, cxc_cptr_t elem);

/**
 * @brief 移除元素
 * @param set set 指针
 * @param elem 元素指针
 * @return 成功返回 true，未找到返回 false
 */
cxc_bool_t cxc_set_erase(cxc_set_t* set, cxc_cptr_t elem);

/**
 * @brief 检查元素是否存在
 * @param set set 指针
 * @param elem 元素指针
 * @return 如果存在返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_set_contains(cxc_set_t* set, cxc_cptr_t elem) {
    return set != NULL && set->map != NULL && cxc_map_contains(set->map, elem);
}

#ifdef __cplusplus
}
#endif

#endif /* CXC_CONTAINER_SET_H */
