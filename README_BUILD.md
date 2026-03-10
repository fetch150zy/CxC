# CxC 构建指南

## 构建要求

- CMake 3.10 或更高版本
- C17 兼容的编译器（GCC 7+, Clang 5+, MSVC 2017+）
- 构建测试需要 GoogleTest（CMake 会自动下载）

## 快速开始

### Linux/macOS

```bash
./build.sh
```

### Windows

```cmd
build.bat
```

### 手动构建

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON ..
make -j$(nproc)
```

## 构建选项

- `CMAKE_BUILD_TYPE`: Debug 或 Release（默认：Release）
- `BUILD_TESTING`: 是否构建测试（默认：ON）

## 输出

构建完成后，静态库位于：
- Linux/macOS: `build/lib/libcxc.a`
- Windows: `build/lib/Release/cxc.lib`

## 运行测试

```bash
cd build
ctest --output-on-failure
```

或者直接运行测试可执行文件：

```bash
./build/bin/cxc_tests
```

## 使用库

在你的项目中：

```cmake
# CMakeLists.txt
add_subdirectory(path/to/cxc)
target_link_libraries(your_target PRIVATE cxc_static)
target_include_directories(your_target PRIVATE path/to/cxc/src)
```

```c
#include "cxc.h"

int main() {
    // 使用 CxC 功能
    return 0;
}
```
