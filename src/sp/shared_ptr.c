/**
 * @file shared_ptr.c
 * @brief shared_ptr 实现
 */

#include "shared_ptr.h"
#include "../core/memory.h"
#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

static void cxc_default_deleter_impl(cxc_ptr_t ptr) {
    cxc_free(ptr);
}

cxc_control_block_t* cxc_control_block_create(cxc_ptr_t ptr, void (*deleter)(cxc_ptr_t)) {
    cxc_control_block_t* cb = (cxc_control_block_t*)cxc_malloc(sizeof(cxc_control_block_t));
    if (cb == NULL) {
        return NULL;
    }
    
    atomic_init(&cb->ref_count, 1);
    atomic_init(&cb->weak_count, 0);
    cb->ptr = ptr;
    cb->deleter = deleter != NULL ? deleter : cxc_default_deleter_impl;
    
    return cb;
}

void cxc_control_block_add_ref(cxc_control_block_t* cb) {
    if (cb != NULL) {
        atomic_fetch_add(&cb->ref_count, 1);
    }
}

cxc_bool_t cxc_control_block_release_ref(cxc_control_block_t* cb) {
    if (cb == NULL) {
        return false;
    }
    
    unsigned int old_count = atomic_fetch_sub(&cb->ref_count, 1);
    if (old_count == 1) {
        /* 引用计数为 0，释放资源 */
        if (cb->ptr != NULL && cb->deleter != NULL) {
            cb->deleter(cb->ptr);
        }
        cb->ptr = NULL;
        
        /* 如果没有弱引用，释放控制块 */
        if (atomic_load(&cb->weak_count) == 0) {
            cxc_free(cb);
            return true;
        }
    }
    
    return false;
}

void cxc_control_block_add_weak_ref(cxc_control_block_t* cb) {
    if (cb != NULL) {
        atomic_fetch_add(&cb->weak_count, 1);
    }
}

void cxc_control_block_release_weak_ref(cxc_control_block_t* cb) {
    if (cb == NULL) {
        return;
    }
    
    unsigned int old_weak_count = atomic_fetch_sub(&cb->weak_count, 1);
    if (old_weak_count == 1) {
        /* 弱引用计数为 0，如果引用计数也为 0，释放控制块 */
        if (atomic_load(&cb->ref_count) == 0) {
            cxc_free(cb);
        }
    }
}

cxc_shared_ptr_t cxc_shared_ptr_create(cxc_ptr_t ptr, void (*deleter)(cxc_ptr_t)) {
    cxc_shared_ptr_t sp = { .control_block = NULL };
    
    if (ptr != NULL) {
        sp.control_block = cxc_control_block_create(ptr, deleter);
        if (sp.control_block == NULL) {
            /* 创建失败，释放原始指针 */
            if (deleter != NULL) {
                deleter(ptr);
            } else {
                cxc_default_deleter_impl(ptr);
            }
        }
    }
    
    return sp;
}

cxc_shared_ptr_t cxc_shared_ptr_copy(const cxc_shared_ptr_t* other) {
    cxc_shared_ptr_t sp = { .control_block = NULL };
    
    if (other != NULL && other->control_block != NULL) {
        cxc_control_block_add_ref(other->control_block);
        sp.control_block = other->control_block;
    }
    
    return sp;
}

void cxc_shared_ptr_release(cxc_shared_ptr_t* sp) {
    if (sp == NULL || sp->control_block == NULL) {
        return;
    }
    
    cxc_control_block_release_ref(sp->control_block);
    sp->control_block = NULL;
}

void cxc_shared_ptr_reset(cxc_shared_ptr_t* sp, cxc_ptr_t ptr, void (*deleter)(cxc_ptr_t)) {
    if (sp == NULL) {
        return;
    }
    
    /* 释放旧的 shared_ptr */
    cxc_shared_ptr_release(sp);
    
    /* 创建新的 shared_ptr */
    if (ptr != NULL) {
        *sp = cxc_shared_ptr_create(ptr, deleter);
    }
}

cxc_uint_t cxc_shared_ptr_use_count(const cxc_shared_ptr_t* sp) {
    if (sp == NULL || sp->control_block == NULL) {
        return 0;
    }
    
    return atomic_load(&sp->control_block->ref_count);
}

#ifdef __cplusplus
}
#endif
