#include <gtest/gtest.h>
extern "C" {
#include "util/util.h"
}

// 比较函数
static int compare_int(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

static bool is_even(const void* elem) {
    return *(const int*)elem % 2 == 0;
}

static void double_value(void* elem) {
    *(int*)elem *= 2;
}

// 测试排序算法
TEST(UtilAlgorithm, QuickSort) {
    int arr[] = {5, 2, 8, 1, 9, 3};
    int expected[] = {1, 2, 3, 5, 8, 9};
    
    cxc_quicksort(arr, 6, sizeof(int), compare_int);
    
    for (int i = 0; i < 6; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST(UtilAlgorithm, MergeSort) {
    int arr[] = {5, 2, 8, 1, 9, 3};
    int expected[] = {1, 2, 3, 5, 8, 9};
    
    cxc_mergesort(arr, 6, sizeof(int), compare_int);
    
    for (int i = 0; i < 6; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST(UtilAlgorithm, HeapSort) {
    int arr[] = {5, 2, 8, 1, 9, 3};
    int expected[] = {1, 2, 3, 5, 8, 9};
    
    cxc_heapsort(arr, 6, sizeof(int), compare_int);
    
    for (int i = 0; i < 6; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

// 测试查找算法
TEST(UtilAlgorithm, LinearSearch) {
    int arr[] = {5, 2, 8, 1, 9, 3};
    int key = 8;
    
    cxc_size_t index = cxc_linear_search(arr, 6, sizeof(int), &key, compare_int);
    EXPECT_EQ(index, 2);
    
    int not_found = 99;
    index = cxc_linear_search(arr, 6, sizeof(int), &not_found, compare_int);
    EXPECT_EQ(index, (cxc_size_t)-1);
}

TEST(UtilAlgorithm, BinarySearch) {
    int arr[] = {1, 2, 3, 5, 8, 9};
    int key = 5;
    
    cxc_size_t index = cxc_binary_search(arr, 6, sizeof(int), &key, compare_int);
    EXPECT_EQ(index, 3);
    
    int not_found = 99;
    index = cxc_binary_search(arr, 6, sizeof(int), &not_found, compare_int);
    EXPECT_EQ(index, (cxc_size_t)-1);
}

// 测试遍历算法
TEST(UtilAlgorithm, ForEach) {
    int arr[] = {1, 2, 3, 4, 5};
    int expected[] = {2, 4, 6, 8, 10};
    
    cxc_for_each(arr, 5, sizeof(int), double_value);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST(UtilAlgorithm, FindIf) {
    int arr[] = {1, 2, 3, 4, 5};
    
    int* found = (int*)cxc_find_if(arr, 5, sizeof(int), is_even);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(*found, 2);
}

TEST(UtilAlgorithm, CountIf) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    
    cxc_size_t count = cxc_count_if(arr, 6, sizeof(int), is_even);
    EXPECT_EQ(count, 3);
}

// 测试变换算法
TEST(UtilAlgorithm, Copy) {
    int src[] = {1, 2, 3, 4, 5};
    int dest[5];
    
    cxc_copy(dest, src, 5, sizeof(int));
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(dest[i], src[i]);
    }
}

TEST(UtilAlgorithm, Fill) {
    int arr[5];
    int value = 42;
    
    cxc_fill(arr, 5, sizeof(int), &value);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(arr[i], 42);
    }
}

TEST(UtilAlgorithm, Transform) {
    int src[] = {1, 2, 3, 4, 5};
    int dest[5];
    int expected[] = {2, 4, 6, 8, 10};
    
    cxc_transform(dest, src, 5, sizeof(int), double_value);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(dest[i], expected[i]);
    }
}

TEST(UtilAlgorithm, Reverse) {
    int arr[] = {1, 2, 3, 4, 5};
    int expected[] = {5, 4, 3, 2, 1};
    
    cxc_reverse(arr, 5, sizeof(int));
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

// 测试函数对象
static void test_closure_func(void* data, void* arg) {
    int* counter = (int*)data;
    int* value = (int*)arg;
    *counter += *value;
}

TEST(UtilFunction, Closure) {
    int counter = 0;
    int value = 10;
    
    cxc_closure_t* closure = cxc_closure_create(&counter, test_closure_func, NULL);
    ASSERT_NE(closure, nullptr);
    
    cxc_closure_call(closure, &value);
    EXPECT_EQ(counter, 10);
    
    cxc_closure_call(closure, &value);
    EXPECT_EQ(counter, 20);
    
    cxc_closure_destroy(closure);
}

static void* test_function_invoke(void* data, void* arg) {
    int* base = (int*)data;
    int* multiplier = (int*)arg;
    static int result;
    result = *base * *multiplier;
    return &result;
}

TEST(UtilFunction, FunctionObject) {
    int base = 5;
    int multiplier = 3;
    
    cxc_function_t* func = cxc_function_create(&base, test_function_invoke, NULL);
    ASSERT_NE(func, nullptr);
    
    int* result = (int*)cxc_function_invoke(func, &multiplier);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 15);
    
    cxc_function_destroy(func);
}

static void callback_func(void* context, void* arg) {
    int* ctx = (int*)context;
    int* val = (int*)arg;
    *ctx += *val;
}

TEST(UtilFunction, Callback) {
    int context_value = 100;
    int arg_value = 5;
    
    cxc_callback_t cb = cxc_callback_create(&context_value, callback_func);
    cxc_callback_invoke(&cb, &arg_value);
    
    EXPECT_EQ(context_value, 105);
}
