/**
 * @file map.h
 * @brief map 键值对容器实现
 * 
 * 使用哈希表实现键值对存储
 */

#ifndef CXC_CONTAINER_MAP_H
#define CXC_CONTAINER_MAP_H

#include "../core/core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 哈希函数类型
 * ============================================================================ */

/**
 * @brief 哈希函数指针类型
 */
typedef cxc_uint_t (*cxc_hash_func_t)(cxc_cptr_t key, cxc_size_t key_size);

/**
 * @brief 比较函数指针类型
 */
typedef int (*cxc_compare_func_t)(cxc_cptr_t key1, cxc_cptr_t key2, cxc_size_t key_size);

/* ============================================================================
 * 哈希表节点结构
 * ============================================================================ */

/**
 * @brief 哈希表节点结构
 */
typedef struct cxc_map_node {
    cxc_ptr_t key;                  /**< 键 */
    cxc_ptr_t value;                /**< 值 */
    struct cxc_map_node* next;      /**< 下一个节点（链式哈希） */
} cxc_map_node_t;

/* ============================================================================
 * map 结构
 * ============================================================================ */

/**
 * @brief map 结构
 */
typedef struct cxc_map {
    cxc_map_node_t** buckets;       /**< 桶数组 */
    cxc_size_t bucket_count;         /**< 桶数量 */
    cxc_size_t size;                /**< 元素数量 */
    cxc_size_t key_size;            /**< 键大小 */
    cxc_size_t value_size;          /**< 值大小 */
    cxc_hash_func_t hash_func;      /**< 哈希函数 */
    cxc_compare_func_t compare_func; /**< 比较函数 */
    void (*key_deleter)(cxc_ptr_t); /**< 键删除器 */
    void (*value_deleter)(cxc_ptr_t); /**< 值删除器 */
} cxc_map_t;

/* ============================================================================
 * map 操作
 * ============================================================================ */

/**
 * @brief 创建 map
 * @param key_size 键大小
 * @param value_size 值大小
 * @param initial_buckets 初始桶数量
 * @param hash_func 哈希函数（可为 NULL，使用默认哈希）
 * @param compare_func 比较函数（可为 NULL，使用默认比较）
 * @param key_deleter 键删除器（可为 NULL）
 * @param value_deleter 值删除器（可为 NULL）
 * @return map 指针，失败返回 NULL
 */
cxc_map_t* cxc_map_create(
    cxc_size_t key_size,
    cxc_size_t value_size,
    cxc_size_t initial_buckets,
    cxc_hash_func_t hash_func,
    cxc_compare_func_t compare_func,
    void (*key_deleter)(cxc_ptr_t),
    void (*value_deleter)(cxc_ptr_t)
);

/**
 * @brief 销毁 map
 * @param map map 指针
 */
void cxc_map_destroy(cxc_map_t* map);

/**
 * @brief 清空 map
 * @param map map 指针
 */
void cxc_map_clear(cxc_map_t* map);

/**
 * @brief 获取 map 大小
 * @param map map 指针
 * @return 元素数量
 */
static inline cxc_size_t cxc_map_size(const cxc_map_t* map) {
    return map != NULL ? map->size : 0;
}

/**
 * @brief 检查 map 是否为空
 * @param map map 指针
 * @return 如果为空返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_map_empty(const cxc_map_t* map) {
    return cxc_map_size(map) == 0;
}

/**
 * @brief 插入或更新键值对
 * @param map map 指针
 * @param key 键指针
 * @param value 值指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_map_insert(cxc_map_t* map, cxc_cptr_t key, cxc_cptr_t value);

/**
 * @brief 查找值
 * @param map map 指针
 * @param key 键指针
 * @return 值指针，未找到返回 NULL
 */
cxc_ptr_t cxc_map_find(cxc_map_t* map, cxc_cptr_t key);

/**
 * @brief 移除键值对
 * @param map map 指针
 * @param key 键指针
 * @return 成功返回 true，未找到返回 false
 */
cxc_bool_t cxc_map_erase(cxc_map_t* map, cxc_cptr_t key);

/**
 * @brief 检查键是否存在
 * @param map map 指针
 * @param key 键指针
 * @return 如果存在返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_map_contains(cxc_map_t* map, cxc_cptr_t key) {
    return cxc_map_find(map, key) != NULL;
}

/* ============================================================================
 * 默认哈希和比较函数
 * ============================================================================ */

/**
 * @brief 默认哈希函数（FNV-1a）
 */
cxc_uint_t cxc_default_hash(cxc_cptr_t key, cxc_size_t key_size);

/**
 * @brief 默认比较函数（memcmp）
 */
int cxc_default_compare(cxc_cptr_t key1, cxc_cptr_t key2, cxc_size_t key_size);

#ifdef __cplusplus
}
#endif

#endif /* CXC_CONTAINER_MAP_H */
