/**
 * @file template.h
 * @brief 模板（泛型）支持
 * 
 * 通过宏实现类似 C++ 模板的泛型编程
 */

#ifndef CXC_TEMPLATE_H
#define CXC_TEMPLATE_H

#include "../core/core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 模板宏定义
 * ============================================================================ */

/**
 * @brief 定义模板函数
 * @param template_name 模板名称
 * @param type_param 类型参数名
 * @param func_name 函数名
 */
#define CXC_TEMPLATE_DEFINE(template_name, type_param, func_name) \
    template_name##_##func_name

/**
 * @brief 实例化模板函数
 * @param template_name 模板名称
 * @param type 具体类型
 * @param func_name 函数名
 */
#define CXC_TEMPLATE_INSTANTIATE(template_name, type, func_name) \
    template_name##_##type##_##func_name

/* ============================================================================
 * 类型特化
 * ============================================================================ */

/**
 * @brief 定义类型特化
 * @param template_name 模板名称
 * @param type 特化类型
 * @param func_name 函数名
 */
#define CXC_TEMPLATE_SPECIALIZE(template_name, type, func_name) \
    template_name##_##type##_##func_name

/* ============================================================================
 * 泛型函数宏（使用 _Generic）
 * ============================================================================ */

/**
 * @brief 泛型函数调用（使用 _Generic）
 * @param expr 表达式
 * @param default_func 默认函数
 * @param ... 类型-函数对列表
 */
#define CXC_TEMPLATE_CALL(expr, default_func, ...) \
    _Generic((expr), __VA_ARGS__, default: default_func)(expr)

/**
 * @brief 泛型函数调用（多参数）
 */
#define CXC_TEMPLATE_CALL_MULTI(expr1, expr2, default_func, ...) \
    _Generic((expr1), __VA_ARGS__, default: default_func)(expr1, expr2)

/* ============================================================================
 * 类型特征（Type Traits）
 * ============================================================================ */

/**
 * @brief 检查类型是否为指针
 */
#define CXC_IS_POINTER(type) \
    _Generic((type){0}, \
        void*: 1, \
        char*: 1, \
        int*: 1, \
        default: 0 \
    )

/**
 * @brief 获取类型大小
 */
#define CXC_TYPE_SIZE(type) sizeof(type)

/**
 * @brief 获取类型对齐
 */
#define CXC_TYPE_ALIGN(type) _Alignof(type)

/**
 * @brief 检查表达式类型是否匹配
 * @param expr 表达式
 * @param type 类型名
 */
#define CXC_IS_SAME_TYPE(expr, type) \
    _Generic((expr), type: 1, default: 0)

/* ============================================================================
 * 模板类宏
 * ============================================================================ */

/**
 * @brief 定义模板类
 * @param class_name 类名
 * @param type_param 类型参数名
 */
#define CXC_TEMPLATE_CLASS_BEGIN(class_name, type_param) \
    typedef struct class_name##_##type_param { \
        type_param data;

/**
 * @brief 模板类结束
 */
#define CXC_TEMPLATE_CLASS_END(class_name, type_param) \
    } class_name##_##type_param##_t;

/* ============================================================================
 * 常用模板函数
 * ============================================================================ */

/**
 * @brief 交换两个值（泛型）
 */
#define CXC_SWAP_TEMPLATE(a, b) \
    do { \
        CXC_TYPEOF(a) _temp = (a); \
        (a) = (b); \
        (b) = _temp; \
    } while (0)

/**
 * @brief 最大值（泛型）
 */
#define CXC_MAX_TEMPLATE(a, b) \
    ((a) > (b) ? (a) : (b))

/**
 * @brief 最小值（泛型）
 */
#define CXC_MIN_TEMPLATE(a, b) \
    ((a) < (b) ? (a) : (b))

/**
 * @brief 比较（泛型）
 */
#define CXC_COMPARE_TEMPLATE(a, b) \
    ((a) > (b) ? 1 : ((a) < (b) ? -1 : 0))

#ifdef __cplusplus
}
#endif

#endif /* CXC_TEMPLATE_H */
