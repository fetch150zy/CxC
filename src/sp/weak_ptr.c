/**
 * @file weak_ptr.c
 * @brief weak_ptr 实现
 */

#include "weak_ptr.h"
#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

cxc_weak_ptr_t cxc_weak_ptr_create(const cxc_shared_ptr_t* sp) {
    cxc_weak_ptr_t wp = { .control_block = NULL };
    
    if (sp != NULL && sp->control_block != NULL) {
        cxc_control_block_add_weak_ref(sp->control_block);
        wp.control_block = sp->control_block;
    }
    
    return wp;
}

cxc_weak_ptr_t cxc_weak_ptr_copy(const cxc_weak_ptr_t* other) {
    cxc_weak_ptr_t wp = { .control_block = NULL };
    
    if (other != NULL && other->control_block != NULL) {
        cxc_control_block_add_weak_ref(other->control_block);
        wp.control_block = other->control_block;
    }
    
    return wp;
}

void cxc_weak_ptr_release(cxc_weak_ptr_t* wp) {
    if (wp == NULL || wp->control_block == NULL) {
        return;
    }
    
    cxc_control_block_release_weak_ref(wp->control_block);
    wp->control_block = NULL;
}

void cxc_weak_ptr_reset(cxc_weak_ptr_t* wp) {
    cxc_weak_ptr_release(wp);
}

cxc_shared_ptr_t cxc_weak_ptr_lock(const cxc_weak_ptr_t* wp) {
    cxc_shared_ptr_t sp = { .control_block = NULL };
    
    if (wp == NULL || wp->control_block == NULL) {
        return sp;
    }
    
    /* 检查对象是否还存在 */
    if (wp->control_block->ptr == NULL) {
        return sp;
    }
    
    /* 增加引用计数 */
    cxc_control_block_add_ref(wp->control_block);
    sp.control_block = wp->control_block;
    
    return sp;
}

cxc_bool_t cxc_weak_ptr_expired(const cxc_weak_ptr_t* wp) {
    if (wp == NULL || wp->control_block == NULL) {
        return true;
    }
    
    return wp->control_block->ptr == NULL;
}

cxc_uint_t cxc_weak_ptr_use_count(const cxc_weak_ptr_t* wp) {
    if (wp == NULL || wp->control_block == NULL) {
        return 0;
    }
    
    return atomic_load(&wp->control_block->ref_count);
}

#ifdef __cplusplus
}
#endif
