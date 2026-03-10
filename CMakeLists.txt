cmake_minimum_required(VERSION 3.10)
project(CxC VERSION 0.1.0 LANGUAGES C CXX)

# 设置 C 标准
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 编译选项
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/src)

# 源文件
set(CORE_SOURCES
    src/core/memory.c
    src/core/error.c
)

set(OOP_SOURCES
    src/oop/class.c
    src/oop/inheritance.c
    src/oop/polymorphism.c
)

set(SP_SOURCES
    src/sp/raii.c
    src/sp/unique_ptr.c
    src/sp/shared_ptr.c
    src/sp/weak_ptr.c
)

set(CONTAINER_SOURCES
    src/container/vector.c
    src/container/list.c
    src/container/map.c
    src/container/set.c
    src/container/string.c
)

set(UTIL_SOURCES
    src/util/algorithm.c
    src/util/function.c
)

set(EXCEPTION_SOURCES
    src/exception/exception.c
)

set(CXC_SOURCES
    ${CORE_SOURCES}
    ${OOP_SOURCES}
    ${SP_SOURCES}
    ${CONTAINER_SOURCES}
    ${UTIL_SOURCES}
    ${EXCEPTION_SOURCES}
)

# 创建静态库
add_library(cxc_static STATIC ${CXC_SOURCES})
set_target_properties(cxc_static PROPERTIES
    OUTPUT_NAME cxc
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# 安装规则
install(TARGETS cxc_static
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY src/
    DESTINATION include/cxc
    FILES_MATCHING PATTERN "*.h"
)

# 示例程序
option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 测试支持
option(BUILD_TESTING "Build tests" ON)

if(BUILD_TESTING)
    enable_testing()
    
    # 查找 GoogleTest
    find_package(GTest QUIET)
    
    if(NOT GTest_FOUND)
        # 如果没有找到，使用 FetchContent 下载
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    
    # 测试可执行文件目录
    set(TEST_SOURCE_DIR ${CMAKE_SOURCE_DIR}/test)
    
    # 添加测试子目录
    add_subdirectory(test)
endif()
