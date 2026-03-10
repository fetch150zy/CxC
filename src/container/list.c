/**
 * @file list.c
 * @brief list 实现
 */

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

cxc_list_node_t* cxc_list_node_create(cxc_ptr_t data) {
    cxc_list_node_t* node = (cxc_list_node_t*)cxc_malloc(sizeof(cxc_list_node_t));
    if (node == NULL) {
        return NULL;
    }
    
    node->prev = NULL;
    node->next = NULL;
    node->data = data;
    
    return node;
}

void cxc_list_node_destroy(cxc_list_node_t* node) {
    if (node != NULL) {
        cxc_free(node);
    }
}

cxc_list_t* cxc_list_create(cxc_size_t elem_size, void (*deleter)(cxc_ptr_t)) {
    if (elem_size == 0) {
        return NULL;
    }
    
    cxc_list_t* list = (cxc_list_t*)cxc_malloc(sizeof(cxc_list_t));
    if (list == NULL) {
        return NULL;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->elem_size = elem_size;
    list->deleter = deleter;
    
    return list;
}

void cxc_list_destroy(cxc_list_t* list) {
    if (list == NULL) {
        return;
    }
    
    cxc_list_clear(list);
    cxc_free(list);
}

void cxc_list_clear(cxc_list_t* list) {
    if (list == NULL) {
        return;
    }
    
    cxc_list_node_t* node = list->head;
    while (node != NULL) {
        cxc_list_node_t* next = node->next;
        
        if (list->deleter != NULL && node->data != NULL) {
            list->deleter(node->data);
        } else if (node->data != NULL) {
            cxc_free(node->data);
        }
        
        cxc_list_node_destroy(node);
        node = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

cxc_bool_t cxc_list_push_front(cxc_list_t* list, cxc_cptr_t elem) {
    if (list == NULL || elem == NULL) {
        return false;
    }
    
    cxc_ptr_t data = cxc_malloc(list->elem_size);
    if (data == NULL) {
        return false;
    }
    
    cxc_memcpy(data, elem, list->elem_size);
    
    cxc_list_node_t* node = cxc_list_node_create(data);
    if (node == NULL) {
        cxc_free(data);
        return false;
    }
    
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    
    list->size++;
    return true;
}

cxc_bool_t cxc_list_push_back(cxc_list_t* list, cxc_cptr_t elem) {
    if (list == NULL || elem == NULL) {
        return false;
    }
    
    cxc_ptr_t data = cxc_malloc(list->elem_size);
    if (data == NULL) {
        return false;
    }
    
    cxc_memcpy(data, elem, list->elem_size);
    
    cxc_list_node_t* node = cxc_list_node_create(data);
    if (node == NULL) {
        cxc_free(data);
        return false;
    }
    
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    
    list->size++;
    return true;
}

void cxc_list_pop_front(cxc_list_t* list) {
    if (list == NULL || list->head == NULL) {
        return;
    }
    
    cxc_list_node_t* node = list->head;
    list->head = node->next;
    
    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }
    
    if (list->deleter != NULL && node->data != NULL) {
        list->deleter(node->data);
    } else if (node->data != NULL) {
        cxc_free(node->data);
    }
    
    cxc_list_node_destroy(node);
    list->size--;
}

void cxc_list_pop_back(cxc_list_t* list) {
    if (list == NULL || list->tail == NULL) {
        return;
    }
    
    cxc_list_node_t* node = list->tail;
    list->tail = node->prev;
    
    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }
    
    if (list->deleter != NULL && node->data != NULL) {
        list->deleter(node->data);
    } else if (node->data != NULL) {
        cxc_free(node->data);
    }
    
    cxc_list_node_destroy(node);
    list->size--;
}

cxc_bool_t cxc_list_insert_after(cxc_list_t* list, cxc_list_node_t* node, cxc_cptr_t elem) {
    if (list == NULL || node == NULL || elem == NULL) {
        return false;
    }
    
    cxc_ptr_t data = cxc_malloc(list->elem_size);
    if (data == NULL) {
        return false;
    }
    
    cxc_memcpy(data, elem, list->elem_size);
    
    cxc_list_node_t* new_node = cxc_list_node_create(data);
    if (new_node == NULL) {
        cxc_free(data);
        return false;
    }
    
    new_node->prev = node;
    new_node->next = node->next;
    
    if (node->next != NULL) {
        node->next->prev = new_node;
    } else {
        list->tail = new_node;
    }
    
    node->next = new_node;
    list->size++;
    
    return true;
}

cxc_bool_t cxc_list_insert_before(cxc_list_t* list, cxc_list_node_t* node, cxc_cptr_t elem) {
    if (list == NULL || node == NULL || elem == NULL) {
        return false;
    }
    
    cxc_ptr_t data = cxc_malloc(list->elem_size);
    if (data == NULL) {
        return false;
    }
    
    cxc_memcpy(data, elem, list->elem_size);
    
    cxc_list_node_t* new_node = cxc_list_node_create(data);
    if (new_node == NULL) {
        cxc_free(data);
        return false;
    }
    
    new_node->prev = node->prev;
    new_node->next = node;
    
    if (node->prev != NULL) {
        node->prev->next = new_node;
    } else {
        list->head = new_node;
    }
    
    node->prev = new_node;
    list->size++;
    
    return true;
}

void cxc_list_erase(cxc_list_t* list, cxc_list_node_t* node) {
    if (list == NULL || node == NULL) {
        return;
    }
    
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    
    if (list->deleter != NULL && node->data != NULL) {
        list->deleter(node->data);
    } else if (node->data != NULL) {
        cxc_free(node->data);
    }
    
    cxc_list_node_destroy(node);
    list->size--;
}

#ifdef __cplusplus
}
#endif
