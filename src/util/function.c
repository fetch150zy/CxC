/**
 * @file function.c
 * @brief 函数对象和闭包实现
 */

#include "function.h"

#ifdef __cplusplus
extern "C" {
#endif

cxc_closure_t* cxc_closure_create(
    cxc_ptr_t data,
    void (*func)(cxc_ptr_t, cxc_ptr_t),
    void (*destroy)(cxc_ptr_t)
) {
    if (func == NULL) {
        return NULL;
    }
    
    cxc_closure_t* closure = (cxc_closure_t*)cxc_malloc(sizeof(cxc_closure_t));
    if (closure == NULL) {
        return NULL;
    }
    
    closure->data = data;
    closure->func = func;
    closure->destroy = destroy;
    
    return closure;
}

void cxc_closure_destroy(cxc_closure_t* closure) {
    if (closure == NULL) {
        return;
    }
    
    if (closure->destroy != NULL && closure->data != NULL) {
        closure->destroy(closure->data);
    }
    
    cxc_free(closure);
}

cxc_function_t* cxc_function_create(
    cxc_ptr_t data,
    cxc_ptr_t (*invoke)(cxc_ptr_t, cxc_ptr_t),
    void (*destroy)(cxc_ptr_t)
) {
    if (invoke == NULL) {
        return NULL;
    }
    
    cxc_function_t* func = (cxc_function_t*)cxc_malloc(sizeof(cxc_function_t));
    if (func == NULL) {
        return NULL;
    }
    
    func->data = data;
    func->invoke = invoke;
    func->destroy = destroy;
    
    return func;
}

void cxc_function_destroy(cxc_function_t* func) {
    if (func == NULL) {
        return;
    }
    
    if (func->destroy != NULL && func->data != NULL) {
        func->destroy(func->data);
    }
    
    cxc_free(func);
}

#ifdef __cplusplus
}
#endif
