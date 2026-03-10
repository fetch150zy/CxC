/**
 * @file exception.h
 * @brief 异常处理机制
 *
 * 使用 setjmp/longjmp 实现类似 C++ 的异常机制
 */

#ifndef CXC_EXCEPTION_H
#define CXC_EXCEPTION_H

#include "../core/core.h"
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 异常类型
 * ============================================================================ */

typedef struct cxc_exception {
    cxc_error_code_t code;
    const char* message;
    const char* file;
    int line;
    const char* function;
} cxc_exception_t;

typedef struct cxc_exception_context {
    jmp_buf env;
    cxc_exception_t exception;
    struct cxc_exception_context* prev;
} cxc_exception_context_t;

/* ============================================================================
 * 异常处理函数
 * ============================================================================ */

cxc_exception_context_t* cxc_exception_get_context(void);
void cxc_exception_set_context(cxc_exception_context_t* ctx);

void cxc_exception_throw(
    cxc_error_code_t code,
    const char* message,
    const char* file,
    int line,
    const char* function
);

/* 获取上次捕获的异常 */
cxc_exception_t* cxc_exception_last(void);

void cxc_exception_rethrow(void);
cxc_exception_t* cxc_exception_current(void);
cxc_bool_t cxc_exception_has_uncaught(void);

/* ============================================================================
 * Try-Catch 宏
 * 
 * 使用方式:
 *   CXC_TRY_BEGIN() {
 *       // 可能抛出异常的代码
 *       CXC_THROW(CXC_ERROR_INVALID_ARGUMENT, "Error message");
 *   }
 *   CXC_TRY_END();
 *   
 *   // 检查是否有异常
 *   cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
 *   if (exc->code != CXC_ERROR_NONE) {
 *       // 处理异常
 *   }
 * ============================================================================ */

/* 取消定义 error.h 中的 CXC_TRY 以避免冲突 */
#ifdef CXC_TRY
    #undef CXC_TRY
#endif

#define CXC_THROW(code, msg) \
    cxc_exception_throw(code, msg, __FILE__, __LINE__, __func__)

#define CXC_THROW_ERROR(code) \
    CXC_THROW(code, cxc_error_string(code))

/**
 * @brief 获取最后一次捕获的异常（在 try 块外使用）
 */
#define CXC_GET_LAST_EXCEPTION() cxc_exception_last()

/**
 * @brief Try 块开始
 * 
 * 使用方式:
 *   CXC_TRY_BEGIN() {
 *       // 可能抛出异常的代码
 *   }
 *   CXC_TRY_END();
 */
#define CXC_TRY_BEGIN() \
    do { \
        cxc_exception_context_t _cxc_exc_ctx_ = {0}; \
        cxc_exception_context_t* _cxc_exc_prev_ = cxc_exception_get_context(); \
        _cxc_exc_ctx_.exception.code = CXC_ERROR_NONE; \
        _cxc_exc_ctx_.prev = _cxc_exc_prev_; \
        cxc_exception_set_context(&_cxc_exc_ctx_); \
        cxc_exception_last()->code = CXC_ERROR_NONE; \
        if (setjmp(_cxc_exc_ctx_.env) == 0)

/**
 * @brief Try 块结束
 */
#define CXC_TRY_END() \
        else { \
            *cxc_exception_last() = _cxc_exc_ctx_.exception; \
        } \
        cxc_exception_set_context(_cxc_exc_prev_); \
    } while (0)

/**
 * @brief 抛出异常（带条件检查）
 */
#define CXC_THROW_IF(cond, code, msg) \
    do { if (cond) CXC_THROW(code, msg); } while (0)

/**
 * @brief 重新抛出当前异常（在 catch 块内使用）
 */
#define CXC_RETHROW() \
    do { \
        cxc_exception_throw(cxc_exception_last()->code, \
                           cxc_exception_last()->message, \
                           cxc_exception_last()->file, \
                           cxc_exception_last()->line, \
                           cxc_exception_last()->function); \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* CXC_EXCEPTION_H */