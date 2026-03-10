/**
 * @file error.c
 * @brief 错误处理工具实现
 */

#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* cxc_error_string(cxc_error_code_t code) {
    switch (code) {
        case CXC_ERROR_NONE:
            return "No error";
        case CXC_ERROR_INVALID_ARGUMENT:
            return "Invalid argument";
        case CXC_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case CXC_ERROR_NOT_FOUND:
            return "Not found";
        case CXC_ERROR_ALREADY_EXISTS:
            return "Already exists";
        case CXC_ERROR_INVALID_STATE:
            return "Invalid state";
        case CXC_ERROR_NOT_SUPPORTED:
            return "Not supported";
        case CXC_ERROR_IO_ERROR:
            return "IO error";
        case CXC_ERROR_TIMEOUT:
            return "Timeout";
        case CXC_ERROR_UNKNOWN:
        default:
            return "Unknown error";
    }
}

#ifdef __cplusplus
}
#endif
