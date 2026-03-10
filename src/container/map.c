/**
 * @file map.c
 * @brief map 实现
 */

#include "map.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CXC_MAP_DEFAULT_BUCKETS 16
#define CXC_MAP_LOAD_FACTOR 0.75

cxc_uint_t cxc_default_hash(cxc_cptr_t key, cxc_size_t key_size) {
    /* FNV-1a 哈希算法 */
    const cxc_uint_t FNV_OFFSET_BASIS = 2166136261U;
    const cxc_uint_t FNV_PRIME = 16777619U;
    
    cxc_uint_t hash = FNV_OFFSET_BASIS;
    const unsigned char* bytes = (const unsigned char*)key;
    
    for (cxc_size_t i = 0; i < key_size; i++) {
        hash ^= bytes[i];
        hash *= FNV_PRIME;
    }
    
    return hash;
}

int cxc_default_compare(cxc_cptr_t key1, cxc_cptr_t key2, cxc_size_t key_size) {
    return memcmp(key1, key2, key_size);
}

static cxc_size_t cxc_map_hash(const cxc_map_t* map, cxc_cptr_t key) {
    cxc_uint_t hash = map->hash_func(key, map->key_size);
    return hash % map->bucket_count;
}

static cxc_map_node_t* cxc_map_find_node(const cxc_map_t* map, cxc_cptr_t key) {
    cxc_size_t bucket = cxc_map_hash(map, key);
    cxc_map_node_t* node = map->buckets[bucket];
    
    while (node != NULL) {
        if (map->compare_func(node->key, key, map->key_size) == 0) {
            return node;
        }
        node = node->next;
    }
    
    return NULL;
}

static cxc_bool_t cxc_map_rehash(cxc_map_t* map) {
    cxc_size_t new_bucket_count = map->bucket_count * 2;
    cxc_map_node_t** new_buckets = (cxc_map_node_t**)cxc_calloc(new_bucket_count * sizeof(cxc_map_node_t*));
    
    if (new_buckets == NULL) {
        return false;
    }
    
    /* 重新分配所有节点 */
    for (cxc_size_t i = 0; i < map->bucket_count; i++) {
        cxc_map_node_t* node = map->buckets[i];
        while (node != NULL) {
            cxc_map_node_t* next = node->next;
            cxc_size_t new_bucket = cxc_default_hash(node->key, map->key_size) % new_bucket_count;
            node->next = new_buckets[new_bucket];
            new_buckets[new_bucket] = node;
            node = next;
        }
    }
    
    cxc_free(map->buckets);
    map->buckets = new_buckets;
    map->bucket_count = new_bucket_count;
    
    return true;
}

cxc_map_t* cxc_map_create(
    cxc_size_t key_size,
    cxc_size_t value_size,
    cxc_size_t initial_buckets,
    cxc_hash_func_t hash_func,
    cxc_compare_func_t compare_func,
    void (*key_deleter)(cxc_ptr_t),
    void (*value_deleter)(cxc_ptr_t)
) {
    if (key_size == 0 || value_size == 0) {
        return NULL;
    }
    
    cxc_map_t* map = (cxc_map_t*)cxc_malloc(sizeof(cxc_map_t));
    if (map == NULL) {
        return NULL;
    }
    
    map->bucket_count = initial_buckets > 0 ? initial_buckets : CXC_MAP_DEFAULT_BUCKETS;
    map->buckets = (cxc_map_node_t**)cxc_calloc(map->bucket_count * sizeof(cxc_map_node_t*));
    
    if (map->buckets == NULL) {
        cxc_free(map);
        return NULL;
    }
    
    map->size = 0;
    map->key_size = key_size;
    map->value_size = value_size;
    map->hash_func = hash_func != NULL ? hash_func : cxc_default_hash;
    map->compare_func = compare_func != NULL ? compare_func : cxc_default_compare;
    map->key_deleter = key_deleter;
    map->value_deleter = value_deleter;
    
    return map;
}

