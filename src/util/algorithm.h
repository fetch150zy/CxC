/**
 * @file algorithm.h
 * @brief 算法工具
 * 
 * 提供排序、查找、遍历等常用算法
 */

#ifndef CXC_UTIL_ALGORITHM_H
#define CXC_UTIL_ALGORITHM_H

#include "../core/core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 比较函数类型
 * ============================================================================ */

/**
 * @brief 比较函数指针类型
 */
typedef int (*cxc_compare_func_t)(cxc_cptr_t a, cxc_cptr_t b);

/**
 * @brief 谓词函数指针类型
 */
typedef cxc_bool_t (*cxc_predicate_func_t)(cxc_cptr_t elem);

/**
 * @brief 变换函数指针类型
 */
typedef void (*cxc_transform_func_t)(cxc_ptr_t elem);

/* ============================================================================
 * 排序算法
 * ============================================================================ */

/**
 * @brief 快速排序
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param compare 比较函数
 */
void cxc_quicksort(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_compare_func_t compare
);

/**
 * @brief 归并排序
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param compare 比较函数
 */
void cxc_mergesort(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_compare_func_t compare
);

/**
 * @brief 堆排序
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param compare 比较函数
 */
void cxc_heapsort(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_compare_func_t compare
);

/* ============================================================================
 * 查找算法
 * ============================================================================ */

/**
 * @brief 线性查找
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param key 要查找的键
 * @param compare 比较函数
 * @return 找到返回索引，未找到返回 (cxc_size_t)-1
 */
cxc_size_t cxc_linear_search(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_cptr_t key,
    cxc_compare_func_t compare
);

/**
 * @brief 二分查找（要求数组已排序）
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param key 要查找的键
 * @param compare 比较函数
 * @return 找到返回索引，未找到返回 (cxc_size_t)-1
 */
cxc_size_t cxc_binary_search(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_cptr_t key,
    cxc_compare_func_t compare
);

/* ============================================================================
 * 遍历算法
 * ============================================================================ */

/**
 * @brief 遍历数组并应用函数
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param func 要应用的函数
 */
void cxc_for_each(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_transform_func_t func
);

/**
 * @brief 查找满足条件的元素
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param predicate 谓词函数
 * @return 找到返回元素指针，未找到返回 NULL
 */
cxc_ptr_t cxc_find_if(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_predicate_func_t predicate
);

/**
 * @brief 计算满足条件的元素数量
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param predicate 谓词函数
 * @return 满足条件的元素数量
 */
cxc_size_t cxc_count_if(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_predicate_func_t predicate
);

/* ============================================================================
 * 变换算法
 * ============================================================================ */

/**
 * @brief 复制数组
 * @param dest 目标数组
 * @param src 源数组
 * @param count 元素数量
 * @param elem_size 元素大小
 */
void cxc_copy(
    cxc_ptr_t dest,
    cxc_cptr_t src,
    cxc_size_t count,
    cxc_size_t elem_size
);

/**
 * @brief 填充数组
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param value 值指针
 */
void cxc_fill(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_cptr_t value
);

/**
 * @brief 变换数组
 * @param dest 目标数组
 * @param src 源数组
 * @param count 元素数量
 * @param elem_size 元素大小
 * @param transform 变换函数
 */
void cxc_transform(
    cxc_ptr_t dest,
    cxc_cptr_t src,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_transform_func_t transform
);

/* ============================================================================
 * 工具函数
 * ============================================================================ */

/**
 * @brief 交换两个元素
 * @param a 第一个元素指针
 * @param b 第二个元素指针
 * @param elem_size 元素大小
 */
void cxc_swap_elem(cxc_ptr_t a, cxc_ptr_t b, cxc_size_t elem_size);

/**
 * @brief 反转数组
 * @param base 数组基地址
 * @param count 元素数量
 * @param elem_size 元素大小
 */
void cxc_reverse(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size
);

#ifdef __cplusplus
}
#endif

#endif /* CXC_UTIL_ALGORITHM_H */
