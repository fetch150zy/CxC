/**
 * @file set.c
 * @brief set 实现
 */

#include "set.h"

#ifdef __cplusplus
extern "C" {
#endif

cxc_set_t* cxc_set_create(
    cxc_size_t elem_size,
    cxc_size_t initial_buckets,
    cxc_hash_func_t hash_func,
    cxc_compare_func_t compare_func,
    void (*deleter)(cxc_ptr_t)
) {
    if (elem_size == 0) {
        return NULL;
    }
    
    cxc_set_t* set = (cxc_set_t*)cxc_malloc(sizeof(cxc_set_t));
    if (set == NULL) {
        return NULL;
    }
    
    /* 使用 map，值大小为 1（占位） */
    set->map = cxc_map_create(
        elem_size,
        1,  /* 值大小 */
        initial_buckets,
        hash_func,
        compare_func,
        deleter,
        NULL  /* 值删除器为空 */
    );
    
    if (set->map == NULL) {
        cxc_free(set);
        return NULL;
    }
    
    return set;
}

void cxc_set_destroy(cxc_set_t* set) {
    if (set == NULL) {
        return;
    }
    
    if (set->map != NULL) {
        cxc_map_destroy(set->map);
    }
    
    cxc_free(set);
}

void cxc_set_clear(cxc_set_t* set) {
    if (set != NULL && set->map != NULL) {
        cxc_map_clear(set->map);
    }
}

cxc_bool_t cxc_set_insert(cxc_set_t* set, cxc_cptr_t elem) {
    if (set == NULL || set->map == NULL || elem == NULL) {
        return false;
    }
    
    /* 使用元素本身作为值（占位） */
    char dummy = 0;
    return cxc_map_insert(set->map, elem, &dummy);
}

cxc_bool_t cxc_set_erase(cxc_set_t* set, cxc_cptr_t elem) {
    if (set == NULL || set->map == NULL || elem == NULL) {
        return false;
    }
    
    return cxc_map_erase(set->map, elem);
}

#ifdef __cplusplus
}
#endif
