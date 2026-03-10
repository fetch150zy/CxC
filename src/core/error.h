/**
 * @file error.h
 * @brief 错误处理工具
 * 
 * 提供结果类型、错误码定义和断言增强
 */

#ifndef CXC_CORE_ERROR_H
#define CXC_CORE_ERROR_H

#include "types.h"
#include <assert.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 错误码定义
 * ============================================================================ */

/**
 * @brief 错误码类型
 */
typedef enum cxc_error_code {
    CXC_ERROR_NONE = 0,              /**< 无错误 */
    CXC_ERROR_INVALID_ARGUMENT,       /**< 无效参数 */
    CXC_ERROR_OUT_OF_MEMORY,          /**< 内存不足 */
    CXC_ERROR_NOT_FOUND,              /**< 未找到 */
    CXC_ERROR_ALREADY_EXISTS,         /**< 已存在 */
    CXC_ERROR_INVALID_STATE,          /**< 无效状态 */
    CXC_ERROR_NOT_SUPPORTED,          /**< 不支持 */
    CXC_ERROR_IO_ERROR,               /**< IO 错误 */
    CXC_ERROR_TIMEOUT,                /**< 超时 */
    CXC_ERROR_UNKNOWN,                /**< 未知错误 */
    CXC_ERROR_COUNT                   /**< 错误码数量 */
} cxc_error_code_t;

/**
 * @brief 获取错误码的字符串描述
 * @param code 错误码
 * @return 错误描述字符串
 */
const char* cxc_error_string(cxc_error_code_t code);

/* ============================================================================
 * 结果类型 (Result<T, E>)
 * ============================================================================ */

/**
 * @brief 结果类型结构
 */
typedef struct cxc_result {
    cxc_bool_t is_ok;         /**< 是否成功 */
    cxc_error_code_t error;    /**< 错误码 */
    union {
        cxc_ptr_t value;       /**< 值指针 */
        cxc_uint_t value_uint; /**< 无符号整数值 */
        cxc_int_t value_int;   /**< 有符号整数值 */
    };
} cxc_result_t;

/**
 * @brief 创建成功结果
 * @param value 值指针
 * @return 结果结构
 */
static inline cxc_result_t cxc_result_ok(cxc_ptr_t value) {
    cxc_result_t result = {
        .is_ok = true,
        .error = CXC_ERROR_NONE,
        .value = value
    };
    return result;
}

/**
 * @brief 创建成功结果（无符号整数）
 * @param value 无符号整数值
 * @return 结果结构
 */
static inline cxc_result_t cxc_result_ok_uint(cxc_uint_t value) {
    cxc_result_t result = {
        .is_ok = true,
        .error = CXC_ERROR_NONE,
        .value_uint = value
    };
    return result;
}

/**
 * @brief 创建成功结果（有符号整数）
 * @param value 有符号整数值
 * @return 结果结构
 */
static inline cxc_result_t cxc_result_ok_int(cxc_int_t value) {
    cxc_result_t result = {
        .is_ok = true,
        .error = CXC_ERROR_NONE,
        .value_int = value
    };
    return result;
}

/**
 * @brief 创建错误结果
 * @param error 错误码
 * @return 结果结构
 */
static inline cxc_result_t cxc_result_err(cxc_error_code_t error) {
    cxc_result_t result = {
        .is_ok = false,
        .error = error,
        .value = NULL
    };
    return result;
}

/**
 * @brief 检查结果是否成功
 * @param result 结果结构
 * @return 成功返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_result_is_ok(const cxc_result_t* result) {
    return result != NULL && result->is_ok;
}

/**
 * @brief 检查结果是否失败
 * @param result 结果结构
 * @return 失败返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_result_is_err(const cxc_result_t* result) {
    return result == NULL || !result->is_ok;
}

/**
 * @brief 获取结果的值（指针）
 * @param result 结果结构
 * @return 值指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_result_unwrap(cxc_result_t* result) {
    if (cxc_result_is_ok(result)) {
        return result->value;
    }
    return NULL;
}

/**
 * @brief 获取结果的值（无符号整数）
 * @param result 结果结构
 * @return 无符号整数值，失败返回 0
 */
static inline cxc_uint_t cxc_result_unwrap_uint(cxc_result_t* result) {
    if (cxc_result_is_ok(result)) {
        return result->value_uint;
    }
    return 0;
}

/**
 * @brief 获取结果的值（有符号整数）
 * @param result 结果结构
 * @return 有符号整数值，失败返回 0
 */
static inline cxc_int_t cxc_result_unwrap_int(cxc_result_t* result) {
    if (cxc_result_is_ok(result)) {
        return result->value_int;
    }
    return 0;
}

/**
 * @brief 获取结果的错误码
 * @param result 结果结构
 * @return 错误码
 */
static inline cxc_error_code_t cxc_result_error(const cxc_result_t* result) {
    if (result == NULL) {
        return CXC_ERROR_UNKNOWN;
    }
    return result->error;
}

/* ============================================================================
 * 断言增强
 * ============================================================================ */

/**
 * @brief 运行时断言（带消息）
 */
#define CXC_ASSERT(condition, message) \
    assert((condition) && (message))

/**
 * @brief 运行时断言（简化版）
 */
#define CXC_ASSERT_S(condition) \
    assert(condition)

/**
 * @brief 断言失败时执行操作
 */
#define CXC_ASSERT_OR(condition, action) \
    do { \
        if (!(condition)) { \
            action; \
            assert(0); \
        } \
    } while (0)

/**
 * @brief 检查指针有效性
 */
#define CXC_ASSERT_PTR(ptr) \
    CXC_ASSERT((ptr) != NULL, "Pointer is NULL")

/**
 * @brief 检查索引有效性
 */
#define CXC_ASSERT_INDEX(index, size) \
    CXC_ASSERT((index) < (size), "Index out of bounds")

/**
 * @brief 不可达代码标记
 */
#define CXC_UNREACHABLE() \
    assert(0 && "Unreachable code")

/* ============================================================================
 * 错误处理辅助宏
 * ============================================================================ */

/**
 * @brief 检查结果并返回错误
 */
#define CXC_TRY(result_var, expr) \
    do { \
        cxc_result_t result_var = (expr); \
        if (cxc_result_is_err(&result_var)) { \
            return cxc_result_err(result_var.error); \
        } \
    } while (0)

/**
 * @brief 检查结果并执行错误处理
 */
#define CXC_TRY_OR(result_var, expr, error_handler) \
    do { \
        cxc_result_t result_var = (expr); \
        if (cxc_result_is_err(&result_var)) { \
            error_handler; \
            return cxc_result_err(result_var.error); \
        } \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* CXC_CORE_ERROR_H */
