#include <gtest/gtest.h>
extern "C" {
#include "lambda/lambda.h"
#include "util/function.h"
}

// 测试 Lambda 定义（无参数）
CXC_DEFINE_LAMBDA_0(lambda_test_0, {
    static int value = 42;
    return &value;
});

TEST(Lambda, DefineLambda0) {
    int* result = (int*)lambda_test_0();
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 42);
}

// 测试 Lambda 定义（单参数）
CXC_DEFINE_LAMBDA_1(lambda_test_1, x, {
    int* val = (int*)x;
    *val *= 2;
    return x;
});

TEST(Lambda, DefineLambda1) {
    int value = 21;
    int* result = (int*)lambda_test_1(&value);
    EXPECT_EQ(value, 42);
    EXPECT_EQ(result, &value);
}

// 测试 Lambda 定义（双参数）
CXC_DEFINE_LAMBDA_2(lambda_test_2, a, b, {
    int* val_a = (int*)a;
    int* val_b = (int*)b;
    static int result;
    result = *val_a + *val_b;
    return &result;
});

TEST(Lambda, DefineLambda2) {
    int a = 10;
    int b = 20;
    int* result = (int*)lambda_test_2(&a, &b);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 30);
}

// 测试 Lambda 包装器
static cxc_ptr_t lambda_wrap_test(cxc_ptr_t x) {
    int* val = (int*)x;
    static int result;
    result = *val * 2;
    return &result;
}

TEST(Lambda, WrapLambda) {
    cxc_lambda_1_t wrapped = CXC_LAMBDA_WRAP_1(lambda_wrap_test);
    int value = 21;
    int* result = (int*)wrapped(&value);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 42);
}
