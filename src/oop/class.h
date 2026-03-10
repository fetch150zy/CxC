/**
 * @file class.h
 * @brief 类基础定义
 * 
 * 提供类结构体定义宏、构造/析构函数绑定和成员访问封装
 */

#ifndef CXC_OOP_CLASS_H
#define CXC_OOP_CLASS_H

#include "../core/core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 类定义宏
 * ============================================================================ */

/**
 * @brief 定义类结构体
 * @param class_name 类名
 * @param ... 成员变量列表，格式为 (type, name)
 * 
 * @example
 * CXC_CLASS(Point,
 *     (int, x),
 *     (int, y)
 * );
 */
#define CXC_CLASS(class_name, ...) \
    typedef struct class_name { \
        CXC_CLASS_MEMBERS(__VA_ARGS__) \
    } class_name##_t; \
    typedef class_name##_t* class_name##_ptr_t; \
    typedef const class_name##_t* class_name##_cptr_t

/**
 * @brief 展开类成员（内部宏）
 * @note 使用递归宏展开成员列表
 */
#define CXC_CLASS_MEMBERS(...) \
    CXC_CLASS_MEMBER_FOR_EACH(__VA_ARGS__)

/* 递归展开宏（最多支持 10 个成员） */
#define CXC_CLASS_MEMBER_FOR_EACH(...) \
    CXC_CLASS_MEMBER_GET_COUNT(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define CXC_CLASS_MEMBER_GET_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) \
    CXC_CLASS_MEMBER_EXPAND_BY_COUNT(N, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10)

#define CXC_CLASS_MEMBER_EXPAND_BY_COUNT(N, ...) \
    CXC_CONCAT(CXC_CLASS_MEMBER_EXPAND_, N)(__VA_ARGS__)

#define CXC_CLASS_MEMBER_EXPAND_0(...)
#define CXC_CLASS_MEMBER_EXPAND_1(m1, ...) CXC_CLASS_MEMBER_EXPAND m1;
#define CXC_CLASS_MEMBER_EXPAND_2(m1, m2, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2;
#define CXC_CLASS_MEMBER_EXPAND_3(m1, m2, m3, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3;
#define CXC_CLASS_MEMBER_EXPAND_4(m1, m2, m3, m4, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3; CXC_CLASS_MEMBER_EXPAND m4;
#define CXC_CLASS_MEMBER_EXPAND_5(m1, m2, m3, m4, m5, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3; CXC_CLASS_MEMBER_EXPAND m4; CXC_CLASS_MEMBER_EXPAND m5;
#define CXC_CLASS_MEMBER_EXPAND_6(m1, m2, m3, m4, m5, m6, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3; CXC_CLASS_MEMBER_EXPAND m4; CXC_CLASS_MEMBER_EXPAND m5; CXC_CLASS_MEMBER_EXPAND m6;
#define CXC_CLASS_MEMBER_EXPAND_7(m1, m2, m3, m4, m5, m6, m7, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3; CXC_CLASS_MEMBER_EXPAND m4; CXC_CLASS_MEMBER_EXPAND m5; CXC_CLASS_MEMBER_EXPAND m6; CXC_CLASS_MEMBER_EXPAND m7;
#define CXC_CLASS_MEMBER_EXPAND_8(m1, m2, m3, m4, m5, m6, m7, m8, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3; CXC_CLASS_MEMBER_EXPAND m4; CXC_CLASS_MEMBER_EXPAND m5; CXC_CLASS_MEMBER_EXPAND m6; CXC_CLASS_MEMBER_EXPAND m7; CXC_CLASS_MEMBER_EXPAND m8;
#define CXC_CLASS_MEMBER_EXPAND_9(m1, m2, m3, m4, m5, m6, m7, m8, m9, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3; CXC_CLASS_MEMBER_EXPAND m4; CXC_CLASS_MEMBER_EXPAND m5; CXC_CLASS_MEMBER_EXPAND m6; CXC_CLASS_MEMBER_EXPAND m7; CXC_CLASS_MEMBER_EXPAND m8; CXC_CLASS_MEMBER_EXPAND m9;
#define CXC_CLASS_MEMBER_EXPAND_10(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, ...) CXC_CLASS_MEMBER_EXPAND m1; CXC_CLASS_MEMBER_EXPAND m2; CXC_CLASS_MEMBER_EXPAND m3; CXC_CLASS_MEMBER_EXPAND m4; CXC_CLASS_MEMBER_EXPAND m5; CXC_CLASS_MEMBER_EXPAND m6; CXC_CLASS_MEMBER_EXPAND m7; CXC_CLASS_MEMBER_EXPAND m8; CXC_CLASS_MEMBER_EXPAND m9; CXC_CLASS_MEMBER_EXPAND m10;

