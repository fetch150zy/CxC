#include <gtest/gtest.h>
extern "C" {
#include "template/template.h"
#include "core/macros.h"
}

// 测试泛型交换
TEST(Template, SwapTemplate) {
    int a = 10, b = 20;
    CXC_SWAP_TEMPLATE(a, b);
    EXPECT_EQ(a, 20);
    EXPECT_EQ(b, 10);
    
    float x = 1.5f, y = 2.5f;
    CXC_SWAP_TEMPLATE(x, y);
    EXPECT_FLOAT_EQ(x, 2.5f);
    EXPECT_FLOAT_EQ(y, 1.5f);
}

// 测试泛型最大值
TEST(Template, MaxTemplate) {
    EXPECT_EQ(CXC_MAX_TEMPLATE(10, 20), 20);
    EXPECT_FLOAT_EQ(CXC_MAX_TEMPLATE(1.5f, 2.5f), 2.5f);
}

// 测试泛型最小值
TEST(Template, MinTemplate) {
    EXPECT_EQ(CXC_MIN_TEMPLATE(10, 20), 10);
    EXPECT_FLOAT_EQ(CXC_MIN_TEMPLATE(1.5f, 2.5f), 1.5f);
}

// 测试泛型比较
TEST(Template, CompareTemplate) {
    EXPECT_EQ(CXC_COMPARE_TEMPLATE(10, 20), -1);
    EXPECT_EQ(CXC_COMPARE_TEMPLATE(20, 10), 1);
    EXPECT_EQ(CXC_COMPARE_TEMPLATE(10, 10), 0);
}

// 测试类型大小
TEST(Template, TypeSize) {
    EXPECT_EQ(CXC_TYPE_SIZE(int), sizeof(int));
    EXPECT_EQ(CXC_TYPE_SIZE(float), sizeof(float));
    EXPECT_EQ(CXC_TYPE_SIZE(double), sizeof(double));
}

// 测试类型对齐
TEST(Template, TypeAlign) {
    EXPECT_EQ(CXC_TYPE_ALIGN(int), _Alignof(int));
    EXPECT_EQ(CXC_TYPE_ALIGN(float), _Alignof(float));
}

// 测试类型检查
TEST(Template, IsSameType) {
    int a = 0;
    float b = 0.0f;
    
    // 使用 CXC_IS_TYPE 代替 CXC_IS_SAME_TYPE（在 C++ 中更兼容）
    EXPECT_TRUE(CXC_IS_TYPE(a, int));
    EXPECT_TRUE(CXC_IS_TYPE(b, float));
    EXPECT_FALSE(CXC_IS_TYPE(a, float));
    EXPECT_FALSE(CXC_IS_TYPE(b, int));
}

// 测试泛型函数调用
static int int_func(int x) {
    return x * 2;
}

static float float_func(float x) {
    return x * 2.0f;
}

TEST(Template, TemplateCall) {
    int i = 10;
    float f = 1.5f;
    
    int result_i = CXC_TEMPLATE_CALL(i, int_func,
        int: int_func,
        float: float_func
    );
    
    EXPECT_EQ(result_i, 20);
    
    float result_f = CXC_TEMPLATE_CALL(f, float_func,
        int: int_func,
        float: float_func
    );
    
    EXPECT_FLOAT_EQ(result_f, 3.0f);
}
