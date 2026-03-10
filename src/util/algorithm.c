/**
 * @file algorithm.c
 * @brief 算法工具实现
 */

#include "algorithm.h"
#include "../core/memory.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static void cxc_quicksort_partition(
    cxc_ptr_t base,
    cxc_size_t left,
    cxc_size_t right,
    cxc_size_t elem_size,
    cxc_compare_func_t compare,
    char* temp
) {
    if (left >= right) {
        return;
    }
    
    cxc_size_t pivot = right;
    cxc_size_t i = left;
    
    for (cxc_size_t j = left; j < right; j++) {
        char* elem_j = (char*)base + j * elem_size;
        char* elem_pivot = (char*)base + pivot * elem_size;
        
        if (compare(elem_j, elem_pivot) < 0) {
            char* elem_i = (char*)base + i * elem_size;
            cxc_swap_elem(elem_i, elem_j, elem_size);
            i++;
        }
    }
    
    char* elem_i = (char*)base + i * elem_size;
    char* elem_pivot = (char*)base + pivot * elem_size;
    cxc_swap_elem(elem_i, elem_pivot, elem_size);
    
    if (i > 0) {
        cxc_quicksort_partition(base, left, i - 1, elem_size, compare, temp);
    }
    cxc_quicksort_partition(base, i + 1, right, elem_size, compare, temp);
}

void cxc_quicksort(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_compare_func_t compare
) {
    if (base == NULL || count == 0 || elem_size == 0 || compare == NULL) {
        return;
    }
    
    char* temp = (char*)cxc_malloc(elem_size);
    if (temp == NULL) {
        return;
    }
    
    cxc_quicksort_partition(base, 0, count - 1, elem_size, compare, temp);
    
    cxc_free(temp);
}

static void cxc_mergesort_merge(
    cxc_ptr_t base,
    cxc_size_t left,
    cxc_size_t mid,
    cxc_size_t right,
    cxc_size_t elem_size,
    cxc_compare_func_t compare,
    char* temp
) {
    cxc_size_t i = left;
    cxc_size_t j = mid + 1;
    cxc_size_t k = 0;
    
    while (i <= mid && j <= right) {
        char* elem_i = (char*)base + i * elem_size;
        char* elem_j = (char*)base + j * elem_size;
        
        if (compare(elem_i, elem_j) <= 0) {
            cxc_memcpy(temp + k * elem_size, elem_i, elem_size);
            i++;
        } else {
            cxc_memcpy(temp + k * elem_size, elem_j, elem_size);
            j++;
        }
        k++;
    }
    
    while (i <= mid) {
        cxc_memcpy(temp + k * elem_size, (char*)base + i * elem_size, elem_size);
        i++;
        k++;
    }
    
    while (j <= right) {
        cxc_memcpy(temp + k * elem_size, (char*)base + j * elem_size, elem_size);
        j++;
        k++;
    }
    
    for (cxc_size_t idx = 0; idx < k; idx++) {
        cxc_memcpy((char*)base + (left + idx) * elem_size, temp + idx * elem_size, elem_size);
    }
}

static void cxc_mergesort_recursive(
    cxc_ptr_t base,
    cxc_size_t left,
    cxc_size_t right,
    cxc_size_t elem_size,
    cxc_compare_func_t compare,
    char* temp
) {
    if (left < right) {
        cxc_size_t mid = left + (right - left) / 2;
        cxc_mergesort_recursive(base, left, mid, elem_size, compare, temp);
        cxc_mergesort_recursive(base, mid + 1, right, elem_size, compare, temp);
        cxc_mergesort_merge(base, left, mid, right, elem_size, compare, temp);
    }
}

void cxc_mergesort(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_compare_func_t compare
) {
    if (base == NULL || count == 0 || elem_size == 0 || compare == NULL) {
        return;
    }
    
    char* temp = (char*)cxc_malloc(count * elem_size);
    if (temp == NULL) {
        return;
    }
    
    cxc_mergesort_recursive(base, 0, count - 1, elem_size, compare, temp);
    
    cxc_free(temp);
}

static void cxc_heapify(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_compare_func_t compare,
    cxc_size_t root
) {
    cxc_size_t largest = root;
    cxc_size_t left = 2 * root + 1;
    cxc_size_t right = 2 * root + 2;
    
    char* elem_largest = (char*)base + largest * elem_size;
    
    if (left < count) {
        char* elem_left = (char*)base + left * elem_size;
        if (compare(elem_left, elem_largest) > 0) {
            largest = left;
            elem_largest = elem_left;
        }
    }
    
    if (right < count) {
        char* elem_right = (char*)base + right * elem_size;
        if (compare(elem_right, elem_largest) > 0) {
            largest = right;
        }
    }
    
    if (largest != root) {
        char* elem_root = (char*)base + root * elem_size;
        char* elem_largest_new = (char*)base + largest * elem_size;
        cxc_swap_elem(elem_root, elem_largest_new, elem_size);
        cxc_heapify(base, count, elem_size, compare, largest);
    }
}