void cxc_map_destroy(cxc_map_t* map) {
    if (map == NULL) {
        return;
    }
    
    cxc_map_clear(map);
    cxc_free(map->buckets);
    cxc_free(map);
}

void cxc_map_clear(cxc_map_t* map) {
    if (map == NULL) {
        return;
    }
    
    for (cxc_size_t i = 0; i < map->bucket_count; i++) {
        cxc_map_node_t* node = map->buckets[i];
        while (node != NULL) {
            cxc_map_node_t* next = node->next;
            
            if (map->key_deleter != NULL) {
                map->key_deleter(node->key);
            } else {
                cxc_free(node->key);
            }
            
            if (map->value_deleter != NULL) {
                map->value_deleter(node->value);
            } else {
                cxc_free(node->value);
            }
            
            cxc_free(node);
            node = next;
        }
        map->buckets[i] = NULL;
    }
    
    map->size = 0;
}

cxc_bool_t cxc_map_insert(cxc_map_t* map, cxc_cptr_t key, cxc_cptr_t value) {
    if (map == NULL || key == NULL || value == NULL) {
        return false;
    }
    
    /* 检查是否需要重新哈希 */
    if (map->size >= map->bucket_count * CXC_MAP_LOAD_FACTOR) {
        if (!cxc_map_rehash(map)) {
            return false;
        }
    }
    
    /* 查找是否已存在 */
    cxc_map_node_t* node = cxc_map_find_node(map, key);
    if (node != NULL) {
        /* 更新值 */
        if (map->value_deleter != NULL) {
            map->value_deleter(node->value);
        } else {
            cxc_free(node->value);
        }
        
        node->value = cxc_malloc(map->value_size);
        if (node->value == NULL) {
            return false;
        }
        cxc_memcpy(node->value, value, map->value_size);
        return true;
    }
    
    /* 创建新节点 */
    node = (cxc_map_node_t*)cxc_malloc(sizeof(cxc_map_node_t));
    if (node == NULL) {
        return false;
    }
    
    node->key = cxc_malloc(map->key_size);
    node->value = cxc_malloc(map->value_size);
    
    if (node->key == NULL || node->value == NULL) {
        cxc_free(node->key);
        cxc_free(node->value);
        cxc_free(node);
        return false;
    }
    
    cxc_memcpy(node->key, key, map->key_size);
    cxc_memcpy(node->value, value, map->value_size);
    
    cxc_size_t bucket = cxc_map_hash(map, key);
    node->next = map->buckets[bucket];
    map->buckets[bucket] = node;
    map->size++;
    
    return true;
}

cxc_ptr_t cxc_map_find(cxc_map_t* map, cxc_cptr_t key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }
    
    cxc_map_node_t* node = cxc_map_find_node(map, key);
    return node != NULL ? node->value : NULL;
}

cxc_bool_t cxc_map_erase(cxc_map_t* map, cxc_cptr_t key) {
    if (map == NULL || key == NULL) {
        return false;
    }
    
    cxc_size_t bucket = cxc_map_hash(map, key);
    cxc_map_node_t* node = map->buckets[bucket];
    cxc_map_node_t* prev = NULL;
    
    while (node != NULL) {
        if (map->compare_func(node->key, key, map->key_size) == 0) {
            if (prev != NULL) {
                prev->next = node->next;
            } else {
                map->buckets[bucket] = node->next;
            }
            
            if (map->key_deleter != NULL) {
                map->key_deleter(node->key);
            } else {
                cxc_free(node->key);
            }
            
            if (map->value_deleter != NULL) {
                map->value_deleter(node->value);
            } else {
                cxc_free(node->value);
            }
            
            cxc_free(node);
            map->size--;
            return true;
        }
        
        prev = node;
        node = node->next;
    }
    
    return false;
}

#ifdef __cplusplus
}
#endif
