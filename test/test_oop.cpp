#include <gtest/gtest.h>
extern "C" {
#include "oop/oop.h"
}

// 测试类定义
CXC_CLASS(Point,
    (int, x),
    (int, y)
);

static void Point_ctor(cxc_ptr_t self) {
    Point_ptr_t p = (Point_ptr_t)self;
    p->x = 0;
    p->y = 0;
}

TEST(OOPClass, CreateDestroy) {
    Point_ptr_t p = (Point_ptr_t)CXC_NEW(Point, Point_ctor);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->x, 0);
    EXPECT_EQ(p->y, 0);
    
    CXC_DELETE(p, NULL);
}

TEST(OOPClass, MemberAccess) {
    Point_t p;
    p.x = 10;
    p.y = 20;
    EXPECT_EQ(CXC_MEMBER(&p, x), 10);
    EXPECT_EQ(CXC_MEMBER(&p, y), 20);
}

// 测试继承
CXC_CLASS(Base,
    (int, value)
);

CXC_INHERIT(Derived, Base,
    (int, extra)
);

TEST(OOPInheritance, BaseMemberAccess) {
    Derived_t d;
    d.base.value = 42;
    d.extra = 100;
    
    EXPECT_EQ(CXC_BASE_MEMBER(&d, Base, value), 42);
    EXPECT_EQ(d.extra, 100);
}

TEST(OOPInheritance, Upcast) {
    Derived_t d;
    d.base.value = 42;
    d.extra = 100;
    Base_ptr_t base = CXC_UPCAST(&d, Base);
    
    ASSERT_NE(base, nullptr);
    EXPECT_EQ(base->value, 42);
}

TEST(OOPInheritance, Downcast) {
    Derived_t d;
    d.base.value = 42;
    d.extra = 100;
    Base_ptr_t base = CXC_UPCAST(&d, Base);
    
    Derived_ptr_t derived = CXC_DOWNCAST(base, Derived, Base);
    ASSERT_NE(derived, nullptr);
    EXPECT_EQ(derived->extra, 100);
}

// 测试多态
typedef struct TestObject {
    cxc_object_t base;
    int value;
} TestObject_t;

static cxc_vtable_t test_vtable = {
    .class_name = "TestObject",
    .vfunc_count = 1,
    .vfuncs = NULL
};

TEST(OOPPolymorphism, TypeName) {
    TestObject_t obj = {.base = {.vtable = &test_vtable}, .value = 42};
    const char* type_name = cxc_type_name(&obj);
    
    EXPECT_NE(type_name, nullptr);
    EXPECT_STREQ(type_name, "TestObject");
}

// 测试封装
CXC_CLASS(Encapsulated,
    (int, public_value)
);

CXC_ACCESSORS(Encapsulated, int, public_value);

TEST(OOPEncapsulation, Accessors) {
    Encapsulated_t obj;
    obj.public_value = 42;
    
    EXPECT_EQ(Encapsulated_get_public_value(&obj), 42);
    
    Encapsulated_set_public_value(&obj, 100);
    EXPECT_EQ(obj.public_value, 100);
}
