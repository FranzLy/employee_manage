/*
 * @Descripttion:哈希数据存储
 * @version:
 * @Author: liyu
 * @Date: 2022-06-04 09:26:44
 */
#ifndef HASH_DATA_H
#define HASH_DATA_H

#include "common/log.h"
#include "employee_def.h"
#include <malloc.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE (1999) //哈希表的长度，质数
#define ADDR_SIZE (sizeof(char*))

///> 哈希节点
typedef struct _hash_node_t {
    employee_info_type_t info;
    struct _hash_node_t* next;
} hash_node_t;

///> 哈希表
typedef struct _hash_table_t {
    hash_node_t** table;
    unsigned int size;
    unsigned int count;
} hash_table_t;

////////////////////////针对整张哈希表的操作//////////////////////////////////////////////

///> 创建哈希表
hash_table_t* hash_create(unsigned int table_size);

///> 哈希表中插入节点
int hash_insert_node(hash_table_t* hash_table, hash_node_t* hash_node);

///> 哈希表中删除节点
int hash_delete_node(hash_table_t* hash_table, int key_value);

///> 哈希表中查找节点
hash_node_t* hash_search_node(hash_table_t* hash_table, int key_value);

///> 遍历整张哈希表
int hash_traverse_all(hash_table_t* hash_table, void (*func)(employee_info_type_t*, matched_info_type_t*), matched_info_type_t* matched_info);

///> 销毁整张表
int hash_destory(hash_table_t* hash_table);

//////////////////////////////////////////////////////////////////////////////////////

#endif /* HASH_DATA_H */