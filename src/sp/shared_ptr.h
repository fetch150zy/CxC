/**
 * @file shared_ptr.h
 * @brief shared_ptr 实现
 * 
 * 提供共享所有权指针、引用计数管理和线程安全控制块
 */

#ifndef CXC_SP_SHARED_PTR_H
#define CXC_SP_SHARED_PTR_H

#include "../core/core.h"
#include "raii.h"
#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 控制块结构
 * ============================================================================ */

/**
 * @brief 引用计数控制块
 */
typedef struct cxc_control_block {
    atomic_uint ref_count;        /**< 引用计数 */
    atomic_uint weak_count;       /**< 弱引用计数 */
    cxc_ptr_t ptr;                /**< 原始指针 */
    void (*deleter)(cxc_ptr_t);   /**< 删除器函数 */
} cxc_control_block_t;

/**
 * @brief 创建控制块
 * @param ptr 原始指针
 * @param deleter 删除器函数（可为 NULL）
 * @return 控制块指针，失败返回 NULL
 */
cxc_control_block_t* cxc_control_block_create(cxc_ptr_t ptr, void (*deleter)(cxc_ptr_t));

/**
 * @brief 增加引用计数
 * @param cb 控制块指针
 */
void cxc_control_block_add_ref(cxc_control_block_t* cb);

/**
 * @brief 减少引用计数
 * @param cb 控制块指针
 * @return 如果引用计数为 0 返回 true，否则返回 false
 */
cxc_bool_t cxc_control_block_release_ref(cxc_control_block_t* cb);

/**
 * @brief 增加弱引用计数
 * @param cb 控制块指针
 */
void cxc_control_block_add_weak_ref(cxc_control_block_t* cb);

/**
 * @brief 减少弱引用计数
 * @param cb 控制块指针
 */
void cxc_control_block_release_weak_ref(cxc_control_block_t* cb);

/* ============================================================================
 * shared_ptr 结构
 * ============================================================================ */

/**
 * @brief shared_ptr 结构
 */
typedef struct cxc_shared_ptr {
    cxc_control_block_t* control_block;  /**< 控制块指针 */
} cxc_shared_ptr_t;

/* ============================================================================
 * shared_ptr 操作
 * ============================================================================ */

/**
 * @brief 创建 shared_ptr
 * @param ptr 原始指针
 * @param deleter 删除器函数（可为 NULL）
 * @return shared_ptr 结构
 */
cxc_shared_ptr_t cxc_shared_ptr_create(cxc_ptr_t ptr, void (*deleter)(cxc_ptr_t));

/**
 * @brief 创建 shared_ptr（从另一个 shared_ptr）
 * @param other 另一个 shared_ptr
 * @return shared_ptr 结构
 */
cxc_shared_ptr_t cxc_shared_ptr_copy(const cxc_shared_ptr_t* other);

/**
 * @brief 释放 shared_ptr
 * @param sp shared_ptr 指针
 */
void cxc_shared_ptr_release(cxc_shared_ptr_t* sp);

/**
 * @brief 重置 shared_ptr
 * @param sp shared_ptr 指针
 * @param ptr 新指针（可为 NULL）
 * @param deleter 删除器函数（可为 NULL）
 */
void cxc_shared_ptr_reset(cxc_shared_ptr_t* sp, cxc_ptr_t ptr, void (*deleter)(cxc_ptr_t));

/**
 * @brief 获取原始指针
 * @param sp shared_ptr 指针
 * @return 原始指针
 */
static inline cxc_ptr_t cxc_shared_ptr_get(const cxc_shared_ptr_t* sp) {
    if (sp == NULL || sp->control_block == NULL) {
        return NULL;
    }
    return sp->control_block->ptr;
}

/**
 * @brief 获取原始指针（指定类型）
 * @param sp shared_ptr 指针
 * @param type 类型
 */
#define CXC_SHARED_PTR_GET(sp, type) \
    ((type*)cxc_shared_ptr_get(sp))

/**
 * @brief 检查是否为空
 * @param sp shared_ptr 指针
 * @return 如果为空返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_shared_ptr_is_null(const cxc_shared_ptr_t* sp) {
    return cxc_shared_ptr_get(sp) == NULL;
}

/**
 * @brief 获取引用计数
 * @param sp shared_ptr 指针
 * @return 引用计数
 */
cxc_uint_t cxc_shared_ptr_use_count(const cxc_shared_ptr_t* sp);

/**
 * @brief 解引用 shared_ptr
 * @param sp shared_ptr 指针
 * @param type 类型
 */
#define CXC_SHARED_PTR_DEREF(sp, type) \
    (*(CXC_SHARED_PTR_GET(sp, type)))

/* ============================================================================
 * 便捷宏
 * ============================================================================ */

/**
 * @brief 创建 shared_ptr（自动分配）
 * @param type 类型
 * @param count 数量
 * @param deleter 删除器函数（可为 NULL）
 */
#define CXC_MAKE_SHARED(type, count, deleter) \
    cxc_shared_ptr_create(CXC_MALLOC(type, count), deleter)

/**
 * @brief 创建 shared_ptr（自动分配并清零）
 * @param type 类型
 * @param count 数量
 * @param deleter 删除器函数（可为 NULL）
 */
#define CXC_MAKE_SHARED_ZEROED(type, count, deleter) \
    cxc_shared_ptr_create(CXC_CALLOC(type, count), deleter)

#ifdef __cplusplus
}
#endif

#endif /* CXC_SP_SHARED_PTR_H */
