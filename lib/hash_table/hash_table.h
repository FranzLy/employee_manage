/*
 * @Descripttion:哈希数据存储
 * @version:
 * @Author: liyu
 * @Date: 2022-06-04 09:26:44
 */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "command_def.h"
#include "common.h"
#include "employee_info.h"
#include "log.h"
#include <malloc.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE (1999) //哈希表的长度，质数

///> 哈希节点
typedef struct _hash_node_t {
    int key; //键
    void* value; //值
    struct _hash_node_t* next;
} hash_node_t;

///> 哈希节点值比较的回调函数
typedef bool (*compare_value_callback)(const void* src, const void* dst);

///> 哈希节点值更新的回调函数
typedef void (*update_value_callback)(void* src, const void* dst);

///> 哈希表
typedef struct _hash_table_t {
    hash_node_t** table; //哈希桶
    unsigned int size; //桶的大小
    unsigned int count; //当前元素数目
    unsigned int val_size; //存储值的大小

    update_value_callback update_func; //值更新的回调函数
    compare_value_callback compare_func; //值比较的回调函数

} hash_table_t;

////////////////////////针对整张哈希表的操作//////////////////////////////////////////////

///> 创建哈希表
hash_table_t* hash_create(unsigned int table_size, unsigned int val_size, update_value_callback update_func, compare_value_callback compare_func);

///> 哈希表中插入节点
int hash_insert_node(hash_table_t* hash_table, int key, void* value);

///> 哈希表中删除节点
int hash_delete_node(hash_table_t* hash_table, int key);

///> 哈希表中更新节点
int hash_update_node(hash_table_t* hash_table, int key, void* value);

///> 哈希表中查找节点
void* hash_search_node(hash_table_t* hash_table, int key);

///> 哈希表中遍历所有节点,可指定匹配信息
void** hash_traverse_all_nodes(hash_table_t* hash_table, int* count, void* value);

///> 销毁整张表
int hash_destory(hash_table_t* hash_table);

//////////////////////////////////////////////////////////////////////////////////////

#endif /* HASH_TABLE_H */