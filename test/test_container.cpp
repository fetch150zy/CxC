#include <gtest/gtest.h>
extern "C" {
#include "container/container.h"
}
#include <string.h>

// 测试 vector
TEST(ContainerVector, CreateDestroy) {
    cxc_vector_t* vec = cxc_vector_create(sizeof(int), 10);
    ASSERT_NE(vec, nullptr);
    EXPECT_EQ(cxc_vector_size(vec), 0);
    EXPECT_GE(cxc_vector_capacity(vec), 10);
    EXPECT_TRUE(cxc_vector_empty(vec));
    
    cxc_vector_destroy(vec);
}

TEST(ContainerVector, PushBack) {
    cxc_vector_t* vec = cxc_vector_create(sizeof(int), 2);
    ASSERT_NE(vec, nullptr);
    
    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(cxc_vector_push_back(vec, &values[i]));
    }
    
    EXPECT_EQ(cxc_vector_size(vec), 5);
    EXPECT_FALSE(cxc_vector_empty(vec));
    
    for (int i = 0; i < 5; i++) {
        int* val = (int*)cxc_vector_at(vec, i);
        ASSERT_NE(val, nullptr);
        EXPECT_EQ(*val, values[i]);
    }
    
    cxc_vector_destroy(vec);
}

TEST(ContainerVector, PopBack) {
    cxc_vector_t* vec = cxc_vector_create(sizeof(int), 10);
    ASSERT_NE(vec, nullptr);
    
    int value = 42;
    cxc_vector_push_back(vec, &value);
    EXPECT_EQ(cxc_vector_size(vec), 1);
    
    cxc_vector_pop_back(vec);
    EXPECT_EQ(cxc_vector_size(vec), 0);
    EXPECT_TRUE(cxc_vector_empty(vec));
    
    cxc_vector_destroy(vec);
}

TEST(ContainerVector, InsertErase) {
    cxc_vector_t* vec = cxc_vector_create(sizeof(int), 10);
    ASSERT_NE(vec, nullptr);
    
    int values[] = {1, 2, 4, 5};
    for (int i = 0; i < 4; i++) {
        cxc_vector_push_back(vec, &values[i]);
    }
    
    int value3 = 3;
    EXPECT_TRUE(cxc_vector_insert(vec, 2, &value3));
    EXPECT_EQ(cxc_vector_size(vec), 5);
    
    int* val = (int*)cxc_vector_at(vec, 2);
    EXPECT_EQ(*val, 3);
    
    cxc_vector_erase(vec, 2);
    EXPECT_EQ(cxc_vector_size(vec), 4);
    
    cxc_vector_destroy(vec);
}

TEST(ContainerVector, FrontBack) {
    cxc_vector_t* vec = cxc_vector_create(sizeof(int), 10);
    ASSERT_NE(vec, nullptr);
    
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        cxc_vector_push_back(vec, &values[i]);
    }
    
    int* front = (int*)cxc_vector_front(vec);
    int* back = (int*)cxc_vector_back(vec);
    
    EXPECT_EQ(*front, 10);
    EXPECT_EQ(*back, 30);
    
    cxc_vector_destroy(vec);
}

// 测试 list
TEST(ContainerList, CreateDestroy) {
    cxc_list_t* list = cxc_list_create(sizeof(int), NULL);
    ASSERT_NE(list, nullptr);
    EXPECT_EQ(cxc_list_size(list), 0);
    EXPECT_TRUE(cxc_list_empty(list));
    
    cxc_list_destroy(list);
}

TEST(ContainerList, PushPop) {
    cxc_list_t* list = cxc_list_create(sizeof(int), NULL);
    ASSERT_NE(list, nullptr);
    
    int value1 = 10;
    int value2 = 20;
    
    EXPECT_TRUE(cxc_list_push_front(list, &value1));
    EXPECT_TRUE(cxc_list_push_back(list, &value2));
    EXPECT_EQ(cxc_list_size(list), 2);
    
    cxc_list_node_t* front = cxc_list_front(list);
    ASSERT_NE(front, nullptr);
    EXPECT_EQ(*(int*)cxc_list_node_data(front), 10);
    
    cxc_list_pop_front(list);
    EXPECT_EQ(cxc_list_size(list), 1);
    
    cxc_list_pop_back(list);
    EXPECT_TRUE(cxc_list_empty(list));
    
    cxc_list_destroy(list);
}

TEST(ContainerList, InsertErase) {
    cxc_list_t* list = cxc_list_create(sizeof(int), NULL);
    ASSERT_NE(list, nullptr);
    
    int values[] = {1, 2, 4};
    for (int i = 0; i < 3; i++) {
        cxc_list_push_back(list, &values[i]);
    }
    
    cxc_list_node_t* node = cxc_list_back(list);
    int value3 = 3;
    EXPECT_TRUE(cxc_list_insert_before(list, node, &value3));
    
    EXPECT_EQ(cxc_list_size(list), 4);
    
    cxc_list_erase(list, node);
    EXPECT_EQ(cxc_list_size(list), 3);
    
    cxc_list_destroy(list);
}

// 测试 map
TEST(ContainerMap, CreateDestroy) {
    cxc_map_t* map = cxc_map_create(
        sizeof(int), sizeof(int),
        16,
        NULL, NULL,
        NULL, NULL
    );
    ASSERT_NE(map, nullptr);
    EXPECT_EQ(cxc_map_size(map), 0);
    EXPECT_TRUE(cxc_map_empty(map));
    
    cxc_map_destroy(map);
}

