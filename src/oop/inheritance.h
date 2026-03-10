/**
 * @file inheritance.h
 * @brief 继承机制
 * 
 * 提供单继承支持、基类成员访问和类型转换宏
 */

#ifndef CXC_OOP_INHERITANCE_H
#define CXC_OOP_INHERITANCE_H

#include "class.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 继承定义宏
 * ============================================================================ */

/**
 * @brief 定义继承类结构体
 * @param derived_name 派生类名
 * @param base_name 基类名
 * @param ... 派生类成员变量列表
 * 
 * @example
 * CXC_CLASS(Base, (int, value));
 * CXC_INHERIT(Derived, Base, (int, extra));
 */
#define CXC_INHERIT(derived_name, base_name, ...) \
    typedef struct derived_name { \
        base_name##_t base; \
        CXC_CLASS_MEMBERS(__VA_ARGS__) \
    } derived_name##_t; \
    typedef derived_name##_t* derived_name##_ptr_t; \
    typedef const derived_name##_t* derived_name##_cptr_t

/* ============================================================================
 * 基类成员访问
 * ============================================================================ */

/**
 * @brief 访问基类成员
 * @param obj 派生类对象指针
 * @param base_name 基类名
 * @param member 成员名
 */
#define CXC_BASE_MEMBER(obj, base_name, member) \
    ((obj)->base.member)

/**
 * @brief 访问基类（返回基类指针）
 * @param obj 派生类对象指针
 * @param base_name 基类名
 */
#define CXC_BASE(obj, base_name) \
    ((base_name##_ptr_t)&((obj)->base))

/**
 * @brief 访问基类（常量）
 * @param obj 派生类对象指针
 * @param base_name 基类名
 */
#define CXC_BASE_C(obj, base_name) \
    ((base_name##_cptr_t)&((obj)->base))

/* ============================================================================
 * 类型转换
 * ============================================================================ */

/**
 * @brief 向上转换（派生类到基类）
 * @param obj 派生类对象指针
 * @param base_name 基类名
 */
#define CXC_UPCAST(obj, base_name) \
    CXC_BASE(obj, base_name)

/**
 * @brief 向上转换（常量）
 * @param obj 派生类对象指针
 * @param base_name 基类名
 */
#define CXC_UPCAST_C(obj, base_name) \
    CXC_BASE_C(obj, base_name)

/**
 * @brief 向下转换（基类到派生类）
 * @param base_obj 基类对象指针
 * @param derived_name 派生类名
 * @param base_name 基类名
 */
#define CXC_DOWNCAST(base_obj, derived_name, base_name) \
    ((derived_name##_ptr_t)((char*)(base_obj) - CXC_OFFSETOF(derived_name##_t, base)))

/**
 * @brief 向下转换（常量）
 * @param base_obj 基类对象指针
 * @param derived_name 派生类名
 * @param base_name 基类名
 */
#define CXC_DOWNCAST_C(base_obj, derived_name, base_name) \
    ((derived_name##_cptr_t)((char*)(base_obj) - CXC_OFFSETOF(derived_name##_t, base)))

/**
 * @brief 静态转换（编译时检查）
 * @param obj 对象指针
 * @param target_type 目标类型
 * @note 使用 core/types.h 中的 CXC_STATIC_CAST
 */
#define CXC_STATIC_CAST_OBJ(obj, target_type) \
    ((target_type)(obj))

/* ============================================================================
 * 类型检查
 * ============================================================================ */

/**
 * @brief 检查对象是否为指定类型（运行时）
 * @param obj 对象指针
 * @param type_name 类型名
 * @return 如果是该类型返回 true，否则返回 false
 * 
 * @note 需要配合 RTTI 使用
 */
cxc_bool_t cxc_is_instance_of(cxc_ptr_t obj, const char* type_name);

#ifdef __cplusplus
}
#endif

#endif /* CXC_OOP_INHERITANCE_H */
