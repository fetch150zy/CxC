/**
 * @file core.h
 * @brief CxC 核心模块统一头文件
 * 
 * 包含所有核心基础设施模块的头文件
 */

#ifndef CXC_CORE_H
#define CXC_CORE_H

#include "types.h"
#include "memory.h"
#include "macros.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 版本信息
 * ============================================================================ */

#define CXC_VERSION_MAJOR 0
#define CXC_VERSION_MINOR 1
#define CXC_VERSION_PATCH 0

#define CXC_VERSION \
    ((CXC_VERSION_MAJOR << 16) | (CXC_VERSION_MINOR << 8) | CXC_VERSION_PATCH)

#ifdef __cplusplus
}
#endif

#endif /* CXC_CORE_H */
