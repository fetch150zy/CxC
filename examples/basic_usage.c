/**
 * @file basic_usage.c
 * @brief CxC 基本使用示例
 */

#include "../src/cxc.h"
#include <stdio.h>

int main(void) {
    printf("CxC Version: %d.%d.%d\n",
           CXC_VERSION_MAJOR,
           CXC_VERSION_MINOR,
           CXC_VERSION_PATCH);
    
    // 示例 1: 使用 vector
    printf("\n=== Vector Example ===\n");
    cxc_vector_t* vec = cxc_vector_create(sizeof(int), 10);
    if (vec != NULL) {
        for (int i = 0; i < 5; i++) {
            cxc_vector_push_back(vec, &i);
        }
        printf("Vector size: %zu\n", cxc_vector_size(vec));
        for (cxc_size_t i = 0; i < cxc_vector_size(vec); i++) {
            int* val = (int*)cxc_vector_at(vec, i);
            printf("  vec[%zu] = %d\n", i, *val);
        }
        cxc_vector_destroy(vec);
    }
    
    // 示例 2: 使用 string
    printf("\n=== String Example ===\n");
    cxc_string_t* str = cxc_string_from_cstr("Hello");
    if (str != NULL) {
        cxc_string_append_cstr(str, ", World!");
        printf("String: %s (length: %zu)\n", cxc_string_cstr(str), cxc_string_length(str));
        cxc_string_destroy(str);
    }
    
    // 示例 3: 使用 unique_ptr
    printf("\n=== Unique Pointer Example ===\n");
    cxc_unique_ptr_t up = CXC_MAKE_UNIQUE(int, 1, NULL);
    if (!cxc_unique_ptr_is_null(&up)) {
        int* ptr = CXC_UNIQUE_PTR_GET(&up, int);
        *ptr = 42;
        printf("Unique pointer value: %d\n", *ptr);
        cxc_unique_ptr_release(&up);
    }
    
    // 示例 4: 使用 map
    printf("\n=== Map Example ===\n");
    cxc_map_t* map = cxc_map_create(
        sizeof(int), sizeof(int),
        16,
        NULL, NULL,
        NULL, NULL
    );
    if (map != NULL) {
        int key1 = 1, value1 = 100;
        int key2 = 2, value2 = 200;
        
        cxc_map_insert(map, &key1, &value1);
        cxc_map_insert(map, &key2, &value2);
        
        int* found = (int*)cxc_map_find(map, &key1);
        if (found != NULL) {
            printf("Map[%d] = %d\n", key1, *found);
        }
        
        cxc_map_destroy(map);
    }
    
    printf("\nAll examples completed!\n");
    return 0;
}
