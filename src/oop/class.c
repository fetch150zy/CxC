/**
 * @file class.c
 * @brief 类基础实现
 */

#include "class.h"

#ifdef __cplusplus
extern "C" {
#endif

cxc_ptr_t cxc_new_impl(cxc_size_t size, cxc_ctor_func_t ctor) {
    if (size == 0) {
        return NULL;
    }
    
    cxc_ptr_t obj = cxc_calloc(size);
    if (obj == NULL) {
        return NULL;
    }
    
    if (ctor != NULL) {
        ctor(obj);
    }
    
    return obj;
}

void cxc_delete_impl(cxc_ptr_t obj, cxc_dtor_func_t dtor) {
    if (obj == NULL) {
        return;
    }
    
    if (dtor != NULL) {
        dtor(obj);
    }
    
    cxc_free(obj);
}

#ifdef __cplusplus
}
#endif
