# CxC 项目开发指南

## 项目概述

**CxC** (C like C++) 是一个 C 语言库，旨在为 C 语言提供类似 C++ 的高级特性，让 C 语言更强大、更易用。

- **作者**: fetch150zy
- **语言**: 标准 C (C17)
- **构建系统**: CMake
- **测试框架**: Google Test (gtest)
- **许可证**: MIT

## 项目结构

```
CxC/
├── src/                    # 源代码
│   ├── core/              # 核心基础设施
│   │   ├── types.h        # 类型系统基础
│   │   ├── memory.h       # 内存管理
│   │   ├── macros.h       # 宏工具集
│   │   ├── error.h        # 错误处理
│   │   └── core.h         # 核心模块汇总
│   ├── oop/               # 面向对象机制
│   │   ├── class.h        # 类基础
│   │   ├── inheritance.h  # 继承机制
│   │   ├── polymorphism.h # 多态机制
│   │   ├── encapsulation.h # 封装支持
│   │   └── oop.h          # OOP 模块汇总
│   ├── sp/                # 智能指针
│   │   ├── raii.h         # RAII 基础
│   │   ├── unique_ptr.h   # 独占所有权指针
│   │   ├── shared_ptr.h   # 共享所有权指针
│   │   ├── weak_ptr.h     # 弱引用指针
│   │   └── sp.h           # 智能指针模块汇总
│   ├── container/         # 标准容器
│   │   ├── vector.h       # 动态数组
│   │   ├── list.h         # 双向链表
│   │   ├── map.h          # 键值对映射
│   │   ├── set.h          # 集合
│   │   ├── string.h       # 动态字符串
│   │   └── container.h    # 容器模块汇总
│   ├── util/              # 工具函数
│   │   ├── algorithm.h    # 算法
│   │   ├── function.h     # 函数对象/回调
│   │   └── util.h         # 工具模块汇总
│   ├── exception/         # 异常处理
│   │   └── exception.h    # 异常机制
│   ├── lambda/            # Lambda 表达式
│   │   └── lambda.h       # Lambda 支持
│   ├── template/          # 模板支持
│   │   └── template.h     # 泛型模板宏
│   └── cxc.h              # 主头文件
├── test/                   # 测试代码
│   ├── test_core.cpp
│   ├── test_oop.cpp
│   ├── test_sp.cpp
│   ├── test_container.cpp
│   ├── test_util.cpp
│   ├── test_exception.c   # 异常测试 (C 语言)
│   ├── test_lambda.cpp
│   ├── test_template.cpp
│   └── CMakeLists.txt
├── include/               # 公开头文件目录
├── examples/              # 示例代码
├── build/                 # 构建输出
├── CMakeLists.txt        # 顶层 CMake 配置
├── build.sh              # 构建脚本
└── README.md             # 项目说明
```

## 已实现功能

### 1. 核心基础设施 (core)

- **类型系统**: 通用类型定义、`typeof` 封装、编译时类型检查
- **内存管理**: 统一分配/释放接口、内存对齐工具、内存池
- **宏工具**: 泛型宏 (`CXC_GENERIC`)、编译时断言、常用辅助宏
- **错误处理**: `CXC_Result` 结果类型、错误码定义

### 2. 面向对象机制 (oop)

- **类**: 结构体定义宏、构造/析构函数绑定、成员访问
- **继承**: 单继承支持、基类成员访问
- **多态**: 虚函数表 (vtable) 实现
- **封装**: 公有/私有成员分离

### 3. 智能指针 (sp)

- **RAII**: `CXC_DEFER` 自动清理宏
- **unique_ptr**: 独占所有权、移动语义、自定义删除器
- **shared_ptr**: 共享所有权、引用计数
- **weak_ptr**: 弱引用、打破循环引用

### 4. 标准容器 (container)

- **vector**: 动态数组
- **list**: 双向链表
- **map/set**: 键值对存储/集合
- **string**: 动态字符串

### 5. 工具函数 (util)

- **algorithm**: 排序、查找、遍历
- **function**: 闭包模拟、回调封装

### 6. 异常处理 (exception)

- 基于 `setjmp/longjmp` 实现
- 使用方式:
```c
CXC_TRY_BEGIN();
{
    CXC_THROW(CXC_ERROR_INVALID_ARGUMENT, "Error message");
}
CXC_TRY_END();

cxc_exception_t* exc = CXC_GET_LAST_EXCEPTION();
if (exc->code != CXC_ERROR_NONE) {
    // 处理异常
}
```

### 7. Lambda 表达式 (lambda)

- 函数指针类型定义
- Lambda 包装器宏

### 8. 模板支持 (template)

- 泛型交换、最大值、最小值
- 类型大小、对齐检查
- `CXC_IS_SAME_TYPE` 类型检查

## 构建与测试

### 构建项目

```bash
./build.sh
```

或手动构建:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON ..
make -j4
```

### 运行测试

```bash
./build.sh
```

构建脚本会自动处理测试超时（默认 60 秒），防止测试卡死。

或手动运行:

```bash
cd build
ctest --output-on-failure
```

分别运行:

```bash
./build/bin/cxc_tests          # C++ 测试
./build/bin/cxc_exception_tests # C 异常测试
```

**注意**: 如果测试卡死，build.sh 会使用 `timeout` 命令自动终止进程，并尝试杀死残留的测试进程。

## 技术要点

1. **泛型实现**: 使用 C11 `_Generic` 实现类型泛型
2. **内存安全**: 智能指针自动管理生命周期
3. **零成本抽象**: 编译时展开，最小运行时开销
4. **兼容性**: 遵循 C17 标准，跨平台支持

## 注意事项

### 异常测试

异常测试使用纯 C 编写（`test/test_exception.c`），因为 C++ 中的宏展开会导致变量作用域问题。

### 宏命名冲突

部分宏名可能与其他头文件冲突（如 `CXC_TRY` 在 `error.h` 中也有定义），使用时请注意区分。

### CI 配置

项目使用 Travis CI 进行持续集成 (`.travis.yml`)。

## 开发指南

### 添加新模块

1. 在 `src/` 下创建模块目录和头文件
2. 在对应的汇总头文件（如 `oop.h`, `sp.h`）中包含新模块
3. 在 `cxc.h` 中添加模块引用
4. 在 `test/CMakeLists.txt` 中添加测试文件
5. 运行 `./build.sh` 验证构建和测试

### 编写测试

- C++ 测试: 使用 Google Test 框架
- C 测试: 使用简单的 assert 风格的测试函数
- 异常相关测试必须使用 C（避免宏作用域问题）

## 联系方式

- GitHub: https://github.com/fetch150zy/CxC
- 问题反馈: https://github.com/fetch150zy/CxC/issues