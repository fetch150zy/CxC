/**
 * @file unique_ptr.c
 * @brief unique_ptr 实现
 */

#include "unique_ptr.h"

#ifdef __cplusplus
extern "C" {
#endif

void cxc_unique_ptr_release(cxc_unique_ptr_t* up) {
    if (up == NULL) {
        return;
    }
    
    if (up->ptr != NULL && up->deleter != NULL) {
        up->deleter(up->ptr);
    }
    
    up->ptr = NULL;
    up->deleter = NULL;
}

void cxc_unique_ptr_reset(cxc_unique_ptr_t* up, cxc_ptr_t ptr) {
    if (up == NULL) {
        return;
    }
    
    /* 释放旧指针 */
    if (up->ptr != NULL && up->deleter != NULL) {
        up->deleter(up->ptr);
    }
    
    /* 设置新指针 */
    up->ptr = ptr;
}

cxc_unique_ptr_t cxc_unique_ptr_move(cxc_unique_ptr_t* src) {
    cxc_unique_ptr_t moved = { .ptr = NULL, .deleter = NULL };
    
    if (src != NULL) {
        moved = *src;
        src->ptr = NULL;
        src->deleter = NULL;
    }
    
    return moved;
}

#ifdef __cplusplus
}
#endif
