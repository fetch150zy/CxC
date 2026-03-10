/**
 * @file encapsulation.h
 * @brief 封装支持
 * 
 * 提供公有/私有成员分离和访问控制宏
 */

#ifndef CXC_OOP_ENCAPSULATION_H
#define CXC_OOP_ENCAPSULATION_H

#include "class.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 访问控制
 * ============================================================================ */

/**
 * @brief 定义公有成员
 * @param class_name 类名
 * @param ... 成员变量列表
 */
#define CXC_PUBLIC(class_name, ...) \
    CXC_CLASS_MEMBERS(__VA_ARGS__)

/**
 * @brief 定义私有成员
 * @param class_name 类名
 * @param ... 成员变量列表
 * 
 * @note 在 C 语言中，私有成员通过命名约定实现（如 _private_member）
 */
#define CXC_PRIVATE(class_name, ...) \
    CXC_CLASS_MEMBERS(__VA_ARGS__)

/**
 * @brief 定义受保护成员
 * @param class_name 类名
 * @param ... 成员变量列表
 */
#define CXC_PROTECTED(class_name, ...) \
    CXC_CLASS_MEMBERS(__VA_ARGS__)

/* ============================================================================
 * 访问器宏（Getter/Setter）
 * ============================================================================ */

/**
 * @brief 定义 Getter 函数
 * @param class_name 类名
 * @param member_type 成员类型
 * @param member_name 成员名
 */
#define CXC_GETTER(class_name, member_type, member_name) \
    static inline member_type class_name##_get_##member_name(class_name##_cptr_t self) { \
        return self->member_name; \
    }

/**
 * @brief 定义 Setter 函数
 * @param class_name 类名
 * @param member_type 成员类型
 * @param member_name 成员名
 */
#define CXC_SETTER(class_name, member_type, member_name) \
    static inline void class_name##_set_##member_name(class_name##_ptr_t self, member_type value) { \
        self->member_name = value; \
    }

/**
 * @brief 定义 Getter 和 Setter
 * @param class_name 类名
 * @param member_type 成员类型
 * @param member_name 成员名
 */
#define CXC_ACCESSORS(class_name, member_type, member_name) \
    CXC_GETTER(class_name, member_type, member_name) \
    CXC_SETTER(class_name, member_type, member_name)

/* ============================================================================
 * 友元函数（通过前向声明实现）
 * ============================================================================ */

/**
 * @brief 声明友元函数
 * @param func_name 函数名
 */
#define CXC_FRIEND(func_name) \
    /* 在 C 语言中，友元通过函数声明实现 */

#ifdef __cplusplus
}
#endif

#endif /* CXC_OOP_ENCAPSULATION_H */
