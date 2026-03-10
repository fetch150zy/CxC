#include <gtest/gtest.h>
extern "C" {
#include "sp/sp.h"
}

// 测试 RAII
TEST(SmartPointerRAII, ScopeGuard) {
    int* ptr = (int*)cxc_malloc(sizeof(int));
    ASSERT_NE(ptr, nullptr);
    
    cxc_scope_guard_t guard = cxc_scope_guard_create(ptr, (cxc_cleanup_func_t)cxc_free);
    ASSERT_TRUE(guard.active);
    
    cxc_scope_guard_release(&guard);
    EXPECT_FALSE(guard.active);
}

// 测试 unique_ptr
TEST(SmartPointerUniquePtr, Create) {
    int* raw_ptr = (int*)cxc_malloc(sizeof(int));
    *raw_ptr = 42;
    
    cxc_unique_ptr_t up = cxc_unique_ptr_create(raw_ptr, NULL);
    EXPECT_FALSE(cxc_unique_ptr_is_null(&up));
    EXPECT_EQ(*(int*)cxc_unique_ptr_get(&up), 42);
    
    cxc_unique_ptr_release(&up);
}

TEST(SmartPointerUniquePtr, MakeUnique) {
    cxc_unique_ptr_t up = CXC_MAKE_UNIQUE(int, 1, NULL);
    ASSERT_FALSE(cxc_unique_ptr_is_null(&up));
    
    int* ptr = CXC_UNIQUE_PTR_GET(&up, int);
    *ptr = 100;
    EXPECT_EQ(*ptr, 100);
    
    cxc_unique_ptr_release(&up);
}

TEST(SmartPointerUniquePtr, Move) {
    int* raw_ptr = (int*)cxc_malloc(sizeof(int));
    *raw_ptr = 42;
    
    cxc_unique_ptr_t up1 = cxc_unique_ptr_create(raw_ptr, NULL);
    cxc_unique_ptr_t up2 = cxc_unique_ptr_move(&up1);
    
    EXPECT_TRUE(cxc_unique_ptr_is_null(&up1));
    EXPECT_FALSE(cxc_unique_ptr_is_null(&up2));
    EXPECT_EQ(*(int*)cxc_unique_ptr_get(&up2), 42);
    
    cxc_unique_ptr_release(&up2);
}

TEST(SmartPointerUniquePtr, Reset) {
    int* ptr1 = (int*)cxc_malloc(sizeof(int));
    *ptr1 = 10;
    
    cxc_unique_ptr_t up = cxc_unique_ptr_create(ptr1, NULL);
    
    int* ptr2 = (int*)cxc_malloc(sizeof(int));
    *ptr2 = 20;
    
    cxc_unique_ptr_reset(&up, ptr2);
    EXPECT_EQ(*(int*)cxc_unique_ptr_get(&up), 20);
    
    cxc_unique_ptr_release(&up);
}

// 测试 shared_ptr
TEST(SmartPointerSharedPtr, Create) {
    int* raw_ptr = (int*)cxc_malloc(sizeof(int));
    *raw_ptr = 42;
    
    cxc_shared_ptr_t sp = cxc_shared_ptr_create(raw_ptr, NULL);
    EXPECT_FALSE(cxc_shared_ptr_is_null(&sp));
    EXPECT_EQ(cxc_shared_ptr_use_count(&sp), 1);
    EXPECT_EQ(*(int*)cxc_shared_ptr_get(&sp), 42);
    
    cxc_shared_ptr_release(&sp);
}

TEST(SmartPointerSharedPtr, Copy) {
    int* raw_ptr = (int*)cxc_malloc(sizeof(int));
    *raw_ptr = 42;
    
    cxc_shared_ptr_t sp1 = cxc_shared_ptr_create(raw_ptr, NULL);
    cxc_shared_ptr_t sp2 = cxc_shared_ptr_copy(&sp1);
    
    EXPECT_EQ(cxc_shared_ptr_use_count(&sp1), 2);
    EXPECT_EQ(cxc_shared_ptr_use_count(&sp2), 2);
    EXPECT_EQ(*(int*)cxc_shared_ptr_get(&sp1), 42);
    EXPECT_EQ(*(int*)cxc_shared_ptr_get(&sp2), 42);
    
    cxc_shared_ptr_release(&sp1);
    EXPECT_EQ(cxc_shared_ptr_use_count(&sp2), 1);
    
    cxc_shared_ptr_release(&sp2);
}

TEST(SmartPointerSharedPtr, Reset) {
    int* ptr1 = (int*)cxc_malloc(sizeof(int));
    *ptr1 = 10;
    
    cxc_shared_ptr_t sp = cxc_shared_ptr_create(ptr1, NULL);
    
    int* ptr2 = (int*)cxc_malloc(sizeof(int));
    *ptr2 = 20;
    
    cxc_shared_ptr_reset(&sp, ptr2, NULL);
    EXPECT_EQ(*(int*)cxc_shared_ptr_get(&sp), 20);
    
    cxc_shared_ptr_release(&sp);
}

// 测试 weak_ptr
TEST(SmartPointerWeakPtr, Create) {
    int* raw_ptr = (int*)cxc_malloc(sizeof(int));
    *raw_ptr = 42;
    
    cxc_shared_ptr_t sp = cxc_shared_ptr_create(raw_ptr, NULL);
    cxc_weak_ptr_t wp = cxc_weak_ptr_create(&sp);
    
    EXPECT_FALSE(cxc_weak_ptr_expired(&wp));
    EXPECT_EQ(cxc_weak_ptr_use_count(&wp), 1);
    
    cxc_shared_ptr_release(&sp);
    EXPECT_TRUE(cxc_weak_ptr_expired(&wp));
    
    cxc_weak_ptr_release(&wp);
}

TEST(SmartPointerWeakPtr, Lock) {
    int* raw_ptr = (int*)cxc_malloc(sizeof(int));
    *raw_ptr = 42;
    
    cxc_shared_ptr_t sp1 = cxc_shared_ptr_create(raw_ptr, NULL);
    cxc_weak_ptr_t wp = cxc_weak_ptr_create(&sp1);
    
    cxc_shared_ptr_t sp2 = cxc_weak_ptr_lock(&wp);
    EXPECT_FALSE(cxc_shared_ptr_is_null(&sp2));
    EXPECT_EQ(cxc_shared_ptr_use_count(&sp1), 2);
    EXPECT_EQ(*(int*)cxc_shared_ptr_get(&sp2), 42);
    
    cxc_shared_ptr_release(&sp1);
    cxc_shared_ptr_release(&sp2);
    cxc_weak_ptr_release(&wp);
}
