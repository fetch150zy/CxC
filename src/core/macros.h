/**
 * @file macros.h
 * @brief 宏工具集
 * 
 * 提供泛型宏、编译时断言和常用辅助宏
 */

#ifndef CXC_CORE_MACROS_H
#define CXC_CORE_MACROS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 编译时断言
 * ============================================================================ */

/**
 * @brief 编译时断言（C11）
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #define CXC_STATIC_ASSERT(condition, message) \
        _Static_assert(condition, message)
#elif defined(__GNUC__) || defined(__clang__)
    #define CXC_STATIC_ASSERT(condition, message) \
        typedef char CXC_STATIC_ASSERT_##__LINE__[(condition) ? 1 : -1]
#else
    #define CXC_STATIC_ASSERT(condition, message) \
        typedef char CXC_STATIC_ASSERT_##__LINE__[(condition) ? 1 : -1]
#endif

/**
 * @brief 编译时断言（简化版）
 */
#define CXC_STATIC_ASSERT_S(condition) \
    CXC_STATIC_ASSERT(condition, "Static assertion failed")

/* ============================================================================
 * 泛型宏 (C11 _Generic)
 * ============================================================================ */

/**
 * @brief 泛型选择宏
 * @param expr 表达式
 * @param ... 类型-值对列表，格式为 type: value
 * @return 匹配类型的值
 * 
 * @example
 * int result = CXC_GENERIC(x,
 *     int: x * 2,
 *     float: x * 2.0f,
 *     default: 0
 * );
 */
#define CXC_GENERIC(expr, ...) \
    _Generic((expr), __VA_ARGS__)

/**
 * @brief 泛型函数选择器
 * @param expr 表达式
 * @param int_func int 类型函数
 * @param float_func float 类型函数
 * @param double_func double 类型函数
 * @param default_func 默认函数
 * @return 函数调用结果
 */
#define CXC_GENERIC_FUNC(expr, int_func, float_func, double_func, default_func) \
    CXC_GENERIC((expr), \
        int: int_func, \
        float: float_func, \
        double: double_func, \
        default: default_func \
    )(expr)

/* ============================================================================
 * 常用辅助宏
 * ============================================================================ */

/**
 * @brief 数组大小
 */
#define CXC_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * @brief 结构体成员偏移量
 */
#define CXC_OFFSETOF(type, member) \
    ((cxc_size_t)&(((type*)0)->member))

/**
 * @brief 结构体成员大小
 */
#define CXC_MEMBER_SIZE(type, member) \
    sizeof(((type*)0)->member)

/**
 * @brief 获取结构体指针（从成员指针）
 */
#define CXC_CONTAINER_OF(ptr, type, member) \
    ((type*)((char*)(ptr) - CXC_OFFSETOF(type, member)))

/**
 * @brief 最小值
 */
#define CXC_MIN(a, b) \
    ((a) < (b) ? (a) : (b))

/**
 * @brief 最大值
 */
#define CXC_MAX(a, b) \
    ((a) > (b) ? (a) : (b))

/**
 * @brief 交换两个值
 */
#define CXC_SWAP(a, b) \
    do { \
        CXC_TYPEOF(a) _temp = (a); \
        (a) = (b); \
        (b) = _temp; \
    } while (0)

/**
 * @brief 字符串化
 */
#define CXC_STRINGIFY(x) #x

/**
 * @brief 字符串化（展开宏）
 */
#define CXC_STRINGIFY_EXPAND(x) CXC_STRINGIFY(x)

/**
 * @brief 连接两个标记
 */
#define CXC_CONCAT(a, b) a##b

/**
 * @brief 连接两个标记（展开宏）
 */
#define CXC_CONCAT_EXPAND(a, b) CXC_CONCAT(a, b)

/**
 * @brief 唯一标识符生成
 */
#define CXC_UNIQUE_ID(prefix) \
    CXC_CONCAT_EXPAND(prefix, __LINE__)

/* ============================================================================
 * 条件编译辅助
 * ============================================================================ */

/**
 * @brief 如果定义了宏则使用值，否则使用默认值
 */
#define CXC_IF_DEFINED(macro, value, default_value) \
    (defined(macro) ? (value) : (default_value))

/**
 * @brief 如果条件为真则使用值，否则使用默认值
 */
#define CXC_IF(condition, value, default_value) \
    ((condition) ? (value) : (default_value))

/* ============================================================================
 * 循环展开辅助
 * ============================================================================ */

/**
 * @brief 重复执行 N 次（编译时展开）
 */
#define CXC_REPEAT_0(func)
#define CXC_REPEAT_1(func) func(0)
#define CXC_REPEAT_2(func) CXC_REPEAT_1(func) func(1)
#define CXC_REPEAT_3(func) CXC_REPEAT_2(func) func(2)
#define CXC_REPEAT_4(func) CXC_REPEAT_3(func) func(3)
#define CXC_REPEAT_5(func) CXC_REPEAT_4(func) func(4)

#define CXC_REPEAT(n, func) CXC_CONCAT_EXPAND(CXC_REPEAT_, n)(func)

/* ============================================================================
 * 变参宏辅助
 * ============================================================================ */

/**
 * @brief 获取第一个参数
 */
#define CXC_FIRST_ARG(first, ...) first

/**
 * @brief 获取剩余参数
 */
#define CXC_REST_ARGS(first, ...) __VA_ARGS__

/**
 * @brief 参数数量（最多支持 10 个）
 */
#define CXC_ARG_COUNT(...) \
    CXC_ARG_COUNT_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define CXC_ARG_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

/* ============================================================================
 * 调试辅助
 * ============================================================================ */

/**
 * @brief 打印文件名和行号
 */
#define CXC_FILE_LINE \
    __FILE__ ":" CXC_STRINGIFY_EXPAND(__LINE__)

/**
 * @brief 打印函数名
 */
#define CXC_FUNCTION __func__

#ifdef __cplusplus
}
#endif

#endif /* CXC_CORE_MACROS_H */
