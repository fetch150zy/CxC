/**
 * @file exception.c
 * @brief 异常处理机制实现
 */

#include "exception.h"
#include <string.h>
#include <stdlib.h>

/* 全局异常存储 */
static cxc_exception_t g_cxc_last_exception = {CXC_ERROR_NONE, NULL, NULL, 0, NULL};

cxc_exception_t* cxc_exception_last(void) {
    return &g_cxc_last_exception;
}

/* 线程局部存储（简化实现，使用全局变量） */
static cxc_exception_context_t* g_exception_context = NULL;

cxc_exception_context_t* cxc_exception_get_context(void) {
    return g_exception_context;
}

void cxc_exception_set_context(cxc_exception_context_t* ctx) {
    g_exception_context = ctx;
}

void cxc_exception_throw(
    cxc_error_code_t code,
    const char* message,
    const char* file,
    int line,
    const char* function
) {
    cxc_exception_context_t* ctx = cxc_exception_get_context();
    
    if (ctx == NULL) {
        /* 没有异常上下文，使用 abort */
        fprintf(stderr, "Uncaught exception: %s (%d) at %s:%d in %s\n",
                message ? message : "Unknown", code, file, line, function);
        abort();
    }
    
    /* 初始化异常信息 */
    ctx->exception.code = code;
    ctx->exception.message = message;
    ctx->exception.file = file;
    ctx->exception.line = line;
    ctx->exception.function = function;
    
    /* 跳转到 setjmp 位置 */
    longjmp(ctx->env, 1);
}

void cxc_exception_rethrow(void) {
    cxc_exception_context_t* ctx = cxc_exception_get_context();
    
    if (ctx == NULL || ctx->prev == NULL) {
        fprintf(stderr, "No exception to rethrow\n");
        abort();
    }
    
    /* 将异常传递给上一级上下文 */
    cxc_exception_t* exc = &ctx->exception;
    cxc_exception_throw(
        exc->code,
        exc->message,
        exc->file,
        exc->line,
        exc->function
    );
}

cxc_exception_t* cxc_exception_current(void) {
    cxc_exception_context_t* ctx = cxc_exception_get_context();
    if (ctx != NULL) {
        /* 检查异常是否已设置（通过检查 code 是否为非零值） */
        if (ctx->exception.code != CXC_ERROR_NONE) {
            return &ctx->exception;
        }
    }
    return NULL;
}

cxc_bool_t cxc_exception_has_uncaught(void) {
    return cxc_exception_current() != NULL;
}

cxc_exception_t* cxc_exception_current_in_catch(void) {
    cxc_exception_context_t* ctx = cxc_exception_get_context();
    if (ctx != NULL && ctx->exception.code != CXC_ERROR_NONE) {
        return &ctx->exception;
    }
    return &g_cxc_last_exception;
}