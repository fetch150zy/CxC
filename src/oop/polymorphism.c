/**
 * @file polymorphism.c
 * @brief 多态机制实现
 */

#include "polymorphism.h"
#include "../core/macros.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

const cxc_type_info_t* cxc_type_info(cxc_ptr_t obj) {
    if (obj == NULL) {
        return NULL;
    }
    
    cxc_object_t* obj_base = (cxc_object_t*)obj;
    if (obj_base->vtable == NULL) {
        return NULL;
    }
    
    /* 简化实现：从 vtable 获取类型信息 */
    /* 实际实现需要维护类型信息表 */
    return NULL;
}

const char* cxc_type_name(cxc_ptr_t obj) {
    if (obj == NULL) {
        return "NULL";
    }
    
    cxc_object_t* obj_base = (cxc_object_t*)obj;
    if (obj_base->vtable == NULL) {
        return "Unknown";
    }
    
    return obj_base->vtable->class_name;
}

cxc_bool_t cxc_is_type(cxc_ptr_t obj, const char* type_name) {
    if (obj == NULL || type_name == NULL) {
        return false;
    }
    
    const char* actual_type = cxc_type_name(obj);
    return strcmp(actual_type, type_name) == 0;
}

cxc_bool_t cxc_is_derived_from(cxc_ptr_t obj, const char* base_type_name) {
    if (obj == NULL || base_type_name == NULL) {
        return false;
    }
    
    const cxc_type_info_t* type_info = cxc_type_info(obj);
    if (type_info == NULL) {
        return false;
    }
    
    /* 遍历继承链 */
    const cxc_type_info_t* current = type_info;
    while (current != NULL) {
        if (strcmp(current->name, base_type_name) == 0) {
            return true;
        }
        current = current->base;
    }
    
    return false;
}

cxc_ptr_t cxc_dynamic_cast(cxc_ptr_t obj, const char* target_type_name) {
    if (obj == NULL || target_type_name == NULL) {
        return NULL;
    }
    
    if (cxc_is_type(obj, target_type_name) || cxc_is_derived_from(obj, target_type_name)) {
        return obj;
    }
    
    return NULL;
}

#ifdef __cplusplus
}
#endif