#define CXC_CLASS_MEMBER_EXPAND(type, name) type name

/* ============================================================================
 * 构造函数和析构函数
 * ============================================================================ */

/**
 * @brief 构造函数函数指针类型
 */
typedef void (*cxc_ctor_func_t)(cxc_ptr_t self);

/**
 * @brief 析构函数函数指针类型
 */
typedef void (*cxc_dtor_func_t)(cxc_ptr_t self);

/**
 * @brief 类元信息结构
 */
typedef struct cxc_class_info {
    const char* name;              /**< 类名 */
    cxc_size_t size;               /**< 类大小 */
    cxc_ctor_func_t constructor;   /**< 构造函数 */
    cxc_dtor_func_t destructor;    /**< 析构函数 */
} cxc_class_info_t;

/**
 * @brief 声明构造函数
 * @param class_name 类名
 * @param func_name 函数名
 */
#define CXC_CTOR_DECLARE(class_name, func_name) \
    void func_name(class_name##_ptr_t self)

/**
 * @brief 声明析构函数
 * @param class_name 类名
 * @param func_name 函数名
 */
#define CXC_DTOR_DECLARE(class_name, func_name) \
    void func_name(class_name##_ptr_t self)

/**
 * @brief 定义构造函数
 * @param class_name 类名
 * @param func_name 函数名
 */
#define CXC_CTOR_DEFINE(class_name, func_name) \
    void func_name(class_name##_ptr_t self)

/**
 * @brief 定义析构函数
 * @param class_name 类名
 * @param func_name 函数名
 */
#define CXC_DTOR_DEFINE(class_name, func_name) \
    void func_name(class_name##_ptr_t self)

/* ============================================================================
 * 对象创建和销毁
 * ============================================================================ */

/**
 * @brief 创建对象
 * @param class_name 类名
 * @param ctor 构造函数（可为 NULL）
 * @return 对象指针，失败返回 NULL
 */
#define CXC_NEW(class_name, ctor) \
    cxc_new_impl(sizeof(class_name##_t), ctor)

/**
 * @brief 销毁对象
 * @param obj 对象指针
 * @param dtor 析构函数（可为 NULL）
 */
#define CXC_DELETE(obj, dtor) \
    cxc_delete_impl(obj, dtor)

/**
 * @brief 创建对象（带初始化）
 * @param class_name 类名
 * @param ctor 构造函数（可为 NULL）
 * @param init_expr 初始化表达式
 * @return 对象指针，失败返回 NULL
 */
#define CXC_NEW_INIT(class_name, ctor, init_expr) \
    ({ \
        class_name##_ptr_t _obj = CXC_NEW(class_name, ctor); \
        if (_obj != NULL) { \
            *_obj = (init_expr); \
        } \
        _obj; \
    })

/**
 * @brief 实现：创建对象
 */
cxc_ptr_t cxc_new_impl(cxc_size_t size, cxc_ctor_func_t ctor);

/**
 * @brief 实现：销毁对象
 */
void cxc_delete_impl(cxc_ptr_t obj, cxc_dtor_func_t dtor);

/* ============================================================================
 * 成员访问
 * ============================================================================ */

/**
 * @brief 访问成员变量
 * @param obj 对象指针
 * @param member 成员名
 */
#define CXC_MEMBER(obj, member) \
    ((obj)->member)

/**
 * @brief 访问成员变量（常量）
 * @param obj 对象指针
 * @param member 成员名
 */
#define CXC_MEMBER_C(obj, member) \
    ((obj)->member)

/**
 * @brief 调用成员函数
 * @param obj 对象指针
 * @param func 函数指针
 */
#define CXC_CALL(obj, func) \
    (func)(obj)

#ifdef __cplusplus
}
#endif

#endif /* CXC_OOP_CLASS_H */
