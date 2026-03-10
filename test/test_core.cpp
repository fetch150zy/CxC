#include <gtest/gtest.h>
extern "C" {
#include "core/core.h"
}

// 测试类型系统
TEST(CoreTypes, BasicTypes) {
    EXPECT_EQ(sizeof(cxc_bool_t), sizeof(bool));
    EXPECT_EQ(sizeof(cxc_size_t), sizeof(size_t));
}

TEST(CoreTypes, TypeCheck) {
    int x = 42;
    float y = 3.14f;
    
    EXPECT_TRUE(CXC_IS_TYPE(x, int));
    EXPECT_TRUE(CXC_IS_TYPE(y, float));
    EXPECT_FALSE(CXC_IS_TYPE(x, float));
}

// 测试内存管理
TEST(CoreMemory, MallocFree) {
    void* ptr = cxc_malloc(100);
    ASSERT_NE(ptr, nullptr);
    cxc_free(ptr);
}

TEST(CoreMemory, Calloc) {
    int* arr = (int*)cxc_calloc(sizeof(int) * 10);
    ASSERT_NE(arr, nullptr);
    
    // 检查是否清零
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(arr[i], 0);
    }
    
    cxc_free(arr);
}

TEST(CoreMemory, Realloc) {
    int* arr = (int*)cxc_malloc(sizeof(int) * 5);
    ASSERT_NE(arr, nullptr);
    
    arr = (int*)cxc_realloc(arr, sizeof(int) * 10);
    ASSERT_NE(arr, nullptr);
    
    cxc_free(arr);
}

TEST(CoreMemory, AlignedMalloc) {
    void* ptr = cxc_aligned_malloc(100, 16);
    ASSERT_NE(ptr, nullptr);
    
    uintptr_t addr = (uintptr_t)ptr;
    EXPECT_EQ(addr % 16, 0);
    
    cxc_aligned_free(ptr);
}

TEST(CoreMemory, MemoryPool) {
    cxc_mempool_t* pool = cxc_mempool_create(sizeof(int), 10);
    ASSERT_NE(pool, nullptr);
    
    int* ptr1 = (int*)cxc_mempool_alloc(pool);
    ASSERT_NE(ptr1, nullptr);
    *ptr1 = 42;
    
    int* ptr2 = (int*)cxc_mempool_alloc(pool);
    ASSERT_NE(ptr2, nullptr);
    *ptr2 = 100;
    
    EXPECT_EQ(*ptr1, 42);
    EXPECT_EQ(*ptr2, 100);
    
    cxc_mempool_free(pool, ptr1);
    cxc_mempool_free(pool, ptr2);
    cxc_mempool_destroy(pool);
}

// 测试宏工具
TEST(CoreMacros, ArraySize) {
    int arr[10];
    EXPECT_EQ(CXC_ARRAY_SIZE(arr), 10);
}

TEST(CoreMacros, MinMax) {
    EXPECT_EQ(CXC_MIN(5, 10), 5);
    EXPECT_EQ(CXC_MAX(5, 10), 10);
}

TEST(CoreMacros, StaticAssert) {
    CXC_STATIC_ASSERT(1 == 1, "Should compile");
    CXC_STATIC_ASSERT_S(sizeof(int) == 4 || sizeof(int) == 8);
}

// 测试错误处理
TEST(CoreError, ResultOk) {
    int value = 42;
    cxc_result_t result = cxc_result_ok(&value);
    
    EXPECT_TRUE(cxc_result_is_ok(&result));
    EXPECT_FALSE(cxc_result_is_err(&result));
    EXPECT_EQ(cxc_result_error(&result), CXC_ERROR_NONE);
    EXPECT_EQ(*(int*)cxc_result_unwrap(&result), 42);
}

TEST(CoreError, ResultErr) {
    cxc_result_t result = cxc_result_err(CXC_ERROR_INVALID_ARGUMENT);
    
    EXPECT_FALSE(cxc_result_is_ok(&result));
    EXPECT_TRUE(cxc_result_is_err(&result));
    EXPECT_EQ(cxc_result_error(&result), CXC_ERROR_INVALID_ARGUMENT);
}

TEST(CoreError, ErrorString) {
    EXPECT_NE(cxc_error_string(CXC_ERROR_NONE), nullptr);
    EXPECT_NE(cxc_error_string(CXC_ERROR_INVALID_ARGUMENT), nullptr);
    EXPECT_NE(cxc_error_string(CXC_ERROR_OUT_OF_MEMORY), nullptr);
}
