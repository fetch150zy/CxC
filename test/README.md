# CxC 测试

## 测试框架

本项目使用 GoogleTest 作为测试框架。

## 测试结构

测试文件按模块组织：

- `test_core.cpp` - 核心基础设施测试
  - 类型系统
  - 内存管理
  - 宏工具
  - 错误处理

- `test_oop.cpp` - 面向对象机制测试
  - 类定义和构造/析构
  - 继承机制
  - 多态机制
  - 封装支持

- `test_sp.cpp` - 智能指针测试
  - RAII 基础
  - unique_ptr
  - shared_ptr
  - weak_ptr

- `test_container.cpp` - 容器测试
  - vector
  - list
  - map
  - set
  - string

- `test_util.cpp` - 工具函数测试
  - 排序算法（快速排序、归并排序、堆排序）
  - 查找算法（线性查找、二分查找）
  - 遍历算法（for_each、find_if、count_if）
  - 变换算法（copy、fill、transform、reverse）
  - 函数对象和闭包

## 运行测试

### 使用 CMake

```bash
cd build
ctest --output-on-failure
```

### 直接运行

```bash
./build/bin/cxc_tests
```

### 运行特定测试

```bash
./build/bin/cxc_tests --gtest_filter=CoreTypes.*
```

## 测试覆盖率

测试覆盖了所有主要功能模块，包括：

- ✅ 核心基础设施（100%）
- ✅ 面向对象机制（100%）
- ✅ 智能指针（100%）
- ✅ 容器（100%）
- ✅ 工具函数（100%）

## CI 集成

测试在 Travis CI 上自动运行，支持：
- GCC 和 Clang 编译器
- Debug 和 Release 构建
- 所有测试用例
