/**
 * @file vector.c
 * @brief vector 实现
 */

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CXC_VECTOR_DEFAULT_CAPACITY 8
#define CXC_VECTOR_GROWTH_FACTOR 2

cxc_vector_t* cxc_vector_create(cxc_size_t elem_size, cxc_size_t initial_capacity) {
    if (elem_size == 0) {
        return NULL;
    }
    
    cxc_vector_t* vec = (cxc_vector_t*)cxc_malloc(sizeof(cxc_vector_t));
    if (vec == NULL) {
        return NULL;
    }
    
    vec->size = 0;
    vec->elem_size = elem_size;
    vec->capacity = initial_capacity > 0 ? initial_capacity : CXC_VECTOR_DEFAULT_CAPACITY;
    vec->data = cxc_malloc(vec->capacity * elem_size);
    
    if (vec->data == NULL) {
        cxc_free(vec);
        return NULL;
    }
    
    return vec;
}

void cxc_vector_destroy(cxc_vector_t* vec) {
    if (vec == NULL) {
        return;
    }
    
    cxc_free(vec->data);
    cxc_free(vec);
}

void cxc_vector_clear(cxc_vector_t* vec) {
    if (vec != NULL) {
        vec->size = 0;
    }
}

cxc_bool_t cxc_vector_reserve(cxc_vector_t* vec, cxc_size_t capacity) {
    if (vec == NULL) {
        return false;
    }
    
    if (capacity <= vec->capacity) {
        return true;
    }
    
    cxc_ptr_t new_data = cxc_realloc(vec->data, capacity * vec->elem_size);
    if (new_data == NULL) {
        return false;
    }
    
    vec->data = new_data;
    vec->capacity = capacity;
    return true;
}

cxc_bool_t cxc_vector_resize(cxc_vector_t* vec, cxc_size_t new_size) {
    if (vec == NULL) {
        return false;
    }
    
    if (new_size > vec->capacity) {
        cxc_size_t new_capacity = vec->capacity * CXC_VECTOR_GROWTH_FACTOR;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (!cxc_vector_reserve(vec, new_capacity)) {
            return false;
        }
    }
    
    vec->size = new_size;
    return true;
}

cxc_bool_t cxc_vector_push_back(cxc_vector_t* vec, cxc_cptr_t elem) {
    if (vec == NULL || elem == NULL) {
        return false;
    }
    
    if (vec->size >= vec->capacity) {
        cxc_size_t new_capacity = vec->capacity * CXC_VECTOR_GROWTH_FACTOR;
        if (!cxc_vector_reserve(vec, new_capacity)) {
            return false;
        }
    }
    
    cxc_ptr_t dest = (char*)vec->data + vec->size * vec->elem_size;
    cxc_memcpy(dest, elem, vec->elem_size);
    vec->size++;
    
    return true;
}

void cxc_vector_pop_back(cxc_vector_t* vec) {
    if (vec != NULL && vec->size > 0) {
        vec->size--;
    }
}

cxc_bool_t cxc_vector_insert(cxc_vector_t* vec, cxc_size_t index, cxc_cptr_t elem) {
    if (vec == NULL || elem == NULL || index > vec->size) {
        return false;
    }
    
    if (vec->size >= vec->capacity) {
        cxc_size_t new_capacity = vec->capacity * CXC_VECTOR_GROWTH_FACTOR;
        if (!cxc_vector_reserve(vec, new_capacity)) {
            return false;
        }
    }
    
    /* 移动元素 */
    cxc_ptr_t dest = (char*)vec->data + (index + 1) * vec->elem_size;
    cxc_ptr_t src = (char*)vec->data + index * vec->elem_size;
    cxc_size_t move_size = (vec->size - index) * vec->elem_size;
    cxc_memmove(dest, src, move_size);
    
    /* 插入新元素 */
    cxc_memcpy(src, elem, vec->elem_size);
    vec->size++;
    
    return true;
}

void cxc_vector_erase(cxc_vector_t* vec, cxc_size_t index) {
    if (vec == NULL || index >= vec->size) {
        return;
    }
    
    /* 移动元素 */
    cxc_ptr_t dest = (char*)vec->data + index * vec->elem_size;
    cxc_ptr_t src = (char*)vec->data + (index + 1) * vec->elem_size;
    cxc_size_t move_size = (vec->size - index - 1) * vec->elem_size;
    cxc_memmove(dest, src, move_size);
    
    vec->size--;
}

#ifdef __cplusplus
}
#endif
