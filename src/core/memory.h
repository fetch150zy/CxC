/**
 * @file memory.h
 * @brief 内存管理工具
 * 
 * 提供统一的内存分配/释放接口、内存对齐工具和内存池基础
 */

#ifndef CXC_CORE_MEMORY_H
#define CXC_CORE_MEMORY_H

#include "types.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 内存分配接口
 * ============================================================================ */

/**
 * @brief 分配指定大小的内存
 * @param size 要分配的字节数
 * @return 指向分配内存的指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_malloc(cxc_size_t size) {
    return malloc(size);
}

/**
 * @brief 分配并清零指定大小的内存
 * @param size 要分配的字节数
 * @return 指向分配内存的指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_calloc(cxc_size_t size) {
    return calloc(1, size);
}

/**
 * @brief 重新分配内存
 * @param ptr 原内存指针
 * @param size 新大小
 * @return 指向重新分配内存的指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_realloc(cxc_ptr_t ptr, cxc_size_t size) {
    return realloc(ptr, size);
}

/**
 * @brief 释放内存
 * @param ptr 要释放的内存指针
 */
static inline void cxc_free(cxc_ptr_t ptr) {
    free(ptr);
}

/**
 * @brief 分配指定类型的内存
 * @param type 类型
 * @param count 数量
 * @return 指向分配内存的指针，失败返回 NULL
 */
#define CXC_MALLOC(type, count) \
    ((type*)cxc_malloc(sizeof(type) * (count)))

/**
 * @brief 分配并清零指定类型的内存
 * @param type 类型
 * @param count 数量
 * @return 指向分配内存的指针，失败返回 NULL
 */
#define CXC_CALLOC(type, count) \
    ((type*)cxc_calloc(sizeof(type) * (count)))

/**
 * @brief 重新分配指定类型的内存
 * @param type 类型
 * @param ptr 原内存指针
 * @param count 新数量
 * @return 指向重新分配内存的指针，失败返回 NULL
 */
#define CXC_REALLOC(type, ptr, count) \
    ((type*)cxc_realloc(ptr, sizeof(type) * (count)))

/* ============================================================================
 * 内存对齐工具
 * ============================================================================ */

/**
 * @brief 计算对齐后的地址
 * @param addr 地址
 * @param alignment 对齐字节数（必须是2的幂）
 * @return 对齐后的地址
 */
static inline uintptr_t cxc_align_up(uintptr_t addr, cxc_size_t alignment) {
    return (addr + alignment - 1) & ~(alignment - 1);
}

/**
 * @brief 计算对齐后的地址（向下）
 * @param addr 地址
 * @param alignment 对齐字节数（必须是2的幂）
 * @return 对齐后的地址
 */
static inline uintptr_t cxc_align_down(uintptr_t addr, cxc_size_t alignment) {
    return addr & ~(alignment - 1);
}

/**
 * @brief 检查地址是否对齐
 * @param addr 地址
 * @param alignment 对齐字节数（必须是2的幂）
 * @return 如果对齐返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_is_aligned(uintptr_t addr, cxc_size_t alignment) {
    return (addr & (alignment - 1)) == 0;
}

/**
 * @brief 分配对齐的内存
 * @param size 要分配的字节数
 * @param alignment 对齐字节数（必须是2的幂）
 * @return 指向分配内存的指针，失败返回 NULL
 */
cxc_ptr_t cxc_aligned_malloc(cxc_size_t size, cxc_size_t alignment);

/**
 * @brief 释放对齐的内存
 * @param ptr 要释放的内存指针
 */
void cxc_aligned_free(cxc_ptr_t ptr);

/**
 * @brief 分配对齐的指定类型的内存
 * @param type 类型
 * @param count 数量
 * @param alignment 对齐字节数（必须是2的幂）
 * @return 指向分配内存的指针，失败返回 NULL
 */
#define CXC_ALIGNED_MALLOC(type, count, alignment) \
    ((type*)cxc_aligned_malloc(sizeof(type) * (count), alignment))

/* ============================================================================
 * 内存操作工具
 * ============================================================================ */

/**
 * @brief 复制内存
 * @param dest 目标地址
 * @param src 源地址
 * @param size 字节数
 * @return 目标地址
 */
static inline cxc_ptr_t cxc_memcpy(cxc_ptr_t dest, cxc_cptr_t src, cxc_size_t size) {
    return memcpy(dest, src, size);
}

/**
 * @brief 移动内存
 * @param dest 目标地址
 * @param src 源地址
 * @param size 字节数
 * @return 目标地址
 */
static inline cxc_ptr_t cxc_memmove(cxc_ptr_t dest, cxc_cptr_t src, cxc_size_t size) {
    return memmove(dest, src, size);
}

/**
 * @brief 设置内存值
 * @param dest 目标地址
 * @param value 要设置的值
 * @param size 字节数
 * @return 目标地址
 */
static inline cxc_ptr_t cxc_memset(cxc_ptr_t dest, int value, cxc_size_t size) {
    return memset(dest, value, size);
}

/**
 * @brief 比较内存
 * @param ptr1 第一个内存块
 * @param ptr2 第二个内存块
 * @param size 字节数
 * @return 如果相等返回 0，ptr1 < ptr2 返回负数，否则返回正数
 */
static inline int cxc_memcmp(cxc_cptr_t ptr1, cxc_cptr_t ptr2, cxc_size_t size) {
    return memcmp(ptr1, ptr2, size);
}

/* ============================================================================
 * 内存池基础（简单实现）
 * ============================================================================ */

/**
 * @brief 内存池结构
 */
typedef struct cxc_mempool {
    cxc_ptr_t blocks;           /**< 内存块链表 */
    cxc_size_t block_size;      /**< 每个块的大小 */
    cxc_size_t block_count;     /**< 块的数量 */
    cxc_size_t free_count;      /**< 空闲块数量 */
} cxc_mempool_t;

/**
 * @brief 创建内存池
 * @param block_size 每个块的大小
 * @param initial_count 初始块数量
 * @return 内存池指针，失败返回 NULL
 */
cxc_mempool_t* cxc_mempool_create(cxc_size_t block_size, cxc_size_t initial_count);

/**
 * @brief 从内存池分配内存
 * @param pool 内存池指针
 * @return 指向分配内存的指针，失败返回 NULL
 */
cxc_ptr_t cxc_mempool_alloc(cxc_mempool_t* pool);

/**
 * @brief 释放内存到内存池
 * @param pool 内存池指针
 * @param ptr 要释放的内存指针
 */
void cxc_mempool_free(cxc_mempool_t* pool, cxc_ptr_t ptr);

/**
 * @brief 销毁内存池
 * @param pool 内存池指针
 */
void cxc_mempool_destroy(cxc_mempool_t* pool);

#ifdef __cplusplus
}
#endif

#endif /* CXC_CORE_MEMORY_H */
