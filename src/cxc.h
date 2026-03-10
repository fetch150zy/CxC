/**
 * @file cxc.h
 * @brief CxC 主头文件
 * 
 * 包含所有 CxC 模块的头文件
 */

#ifndef CXC_H
#define CXC_H

/* 核心模块 */
#include "core/core.h"

/* 面向对象模块 */
#include "oop/oop.h"

/* 智能指针模块 */
#include "sp/sp.h"

/* 容器模块 */
#include "container/container.h"

/* 工具函数模块 */
#include "util/util.h"

/* 异常处理模块 */
#include "exception/exception.h"

/* Lambda 模块 */
#include "lambda/lambda.h"

/* 模板模块 */
#include "template/template.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief CxC 版本信息
 */
#define CXC_VERSION_MAJOR 0
#define CXC_VERSION_MINOR 1
#define CXC_VERSION_PATCH 0

/**
 * @brief CxC 版本号
 */
#define CXC_VERSION \
    ((CXC_VERSION_MAJOR << 16) | (CXC_VERSION_MINOR << 8) | CXC_VERSION_PATCH)

#ifdef __cplusplus
}
#endif

#endif /* CXC_H */