void cxc_heapsort(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_compare_func_t compare
) {
    if (base == NULL || count == 0 || elem_size == 0 || compare == NULL) {
        return;
    }
    
    /* 构建堆 */
    for (cxc_int_t i = (cxc_int_t)(count / 2) - 1; i >= 0; i--) {
        cxc_heapify(base, count, elem_size, compare, (cxc_size_t)i);
    }
    
    /* 提取元素 */
    for (cxc_size_t i = count - 1; i > 0; i--) {
        cxc_swap_elem(base, (char*)base + i * elem_size, elem_size);
        cxc_heapify(base, i, elem_size, compare, 0);
    }
}

cxc_size_t cxc_linear_search(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_cptr_t key,
    cxc_compare_func_t compare
) {
    if (base == NULL || key == NULL || compare == NULL) {
        return (cxc_size_t)-1;
    }
    
    for (cxc_size_t i = 0; i < count; i++) {
        const char* elem = (const char*)base + i * elem_size;
        if (compare(elem, key) == 0) {
            return i;
        }
    }
    
    return (cxc_size_t)-1;
}

cxc_size_t cxc_binary_search(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_cptr_t key,
    cxc_compare_func_t compare
) {
    if (base == NULL || key == NULL || compare == NULL) {
        return (cxc_size_t)-1;
    }
    
    cxc_size_t left = 0;
    cxc_size_t right = count;
    
    while (left < right) {
        cxc_size_t mid = left + (right - left) / 2;
        const char* elem_mid = (const char*)base + mid * elem_size;
        int cmp = compare(elem_mid, key);
        
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return (cxc_size_t)-1;
}

void cxc_for_each(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_transform_func_t func
) {
    if (base == NULL || func == NULL) {
        return;
    }
    
    for (cxc_size_t i = 0; i < count; i++) {
        char* elem = (char*)base + i * elem_size;
        func(elem);
    }
}

cxc_ptr_t cxc_find_if(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_predicate_func_t predicate
) {
    if (base == NULL || predicate == NULL) {
        return NULL;
    }
    
    for (cxc_size_t i = 0; i < count; i++) {
        const char* elem = (const char*)base + i * elem_size;
        if (predicate(elem)) {
            return (cxc_ptr_t)elem;
        }
    }
    
    return NULL;
}

cxc_size_t cxc_count_if(
    cxc_cptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_predicate_func_t predicate
) {
    if (base == NULL || predicate == NULL) {
        return 0;
    }
    
    cxc_size_t count_result = 0;
    for (cxc_size_t i = 0; i < count; i++) {
        const char* elem = (const char*)base + i * elem_size;
        if (predicate(elem)) {
            count_result++;
        }
    }
    
    return count_result;
}

void cxc_copy(
    cxc_ptr_t dest,
    cxc_cptr_t src,
    cxc_size_t count,
    cxc_size_t elem_size
) {
    if (dest == NULL || src == NULL) {
        return;
    }
    
    cxc_memcpy(dest, src, count * elem_size);
}

void cxc_fill(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_cptr_t value
) {
    if (base == NULL || value == NULL) {
        return;
    }
    
    for (cxc_size_t i = 0; i < count; i++) {
        char* elem = (char*)base + i * elem_size;
        cxc_memcpy(elem, value, elem_size);
    }
}

void cxc_transform(
    cxc_ptr_t dest,
    cxc_cptr_t src,
    cxc_size_t count,
    cxc_size_t elem_size,
    cxc_transform_func_t transform
) {
    if (dest == NULL || src == NULL || transform == NULL) {
        return;
    }
    
    for (cxc_size_t i = 0; i < count; i++) {
        const char* src_elem = (const char*)src + i * elem_size;
        char* dest_elem = (char*)dest + i * elem_size;
        cxc_memcpy(dest_elem, src_elem, elem_size);
        transform(dest_elem);
    }
}

void cxc_swap_elem(cxc_ptr_t a, cxc_ptr_t b, cxc_size_t elem_size) {
    if (a == NULL || b == NULL || elem_size == 0) {
        return;
    }
    
    char* temp = (char*)cxc_malloc(elem_size);
    if (temp == NULL) {
        return;
    }
    
    cxc_memcpy(temp, a, elem_size);
    cxc_memcpy(a, b, elem_size);
    cxc_memcpy(b, temp, elem_size);
    
    cxc_free(temp);
}

void cxc_reverse(
    cxc_ptr_t base,
    cxc_size_t count,
    cxc_size_t elem_size
) {
    if (base == NULL || count == 0) {
        return;
    }
    
    cxc_size_t left = 0;
    cxc_size_t right = count - 1;
    
    while (left < right) {
        char* elem_left = (char*)base + left * elem_size;
        char* elem_right = (char*)base + right * elem_size;
        cxc_swap_elem(elem_left, elem_right, elem_size);
        left++;
        right--;
    }
}

#ifdef __cplusplus
}
#endif
