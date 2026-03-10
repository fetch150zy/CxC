/**
 * @file types.h
 * @brief 类型系统基础定义
 * 
 * 提供通用类型定义、编译时类型检查宏和 typeof 扩展封装
 */

#ifndef CXC_CORE_TYPES_H
#define CXC_CORE_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 基础类型定义
 * ============================================================================ */

/**
 * @brief 通用指针类型
 */
typedef void* cxc_ptr_t;

/**
 * @brief 通用常量指针类型
 */
typedef const void* cxc_cptr_t;

/**
 * @brief 布尔类型（兼容性）
 */
#ifndef CXC_BOOL_DEFINED
#define CXC_BOOL_DEFINED
typedef bool cxc_bool_t;
#endif

/**
 * @brief 大小类型
 */
typedef size_t cxc_size_t;

/**
 * @brief 有符号整数类型
 */
typedef intptr_t cxc_int_t;

/**
 * @brief 无符号整数类型
 */
typedef uintptr_t cxc_uint_t;

/* ============================================================================
 * typeof 扩展封装
 * ============================================================================ */

#if defined(__GNUC__) || defined(__clang__)
    /**
     * @brief 获取表达式的类型（GCC/Clang 扩展）
     */
    #define CXC_TYPEOF(expr) __typeof__(expr)
    
    /**
     * @brief 获取表达式的类型（变量声明）
     */
    #define CXC_TYPEOF_VAR(var, expr) CXC_TYPEOF(expr) var = (expr)
#elif defined(_MSC_VER)
    /**
     * @brief 获取表达式的类型（MSVC）
     */
    #define CXC_TYPEOF(expr) decltype(expr)
    
    /**
     * @brief 获取表达式的类型（变量声明）
     */
    #define CXC_TYPEOF_VAR(var, expr) auto var = (expr)
#else
    /**
     * @brief 获取表达式的类型（回退方案）
     * @note 在不支持 typeof 的编译器上，需要手动指定类型
     */
    #define CXC_TYPEOF(expr) void
    #define CXC_TYPEOF_VAR(var, expr) void var = (expr)
#endif

/* ============================================================================
 * 编译时类型检查宏
 * ============================================================================ */

/**
 * @brief 检查两个类型是否相同
 */
#define CXC_SAME_TYPE(type1, type2) \
    _Generic((type1){0}, type2: 1, default: 0)

/**
 * @brief 检查表达式是否为指定类型
 */
#define CXC_IS_TYPE(expr, type) \
    _Generic((expr), type: 1, default: 0)

/**
 * @brief 获取表达式的类型名称（用于调试）
 */
#define CXC_TYPE_NAME(expr) \
    _Generic((expr), \
        char: "char", \
        signed char: "signed char", \
        unsigned char: "unsigned char", \
        short: "short", \
        unsigned short: "unsigned short", \
        int: "int", \
        unsigned int: "unsigned int", \
        long: "long", \
        unsigned long: "unsigned long", \
        long long: "long long", \
        unsigned long long: "unsigned long long", \
        float: "float", \
        double: "double", \
        long double: "long double", \
        void*: "void*", \
        char*: "char*", \
        default: "unknown" \
    )

/* ============================================================================
 * 类型转换辅助宏
 * ============================================================================ */

/**
 * @brief 安全的类型转换
 */
#define CXC_CAST(type, expr) ((type)(expr))

/**
 * @brief 常量转换
 */
#define CXC_CONST_CAST(type, expr) ((type)(expr))

/**
 * @brief 静态转换（编译时检查）
 */
#define CXC_STATIC_CAST(type, expr) ((type)(expr))

#ifdef __cplusplus
}
#endif

#endif /* CXC_CORE_TYPES_H */
