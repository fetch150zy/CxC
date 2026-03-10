/**
 * @file polymorphism.h
 * @brief 多态机制
 * 
 * 提供虚函数表实现、虚函数声明和覆盖、运行时类型信息
 */

#ifndef CXC_OOP_POLYMORPHISM_H
#define CXC_OOP_POLYMORPHISM_H

#include "inheritance.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 虚函数表 (vtable)
 * ============================================================================ */

/**
 * @brief 虚函数指针类型
 */
typedef cxc_ptr_t (*cxc_vfunc_t)(cxc_ptr_t self, ...);

/**
 * @brief 虚函数表结构
 */
typedef struct cxc_vtable {
    const char* class_name;     /**< 类名 */
    cxc_size_t vfunc_count;     /**< 虚函数数量 */
    cxc_vfunc_t* vfuncs;        /**< 虚函数指针数组 */
} cxc_vtable_t;

/**
 * @brief 对象结构（带虚函数表）
 */
typedef struct cxc_object {
    const cxc_vtable_t* vtable; /**< 虚函数表指针 */
} cxc_object_t;

/**
 * @brief 定义虚函数表
 * @param class_name 类名
 * @param vfunc_count 虚函数数量
 */
#define CXC_VTABLE_DEFINE(class_name, vfunc_count) \
    static cxc_vfunc_t class_name##_vfuncs[vfunc_count]; \
    static const cxc_vtable_t class_name##_vtable = { \
        .class_name = #class_name, \
        .vfunc_count = vfunc_count, \
        .vfuncs = class_name##_vfuncs \
    }

/**
 * @brief 初始化虚函数表
 * @param class_name 类名
 * @param index 虚函数索引
 * @param func 函数指针
 */
#define CXC_VTABLE_INIT(class_name, index, func) \
    (class_name##_vfuncs[index] = (cxc_vfunc_t)(func))

/* ============================================================================
 * 虚函数调用
 * ============================================================================ */

/**
 * @brief 调用虚函数
 * @param obj 对象指针（必须包含 cxc_object_t 作为第一个成员）
 * @param index 虚函数索引
 * @param ... 函数参数
 */
#define CXC_VCALL(obj, index, ...) \
    (((cxc_object_t*)(obj))->vtable->vfuncs[index])(obj, ##__VA_ARGS__)

/**
 * @brief 声明虚函数
 * @param class_name 类名
 * @param func_name 函数名
 * @param return_type 返回类型
 * @param ... 参数列表
 */
#define CXC_VFUNC_DECLARE(class_name, func_name, return_type, ...) \
    return_type func_name(class_name##_ptr_t self, ##__VA_ARGS__)

/**
 * @brief 定义虚函数
 * @param class_name 类名
 * @param func_name 函数名
 * @param return_type 返回类型
 * @param ... 参数列表
 */
#define CXC_VFUNC_DEFINE(class_name, func_name, return_type, ...) \
    return_type func_name(class_name##_ptr_t self, ##__VA_ARGS__)

/**
 * @brief 覆盖虚函数
 * @param derived_name 派生类名
 * @param base_name 基类名
 * @param vtable_index 虚函数表索引
 * @param func 函数指针
 */
#define CXC_VFUNC_OVERRIDE(derived_name, base_name, vtable_index, func) \
    CXC_VTABLE_INIT(derived_name, vtable_index, func)

/* ============================================================================
 * 运行时类型信息 (RTTI)
 * ============================================================================ */

/**
 * @brief 类型信息结构
 */
typedef struct cxc_type_info {
    const char* name;           /**< 类型名 */
    cxc_size_t size;            /**< 类型大小 */
    const cxc_vtable_t* vtable; /**< 虚函数表 */
    struct cxc_type_info* base; /**< 基类类型信息 */
} cxc_type_info_t;

/**
 * @brief 获取对象的类型信息
 * @param obj 对象指针
 * @return 类型信息指针
 */
const cxc_type_info_t* cxc_type_info(cxc_ptr_t obj);

/**
 * @brief 获取类型名
 * @param obj 对象指针
 * @return 类型名字符串
 */
const char* cxc_type_name(cxc_ptr_t obj);

/**
 * @brief 检查对象是否为指定类型
 * @param obj 对象指针
 * @param type_name 类型名
 * @return 如果是该类型返回 true，否则返回 false
 */
cxc_bool_t cxc_is_type(cxc_ptr_t obj, const char* type_name);

/**
 * @brief 检查对象是否为指定类型的派生类
 * @param obj 对象指针
 * @param base_type_name 基类类型名
 * @return 如果是派生类返回 true，否则返回 false
 */
cxc_bool_t cxc_is_derived_from(cxc_ptr_t obj, const char* base_type_name);

/* ============================================================================
 * 动态类型转换
 * ============================================================================ */

/**
 * @brief 动态转换（运行时检查）
 * @param obj 对象指针
 * @param target_type_name 目标类型名
 * @return 转换后的指针，失败返回 NULL
 */
cxc_ptr_t cxc_dynamic_cast(cxc_ptr_t obj, const char* target_type_name);

#ifdef __cplusplus
}
#endif

#endif /* CXC_OOP_POLYMORPHISM_H */
