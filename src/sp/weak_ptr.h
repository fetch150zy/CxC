/**
 * @file weak_ptr.h
 * @brief weak_ptr 实现
 * 
 * 提供弱引用指针和打破循环引用
 */

#ifndef CXC_SP_WEAK_PTR_H
#define CXC_SP_WEAK_PTR_H

#include "../core/core.h"
#include "shared_ptr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * weak_ptr 结构
 * ============================================================================ */

/**
 * @brief weak_ptr 结构
 */
typedef struct cxc_weak_ptr {
    cxc_control_block_t* control_block;  /**< 控制块指针 */
} cxc_weak_ptr_t;

/* ============================================================================
 * weak_ptr 操作
 * ============================================================================ */

/**
 * @brief 创建 weak_ptr（从 shared_ptr）
 * @param sp shared_ptr 指针
 * @return weak_ptr 结构
 */
cxc_weak_ptr_t cxc_weak_ptr_create(const cxc_shared_ptr_t* sp);

/**
 * @brief 创建 weak_ptr（从另一个 weak_ptr）
 * @param other 另一个 weak_ptr
 * @return weak_ptr 结构
 */
cxc_weak_ptr_t cxc_weak_ptr_copy(const cxc_weak_ptr_t* other);

/**
 * @brief 释放 weak_ptr
 * @param wp weak_ptr 指针
 */
void cxc_weak_ptr_release(cxc_weak_ptr_t* wp);

/**
 * @brief 重置 weak_ptr
 * @param wp weak_ptr 指针
 */
void cxc_weak_ptr_reset(cxc_weak_ptr_t* wp);

/**
 * @brief 从 weak_ptr 创建 shared_ptr
 * @param wp weak_ptr 指针
 * @return shared_ptr 结构，如果对象已销毁则返回空的 shared_ptr
 */
cxc_shared_ptr_t cxc_weak_ptr_lock(const cxc_weak_ptr_t* wp);

/**
 * @brief 检查 weak_ptr 是否过期
 * @param wp weak_ptr 指针
 * @return 如果过期返回 true，否则返回 false
 */
cxc_bool_t cxc_weak_ptr_expired(const cxc_weak_ptr_t* wp);

/**
 * @brief 获取引用计数
 * @param wp weak_ptr 指针
 * @return 引用计数
 */
cxc_uint_t cxc_weak_ptr_use_count(const cxc_weak_ptr_t* wp);

#ifdef __cplusplus
}
#endif

#endif /* CXC_SP_WEAK_PTR_H */
