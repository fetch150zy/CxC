/**
 * @file list.h
 * @brief list 双向链表实现
 * 
 * 提供泛型双向链表和节点内存管理
 */

#ifndef CXC_CONTAINER_LIST_H
#define CXC_CONTAINER_LIST_H

#include "../core/core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 链表节点结构
 * ============================================================================ */

/**
 * @brief 链表节点结构
 */
typedef struct cxc_list_node {
    struct cxc_list_node* prev;   /**< 前驱节点 */
    struct cxc_list_node* next;   /**< 后继节点 */
    cxc_ptr_t data;               /**< 数据指针 */
} cxc_list_node_t;

/**
 * @brief 创建节点
 * @param data 数据指针
 * @return 节点指针，失败返回 NULL
 */
cxc_list_node_t* cxc_list_node_create(cxc_ptr_t data);

/**
 * @brief 销毁节点
 * @param node 节点指针
 */
void cxc_list_node_destroy(cxc_list_node_t* node);

/* ============================================================================
 * list 结构
 * ============================================================================ */

/**
 * @brief list 结构
 */
typedef struct cxc_list {
    cxc_list_node_t* head;        /**< 头节点 */
    cxc_list_node_t* tail;         /**< 尾节点 */
    cxc_size_t size;               /**< 元素数量 */
    cxc_size_t elem_size;          /**< 元素大小 */
    void (*deleter)(cxc_ptr_t);    /**< 删除器函数 */
} cxc_list_t;

/* ============================================================================
 * list 操作
 * ============================================================================ */

/**
 * @brief 创建 list
 * @param elem_size 元素大小
 * @param deleter 删除器函数（可为 NULL）
 * @return list 指针，失败返回 NULL
 */
cxc_list_t* cxc_list_create(cxc_size_t elem_size, void (*deleter)(cxc_ptr_t));

/**
 * @brief 销毁 list
 * @param list list 指针
 */
void cxc_list_destroy(cxc_list_t* list);

/**
 * @brief 清空 list
 * @param list list 指针
 */
void cxc_list_clear(cxc_list_t* list);

/**
 * @brief 获取 list 大小
 * @param list list 指针
 * @return 元素数量
 */
static inline cxc_size_t cxc_list_size(const cxc_list_t* list) {
    return list != NULL ? list->size : 0;
}

/**
 * @brief 检查 list 是否为空
 * @param list list 指针
 * @return 如果为空返回 true，否则返回 false
 */
static inline cxc_bool_t cxc_list_empty(const cxc_list_t* list) {
    return cxc_list_size(list) == 0;
}

/**
 * @brief 在头部插入元素
 * @param list list 指针
 * @param elem 元素指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_list_push_front(cxc_list_t* list, cxc_cptr_t elem);

/**
 * @brief 在尾部插入元素
 * @param list list 指针
 * @param elem 元素指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_list_push_back(cxc_list_t* list, cxc_cptr_t elem);

/**
 * @brief 移除头部元素
 * @param list list 指针
 */
void cxc_list_pop_front(cxc_list_t* list);

/**
 * @brief 移除尾部元素
 * @param list list 指针
 */
void cxc_list_pop_back(cxc_list_t* list);

/**
 * @brief 在指定节点后插入元素
 * @param list list 指针
 * @param node 节点指针
 * @param elem 元素指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_list_insert_after(cxc_list_t* list, cxc_list_node_t* node, cxc_cptr_t elem);

/**
 * @brief 在指定节点前插入元素
 * @param list list 指针
 * @param node 节点指针
 * @param elem 元素指针
 * @return 成功返回 true，失败返回 false
 */
cxc_bool_t cxc_list_insert_before(cxc_list_t* list, cxc_list_node_t* node, cxc_cptr_t elem);

/**
 * @brief 移除指定节点
 * @param list list 指针
 * @param node 节点指针
 */
void cxc_list_erase(cxc_list_t* list, cxc_list_node_t* node);

/**
 * @brief 获取头部节点
 * @param list list 指针
 * @return 节点指针，失败返回 NULL
 */
static inline cxc_list_node_t* cxc_list_front(const cxc_list_t* list) {
    return list != NULL ? list->head : NULL;
}

/**
 * @brief 获取尾部节点
 * @param list list 指针
 * @return 节点指针，失败返回 NULL
 */
static inline cxc_list_node_t* cxc_list_back(const cxc_list_t* list) {
    return list != NULL ? list->tail : NULL;
}

/**
 * @brief 获取节点的数据
 * @param node 节点指针
 * @return 数据指针，失败返回 NULL
 */
static inline cxc_ptr_t cxc_list_node_data(cxc_list_node_t* node) {
    return node != NULL ? node->data : NULL;
}

/**
 * @brief 获取节点的数据（常量）
 * @param node 节点指针
 * @return 数据指针，失败返回 NULL
 */
static inline cxc_cptr_t cxc_list_node_data_c(const cxc_list_node_t* node) {
    return node != NULL ? node->data : NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* CXC_CONTAINER_LIST_H */
