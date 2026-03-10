/**
 * @file raii.c
 * @brief RAII 基础实现
 */

#include "raii.h"

#ifdef __cplusplus
extern "C" {
#endif

void cxc_scope_guard_release(cxc_scope_guard_t* guard) {
    if (guard == NULL) {
        return;
    }
    
    if (guard->active && guard->cleanup_func != NULL) {
        guard->cleanup_func(guard->data);
        guard->active = false;
    }
}

#ifdef __cplusplus
}
#endif