TEST(ContainerMap, InsertFind) {
    cxc_map_t* map = cxc_map_create(
        sizeof(int), sizeof(int),
        16,
        NULL, NULL,
        NULL, NULL
    );
    ASSERT_NE(map, nullptr);
    
    int key1 = 1, value1 = 100;
    int key2 = 2, value2 = 200;
    
    EXPECT_TRUE(cxc_map_insert(map, &key1, &value1));
    EXPECT_TRUE(cxc_map_insert(map, &key2, &value2));
    EXPECT_EQ(cxc_map_size(map), 2);
    
    int* found1 = (int*)cxc_map_find(map, &key1);
    ASSERT_NE(found1, nullptr);
    EXPECT_EQ(*found1, 100);
    
    int* found2 = (int*)cxc_map_find(map, &key2);
    ASSERT_NE(found2, nullptr);
    EXPECT_EQ(*found2, 200);
    
    int key3 = 3;
    int* found3 = (int*)cxc_map_find(map, &key3);
    EXPECT_EQ(found3, nullptr);
    
    cxc_map_destroy(map);
}

TEST(ContainerMap, Erase) {
    cxc_map_t* map = cxc_map_create(
        sizeof(int), sizeof(int),
        16,
        NULL, NULL,
        NULL, NULL
    );
    ASSERT_NE(map, nullptr);
    
    int key = 1, value = 100;
    cxc_map_insert(map, &key, &value);
    EXPECT_EQ(cxc_map_size(map), 1);
    
    EXPECT_TRUE(cxc_map_erase(map, &key));
    EXPECT_EQ(cxc_map_size(map), 0);
    EXPECT_FALSE(cxc_map_contains(map, &key));
    
    cxc_map_destroy(map);
}

// 测试 set
TEST(ContainerSet, CreateDestroy) {
    cxc_set_t* set = cxc_set_create(
        sizeof(int),
        16,
        NULL, NULL,
        NULL
    );
    ASSERT_NE(set, nullptr);
    EXPECT_EQ(cxc_set_size(set), 0);
    EXPECT_TRUE(cxc_set_empty(set));
    
    cxc_set_destroy(set);
}

TEST(ContainerSet, InsertErase) {
    cxc_set_t* set = cxc_set_create(
        sizeof(int),
        16,
        NULL, NULL,
        NULL
    );
    ASSERT_NE(set, nullptr);
    
    int value1 = 10;
    int value2 = 20;
    
    EXPECT_TRUE(cxc_set_insert(set, &value1));
    EXPECT_TRUE(cxc_set_insert(set, &value2));
    EXPECT_EQ(cxc_set_size(set), 2);
    
    EXPECT_TRUE(cxc_set_contains(set, &value1));
    EXPECT_TRUE(cxc_set_contains(set, &value2));
    
    EXPECT_TRUE(cxc_set_erase(set, &value1));
    EXPECT_FALSE(cxc_set_contains(set, &value1));
    EXPECT_EQ(cxc_set_size(set), 1);
    
    cxc_set_destroy(set);
}

// 测试 string
TEST(ContainerString, CreateDestroy) {
    cxc_string_t* str = cxc_string_create(16);
    ASSERT_NE(str, nullptr);
    EXPECT_EQ(cxc_string_length(str), 0);
    EXPECT_TRUE(cxc_string_empty(str));
    
    cxc_string_destroy(str);
}

TEST(ContainerString, FromCStr) {
    cxc_string_t* str = cxc_string_from_cstr("Hello");
    ASSERT_NE(str, nullptr);
    EXPECT_EQ(cxc_string_length(str), 5);
    EXPECT_STREQ(cxc_string_cstr(str), "Hello");
    
    cxc_string_destroy(str);
}

TEST(ContainerString, Append) {
    cxc_string_t* str = cxc_string_create(16);
    ASSERT_NE(str, nullptr);
    
    EXPECT_TRUE(cxc_string_append_char(str, 'H'));
    EXPECT_TRUE(cxc_string_append_cstr(str, "ello"));
    EXPECT_EQ(cxc_string_length(str), 5);
    EXPECT_STREQ(cxc_string_cstr(str), "Hello");
    
    cxc_string_destroy(str);
}

TEST(ContainerString, Compare) {
    cxc_string_t* str1 = cxc_string_from_cstr("Hello");
    cxc_string_t* str2 = cxc_string_from_cstr("World");
    cxc_string_t* str3 = cxc_string_from_cstr("Hello");
    
    ASSERT_NE(str1, nullptr);
    ASSERT_NE(str2, nullptr);
    ASSERT_NE(str3, nullptr);
    
    EXPECT_LT(cxc_string_compare(str1, str2), 0);
    EXPECT_EQ(cxc_string_compare(str1, str3), 0);
    
    cxc_string_destroy(str1);
    cxc_string_destroy(str2);
    cxc_string_destroy(str3);
}

TEST(ContainerString, Find) {
    cxc_string_t* str = cxc_string_from_cstr("Hello World");
    ASSERT_NE(str, nullptr);
    
    cxc_size_t pos = cxc_string_find(str, "World");
    EXPECT_EQ(pos, 6);
    
    pos = cxc_string_find(str, "Not Found");
    EXPECT_EQ(pos, (cxc_size_t)-1);
    
    cxc_string_destroy(str);
}

TEST(ContainerString, Substr) {
    cxc_string_t* str = cxc_string_from_cstr("Hello World");
    ASSERT_NE(str, nullptr);
    
    cxc_string_t* substr = cxc_string_substr(str, 0, 5);
    ASSERT_NE(substr, nullptr);
    EXPECT_STREQ(cxc_string_cstr(substr), "Hello");
    
    cxc_string_destroy(str);
    cxc_string_destroy(substr);
}
