#include <gtest/gtest.h>
extern "C" {
#include "exception/exception.h"
}

// 测试异常抛出和捕获
TEST(Exception, ThrowAndCatch) {
    int caught = 0;
    
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_INVALID_ARGUMENT, "Test exception");
        FAIL() << "Should not reach here";
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code != CXC_ERROR_NONE) {
        caught = 1;
        EXPECT_EQ(exc->code, CXC_ERROR_INVALID_ARGUMENT);
        EXPECT_STREQ(exc->message, "Test exception");
        EXPECT_NE(exc->file, nullptr);
        EXPECT_GT(exc->line, 0);
    }
    EXPECT_EQ(caught, 1);
}

// 测试异常错误码
TEST(Exception, ThrowErrorCode) {
    int caught = 0;
    
    CXC_TRY_BEGIN() {
        CXC_THROW_ERROR(CXC_ERROR_OUT_OF_MEMORY);
        FAIL() << "Should not reach here";
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code != CXC_ERROR_NONE) {
        caught = 1;
        EXPECT_EQ(exc->code, CXC_ERROR_OUT_OF_MEMORY);
        EXPECT_NE(exc->message, nullptr);
    }
    EXPECT_EQ(caught, 1);
}

// 测试无异常情况
TEST(Exception, NoException) {
    bool executed = false;
    
    CXC_TRY_BEGIN() {
        executed = true;
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    EXPECT_EQ(exc->code, CXC_ERROR_NONE);
    EXPECT_TRUE(executed);
}

// 测试嵌套异常 - 内层捕获后外层不再处理
TEST(Exception, NestedException) {
    bool inner_caught = false;
    
    // 内层 try 块
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_INVALID_STATE, "Inner exception");
    }
    CXC_TRY_END();
    
    // 检查内层异常
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code != CXC_ERROR_NONE) {
        inner_caught = true;
        EXPECT_EQ(exc->code, CXC_ERROR_INVALID_STATE);
    }
    
    EXPECT_TRUE(inner_caught);
    
    // 外层 try 块 - 在内层结束后再抛出
    bool outer_caught = false;
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_IO_ERROR, "Outer exception");
    }
    CXC_TRY_END();
    
    exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code != CXC_ERROR_NONE) {
        outer_caught = true;
        EXPECT_EQ(exc->code, CXC_ERROR_IO_ERROR);
    }
    
    EXPECT_TRUE(outer_caught);
}

// 测试条件抛出
TEST(Exception, ThrowIf) {
    int caught = 0;
    
    CXC_TRY_BEGIN() {
        CXC_THROW_IF(1, CXC_ERROR_INVALID_ARGUMENT, "Conditional throw");
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code != CXC_ERROR_NONE) {
        caught = 1;
        EXPECT_EQ(exc->code, CXC_ERROR_INVALID_ARGUMENT);
    }
    EXPECT_EQ(caught, 1);
}

// 测试条件抛出不触发
TEST(Exception, ThrowIfNotTriggered) {
    bool executed = false;
    
    CXC_TRY_BEGIN() {
        CXC_THROW_IF(0, CXC_ERROR_INVALID_ARGUMENT, "Should not throw");
        executed = true;
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    EXPECT_EQ(exc->code, CXC_ERROR_NONE);
    EXPECT_TRUE(executed);
}

// 测试捕获特定错误码
TEST(Exception, CatchSpecificCode) {
    bool caught = false;
    
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_NOT_FOUND, "Not found");
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code == CXC_ERROR_NOT_FOUND) {
        caught = true;
        EXPECT_EQ(exc->code, CXC_ERROR_NOT_FOUND);
    }
    EXPECT_TRUE(caught);
}

// 测试连续多次异常处理
TEST(Exception, MultipleExceptions) {
    int first_caught = 0;
    int second_caught = 0;
    
    // 第一次异常
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_INVALID_ARGUMENT, "First");
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code != CXC_ERROR_NONE) {
        first_caught = 1;
    }
    
    // 第二次异常
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_OUT_OF_MEMORY, "Second");
    }
    CXC_TRY_END();
    
    exc = CXC_GET_LAST_EXCEPTION();
    if (exc->code != CXC_ERROR_NONE) {
        second_caught = 1;
        EXPECT_EQ(exc->code, CXC_ERROR_OUT_OF_MEMORY);
    }
    
    EXPECT_EQ(first_caught, 1);
    EXPECT_EQ(second_caught, 1);
}

// 测试异常信息的完整性
TEST(Exception, ExceptionInfo) {
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_INVALID_ARGUMENT, "Test message");
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    EXPECT_EQ(exc->code, CXC_ERROR_INVALID_ARGUMENT);
    EXPECT_STREQ(exc->message, "Test message");
    EXPECT_NE(exc->file, nullptr);
    EXPECT_GT(exc->line, 0);
    EXPECT_NE(exc->function, nullptr);
}

// 测试错误码字符串转换
TEST(Exception, ErrorString) {
    CXC_TRY_BEGIN() {
        CXC_THROW_ERROR(CXC_ERROR_OUT_OF_MEMORY);
    }
    CXC_TRY_END();
    
    cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
    EXPECT_EQ(exc->code, CXC_ERROR_OUT_OF_MEMORY);
    EXPECT_NE(exc->message, nullptr);
}

// 测试清除异常
TEST(Exception, ClearException) {
    // 第一次抛出异常
    CXC_TRY_BEGIN() {
        CXC_THROW(CXC_ERROR_INVALID_ARGUMENT, "First");
    }
    CXC_TRY_END();
    
    EXPECT_NE(CXC_GET_LAST_EXCEPTION()->code, CXC_ERROR_NONE);
    
    // 第二次正常执行，异常应被清除
    CXC_TRY_BEGIN() {
        // 不抛出异常
    }
    CXC_TRY_END();
    
    EXPECT_EQ(CXC_GET_LAST_EXCEPTION()->code, CXC_ERROR_NONE);
}