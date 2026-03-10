/**
 * @file inheritance.c
 * @brief 继承机制实现
 */

#include "inheritance.h"
#include "../core/macros.h"

#ifdef __cplusplus
extern "C" {
#endif

cxc_bool_t cxc_is_instance_of(cxc_ptr_t obj, const char* type_name) {
    /* 基础实现，需要配合 RTTI 使用 */
    (void)obj;
    (void)type_name;
    return false;
}

#ifdef __cplusplus
}
#endif
