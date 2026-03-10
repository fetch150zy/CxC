# 测试配置

# 启用 C++ 语言支持
enable_language(CXX)

# C++ 测试源文件
set(CXX_TEST_SOURCES
    test_core.cpp
    test_oop.cpp
    test_sp.cpp
    test_container.cpp
    test_util.cpp
    test_lambda.cpp
    test_template.cpp
    test_exception.cpp
)

# 创建 C++ 测试可执行文件
add_executable(cxc_tests ${CXX_TEST_SOURCES})

# 设置 C++ 标准
set_target_properties(cxc_tests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)

# 链接库
target_link_libraries(cxc_tests
    PRIVATE
    cxc_static
    GTest::gtest_main
)

# 包含目录
target_include_directories(cxc_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# 添加测试
add_test(NAME CxC_Tests COMMAND cxc_tests)

# 设置测试属性
set_tests_properties(CxC_Tests PROPERTIES
    TIMEOUT 300
)