#!/bin/bash

set -e

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 测试超时时间（秒）
TEST_TIMEOUT=60

echo -e "${GREEN}Building CxC...${NC}"

# 创建构建目录
mkdir -p build
cd build

# 配置 CMake
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON ..

# 编译
echo -e "${YELLOW}Building...${NC}"
# 检测 CPU 核心数（兼容 macOS 和 Linux）
CORES=4
if command -v nproc > /dev/null 2>&1; then
    CORES=$(nproc 2>/dev/null || echo 4)
elif command -v sysctl > /dev/null 2>&1; then
    CORES=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
fi
make -j${CORES}

# 运行测试
if [ "$1" != "--no-test" ]; then
    echo -e "${YELLOW}Running tests (timeout: ${TEST_TIMEOUT}s)...${NC}"
    
    # 使用 timeout 命令运行测试，防止卡死
    # 先运行异常测试（C 语言）
    if command -v timeout > /dev/null 2>&1; then
        timeout ${TEST_TIMEOUT} ctest -R Exception_Tests --output-on-failure || {
            echo -e "${RED}Exception tests timed out or failed!${NC}"
            # 尝试杀死任何残留的测试进程
            pkill -f cxc_tests 2>/dev/null || true
            exit 1
        }
        
        # 运行其他测试（C++）
        timeout ${TEST_TIMEOUT} ctest -R "CxC_Tests" --output-on-failure || {
            echo -e "${RED}Tests timed out or failed!${NC}"
            pkill -f cxc_tests 2>/dev/null || true
            exit 1
        }
    else
        # 如果没有 timeout 命令，直接运行
        ctest --output-on-failure
    fi
    
    echo -e "${GREEN}All tests passed!${NC}"
fi

echo -e "${GREEN}Build complete!${NC}"
echo -e "Static library: ${GREEN}build/lib/libcxc.a${NC}"