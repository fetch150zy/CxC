/**
 * @file string.h
 * @brief string 动态字符串实现
 * 
 * 提供动态字符串和常用字符串操作
 */

#ifndef CXC_CONTAINER_STRING_H
#define CXC_CONTAINER_STRING_H

#include "../core/core.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * string 结构
 * ============================================================================ */

/**
 * @brief string 结构
 */
typedef struct cxc_string {
    char* data;             /**< 字符串数据 */
    cxc_size_t length;       /**< 字符串长度 */
    cxc_size_t capacity;    /**< 容量 */
} cxc_string_t;

/* ============================================================================
 * string 操作
 * ============================================================================ */

/**
 * @brief 创建 string
 * @param initial_capacity 初始容量
 * @return string 指针，失败返回 NULL
 */
cxc_string_t* cxc_string_create(cxc_size_t initial_capacity);

/**
 * @brief 从 C 字符串创建 string
 * @param cstr C 字符串
 * @return string 指针，失败返回 NULL
 */
cxc_string_t* cxc_string_from_cstr(const char* cstr);

/**
 * @brief 销毁 string
 * @param str string 指针
 */
void cxc_string_destroy(cxc_string_t* str);

/**
 * @brief 清空 string
 * @param str string 指针
 */
void cxc_string_clear(cxc_string_t* str);

/**
 * @brief 获取 string 长度
 * @param str string 指针
 * @return 字符串长度
 */
static inline cxc_size_t cxc_string_length(const cxc_string_t* str) {
    return str != NULL ? str->length : 0;
}

/**
 * @brief 获取 string 容量
 * @param str string 指针
 * @return 容量
 */
static inline cxc_size_t cxc_string_capacity(const cxc_string_t* str) {
    return str != NULL ? str->capacity : 0;
}

/**
 * @brief 检查 string 是否为空
 * @param str string 指针
 * @return 如果为空返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_string_empty(const cxc_string_t* str) {
    return cxc_string_length(str) == 0;
}

/**
 * @brief 获取 C 字符串
 * @param str string 指针
 * @return C 字符串指针，失败返回 NULL
 */
static inline const char* cxc_string_cstr(const cxc_string_t* str) {
    return str != NULL ? str->data : NULL;
}

/**
 * @brief 追加字符
 * @param str string 指针
 * @param c 字符
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_string_append_char(cxc_string_t* str, char c);

/**
 * @brief 追加 C 字符串
 * @param str string 指针
 * @param cstr C 字符串
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_string_append_cstr(cxc_string_t* str, const char* cstr);

/**
 * @brief 追加另一个 string
 * @param str string 指针
 * @param other 另一个 string 指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_string_append_string(cxc_string_t* str, const cxc_string_t* other);

/**
 * @brief 设置内容
 * @param str string 指针
 * @param cstr C 字符串
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_string_assign(cxc_string_t* str, const char* cstr);

/**
 * @brief 获取指定位置的字符
 * @param str string 指针
 * @param index 索引
 * @return 字符，失败返回 '\0'
 */
static inline char cxc_string_at(const cxc_string_t* str, cxc_size_t index) {
    if (str == NULL || index >= str->length) {
        return '\0';
    }
    return str->data[index];
}

/**
 * @brief 比较两个 string
 * @param str1 第一个 string 指针
 * @param str2 第二个 string 指针
 * @return 如果相等返回 0，str1 < str2 返回负数，否则返回正数
 */
int cxc_string_compare(const cxc_string_t* str1, const cxc_string_t* str2);

/**
 * @brief 查找子字符串
 * @param str string 指针
 * @param substr 子字符串
 * @return 找到返回索引，未找到返回 (cxc_size_t)-1
 */
cxc_size_t cxc_string_find(const cxc_string_t* str, const char* substr);

/**
 * @brief 子字符串
 * @param str string 指针
 * @param start 起始索引
 * @param length 长度
 * @return 新的 string 指针，失败返回 NULL
 */
cxc_string_t* cxc_string_substr(const cxc_string_t* str, cxc_size_t start, cxc_size_t length);

#ifdef __cplusplus
}
#endif

#endif /* CXC_CONTAINER_STRING_H */
