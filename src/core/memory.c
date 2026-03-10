/**
 * @file memory.c
 * @brief 内存管理工具实现
 */

#include "memory.h"
#include <stdalign.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 对齐内存分配实现
 * ============================================================================ */

cxc_ptr_t cxc_aligned_malloc(cxc_size_t size, cxc_size_t alignment) {
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        return NULL; /* alignment 必须是2的幂 */
    }
    
    /* 分配额外空间存储原始指针 */
    cxc_size_t total_size = size + alignment + sizeof(void*);
    void* raw_ptr = malloc(total_size);
    
    if (raw_ptr == NULL) {
        return NULL;
    }
    
    /* 计算对齐后的地址 */
    uintptr_t addr = (uintptr_t)raw_ptr + sizeof(void*);
    uintptr_t aligned_addr = cxc_align_up(addr, alignment);
    
    /* 在对齐地址前存储原始指针 */
    void** ptr_storage = (void**)(aligned_addr - sizeof(void*));
    *ptr_storage = raw_ptr;
    
    return (void*)aligned_addr;
}

void cxc_aligned_free(cxc_ptr_t ptr) {
    if (ptr == NULL) {
        return;
    }
    
    /* 获取原始指针 */
    void** ptr_storage = (void**)((uintptr_t)ptr - sizeof(void*));
    void* raw_ptr = *ptr_storage;
    
    free(raw_ptr);
}

/* ============================================================================
 * 内存池实现
 * ============================================================================ */

typedef struct cxc_mempool_block {
    struct cxc_mempool_block* next;
} cxc_mempool_block_t;

cxc_mempool_t* cxc_mempool_create(cxc_size_t block_size, cxc_size_t initial_count) {
    if (block_size == 0 || initial_count == 0) {
        return NULL;
    }
    
    /* 确保块大小至少能容纳一个指针 */
    cxc_size_t actual_block_size = block_size;
    if (actual_block_size < sizeof(cxc_mempool_block_t)) {
        actual_block_size = sizeof(cxc_mempool_block_t);
    }
    
    cxc_mempool_t* pool = (cxc_mempool_t*)cxc_malloc(sizeof(cxc_mempool_t));
    if (pool == NULL) {
        return NULL;
    }
    
    pool->block_size = actual_block_size;
    pool->block_count = initial_count;
    pool->free_count = initial_count;
    pool->blocks = NULL;
    
    /* 分配初始块 */
    for (cxc_size_t i = 0; i < initial_count; i++) {
        cxc_mempool_block_t* block = (cxc_mempool_block_t*)cxc_malloc(actual_block_size);
        if (block == NULL) {
            /* 清理已分配的块 */
            cxc_mempool_destroy(pool);
            return NULL;
        }
        block->next = (cxc_mempool_block_t*)pool->blocks;
        pool->blocks = block;
    }
    
    return pool;
}

cxc_ptr_t cxc_mempool_alloc(cxc_mempool_t* pool) {
    if (pool == NULL || pool->blocks == NULL) {
        return NULL;
    }
    
    cxc_mempool_block_t* block = (cxc_mempool_block_t*)pool->blocks;
    pool->blocks = block->next;
    pool->free_count--;
    
    return block;
}

void cxc_mempool_free(cxc_mempool_t* pool, cxc_ptr_t ptr) {
    if (pool == NULL || ptr == NULL) {
        return;
    }
    
    cxc_mempool_block_t* block = (cxc_mempool_block_t*)ptr;
    block->next = (cxc_mempool_block_t*)pool->blocks;
    pool->blocks = block;
    pool->free_count++;
}

void cxc_mempool_destroy(cxc_mempool_t* pool) {
    if (pool == NULL) {
        return;
    }
    
    /* 释放所有块 */
    cxc_mempool_block_t* block = (cxc_mempool_block_t*)pool->blocks;
    while (block != NULL) {
        cxc_mempool_block_t* next = block->next;
        cxc_free(block);
        block = next;
    }
    
    cxc_free(pool);
}

#ifdef __cplusplus
}
#endif
