/**
 * @file lambda.h
 * @brief Lambda 表达式支持
 * 
 * 通过宏和函数指针实现类似 C++ lambda 的语法
 */

#ifndef CXC_LAMBDA_H
#define CXC_LAMBDA_H

#include "../core/core.h"
#include "../util/function.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Lambda 函数类型
 * ============================================================================ */

/**
 * @brief Lambda 函数指针类型（无参数）
 */
typedef cxc_ptr_t (*cxc_lambda_0_t)(void);

/**
 * @brief Lambda 函数指针类型（单参数）
 */
typedef cxc_ptr_t (*cxc_lambda_1_t)(cxc_ptr_t);

/**
 * @brief Lambda 函数指针类型（双参数）
 */
typedef cxc_ptr_t (*cxc_lambda_2_t)(cxc_ptr_t, cxc_ptr_t);

/**
 * @brief Lambda 函数指针类型（三参数）
 */
typedef cxc_ptr_t (*cxc_lambda_3_t)(cxc_ptr_t, cxc_ptr_t, cxc_ptr_t);

/* ============================================================================
 * Lambda 结构
 * ============================================================================ */

/**
 * @brief Lambda 结构
 */
typedef struct cxc_lambda {
    cxc_ptr_t data;              /**< 捕获的数据 */
    cxc_ptr_t (*invoke)(cxc_ptr_t, ...); /**< 调用函数 */
    void (*destroy)(cxc_ptr_t);  /**< 销毁函数 */
    cxc_size_t arg_count;        /**< 参数数量 */
} cxc_lambda_t;

/* ============================================================================
 * Lambda 创建宏
 * ============================================================================ */

/**
 * @brief 定义 Lambda 函数（无参数）
 * @param name Lambda 名称
 * @param body 函数体
 */
#define CXC_DEFINE_LAMBDA_0(name, body) \
    static cxc_ptr_t name(void) { \
        body \
    }

/**
 * @brief 定义 Lambda 函数（单参数）
 */
#define CXC_DEFINE_LAMBDA_1(name, param, body) \
    static cxc_ptr_t name(cxc_ptr_t param) { \
        body \
    }

/**
 * @brief 定义 Lambda 函数（双参数）
 */
#define CXC_DEFINE_LAMBDA_2(name, param1, param2, body) \
    static cxc_ptr_t name(cxc_ptr_t param1, cxc_ptr_t param2) { \
        body \
    }

/**
 * @brief 定义 Lambda 函数（三参数）
 */
#define CXC_DEFINE_LAMBDA_3(name, param1, param2, param3, body) \
    static cxc_ptr_t name(cxc_ptr_t param1, cxc_ptr_t param2, cxc_ptr_t param3) { \
        body \
    }

/* ============================================================================
 * Lambda 辅助宏
 * ============================================================================ */

/**
 * @brief 捕获变量（按值）
 */
#define CXC_CAPTURE_BY_VALUE(type, name, value) \
    type name = *(type*)(value);

/**
 * @brief 捕获变量（按引用）
 */
#define CXC_CAPTURE_BY_REF(type, name, value) \
    type* name = (type*)(value);

/**
 * @brief 创建 Lambda 包装器（无参数）
 */
#define CXC_LAMBDA_WRAP_0(func) \
    (cxc_lambda_0_t)(func)

/**
 * @brief 创建 Lambda 包装器（单参数）
 */
#define CXC_LAMBDA_WRAP_1(func) \
    (cxc_lambda_1_t)(func)

/**
 * @brief 创建 Lambda 包装器（双参数）
 */
#define CXC_LAMBDA_WRAP_2(func) \
    (cxc_lambda_2_t)(func)

#ifdef __cplusplus
}
#endif

#endif /* CXC_LAMBDA_H */
