/*
 * @Descripttion: 哈希数据存储
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */

#include "hash_table.h"

////////////////////////针对整张哈希表的操作//////////////////////////////////////////////

/*
 *@brief 创建哈希表
 *@param [in]table_size 哈希表数组大小
 *@return 哈希表指针
 */
hash_table_t* hash_create(unsigned int table_size, unsigned int val_size, update_value_callback update_func, compare_value_callback compare_func)
{
    hash_table_t* ret_table = NULL;

    //参数检查
    if (table_size <= 0 || val_size <= 0) {
        LOG_ERR("hash_create failed cause given table size %d or value size %d is invalid.", table_size, val_size);
        goto out;
    }

    //申请空间初始化整表
    ret_table = (hash_table_t*)malloc(sizeof(hash_table_t));
    if (ret_table == NULL) {
        LOG_ERR("hash_create failed cause malloc table failed.");
        goto out;
    }

    //为表内数组申请空间
    ret_table->table = (hash_node_t**)malloc(table_size * sizeof(hash_node_t*));
    if (ret_table->table == NULL) {
        FREE(ret_table);
        LOG_ERR("hash_create failed cause malloc table element failed.");
        goto out;
    }

    //将表置零，然后初始化表的大小
    memset(ret_table->table, 0, table_size * sizeof(hash_node_t*));
    ret_table->size = table_size;
    ret_table->count = 0;
    ret_table->val_size = val_size;
    ret_table->update_func = update_func;
    ret_table->compare_func = compare_func;

out:
    return ret_table;
}

/*
 *@brief 哈希表中插入节点
 *@param [in]hash_table 哈希表
 *@param [in]key 主键
 *@param [in]value  值
 *@return 0 插入成功 -1 插入失败
 */
int hash_insert_node(hash_table_t* hash_table, int key, void* value)
{
    hash_node_t node, *cur = NULL, *prev = NULL;

    if (hash_table == NULL) {
        LOG_ERR("Use hash_create() create a hash table before insert.");
        return -1;
    }

    if (key <= 0 || value == NULL) {
        LOG_ERR("Wrong hash table key %d or null value used to insert one node.", key);
        return -1;
    }

    cur = hash_table->table[key % (hash_table->size)];
    prev = cur;

    //插入数据
    while (cur) { /* if key is already stroed, not update */
        if (cur->key == key) {
            LOG_WARN("Same element has been stored in hash table.");
            return -1;
        }
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) { /* if key has not been stored, then add it */

        hash_node_t* node = malloc(sizeof(hash_node_t));
        if (node == NULL) {
            LOG_ERR("Malloc for a new element failed.");
            return -1;
        }

        memset(node, 0, sizeof(hash_node_t));
        node->key = key;
        node->value = malloc(hash_table->val_size);
        if (node->value == NULL) {
            LOG_ERR("Malloc for a new element failed.");
            return -1;
        }

        hash_table->update_func(node->value, value);
        node->next = NULL;

        if (prev == NULL) {
            hash_table->table[key % (hash_table->size)] = node;
        } else {
            prev->next = node;
        }
    }
    hash_table->count++;

    return 0;
}

/*
 *@brief 哈希表中删除节点
 *@param [in]hash_table 哈希表
 *@param [in]key 主键
 *@return 0 删除成功 -1 删除失败
 */
int hash_delete_node(hash_table_t* hash_table, int key)
{
    int key_index;
    hash_node_t *prev = NULL, *cur = NULL;

    //参数检查
    if (hash_table == NULL) {
        LOG_ERR("hash_delete_node failed cause hash table has not been created.");
        return -1;
    }

    //查找到该节点后删除
    key_index = key % (hash_table->size);
    cur = hash_table->table[key_index];
    prev = cur;
    while (cur) {
        if (key == cur->key) {
            if (cur == prev) {
                hash_table->table[key_index] = cur->next;
            } else {
                prev->next = cur->next;
            }
            FREE(cur->value);
            FREE(cur);

            hash_table->count--;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }

    LOG_WARN("hash_delete_node failed cause no node match key %d.", key);
    return -1;
}

/*
 *@brief 哈希表中更新节点
 *@param [in]hash_table 哈希表
 *@param [in]key 主键
 *@return 0 更新成功 -1 更新失败
 */
int hash_update_node(hash_table_t* hash_table, int key, void* value)
{
    int key_index;
    hash_node_t *prev = NULL, *cur = NULL;

    //参数检查
    if (hash_table == NULL || value == NULL) {
        LOG_ERR("hash_delete_node failed cause hash table has not been created or value is null.");
        return -1;
    }

    //查找到该节点后更新值
    key_index = key % (hash_table->size);
    cur = hash_table->table[key_index];
    prev = cur;
    while (cur) {
        if (key == cur->key) {
            hash_table->update_func(cur->value, value);
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }

    LOG_WARN("hash_update_node failed cause no node match key %d.", key);
    return -1;
}

/*
 *@brief 哈希表中查找节点
 *@param [in]hash_table 哈希表
 *@param [in]key 主键
 *@return 成功：键值为key的节点    失败：NULL
 */
void* hash_search_node(hash_table_t* hash_table, int key)
{
    hash_node_t* ret_node = NULL;
    int key_index;

    //参数检查
    if (hash_table == NULL) {
        LOG_ERR("hash_search_node failed cause hash table has not been created.");
        goto out;
    }

    //按key值查找
    key_index = key % (hash_table->size);
    ret_node = hash_table->table[key_index];
    while (ret_node) {
        if (ret_node->key == key) {
            break;
        }
        ret_node = ret_node->next;
    }

out:
    if (ret_node != NULL) {
        return ret_node->value;
    }
    return ret_node;
}

/*
 *@brief 遍历整张哈希表
 *@param [in]hash_table 哈希表
 *@param [out]count 返回的元素个数
 *@param [in]value  值
 *@return 0 成功 -1 失败
 */
void** hash_traverse_all_nodes(hash_table_t* hash_table, int* count, void* value)
{
    int i;
    hash_node_t* cur;

    //参数检查
    if (hash_table == NULL || count == NULL) {
        LOG_ERR("hash_traverse_all_nodes failed cause hash table has not been created.");
        return NULL;
    }

    //按照当前哈希表中的最大元素容量申请数组空间
    void** matched_values = (void**)malloc(sizeof(void*) * (hash_table->count));
    if (matched_values == NULL) {
        return NULL;
    }

    //遍历整表
    (*count) = 0;
    for (i = 0; i < hash_table->size; i++) {
        cur = hash_table->table[i];

        while (cur) {
            if (hash_table->compare_func(cur->value, value)) {
                matched_values[*count] = cur->value;
                (*count)++;
            }
            cur = cur->next;
        }
    }

    //若未匹配,则释放空间
    if ((*count) == 0) {
        FREE(matched_values);
    }

    return matched_values;
}

/*
 *@brief 释放链表的空间
 *@param [in]head 链表头
 *@return 0 成功 -1 失败
 */
static int clear_list_space(hash_node_t* head)
{
    hash_node_t *cur = NULL, *next = NULL;

    if (head == NULL) {
        LOG_ERR("clear_list_space failed cause list is null.");
        return -1;
    }

    cur = head;
    while (cur) {
        next = cur->next;
        FREE(cur->value);
        FREE(cur);
        cur = next;
    }

    return 0;
}

/*
 *@brief 销毁整张表
 *@param [in]hash_table 哈希表
 *@return 0 成功 -1 失败
 */
int hash_destory(hash_table_t* hash_table)
{
    int i;
    hash_node_t* head;

    //参数检查
    if (hash_table == NULL) {
        LOG_ERR("hash_destory failed cause param is invalid.");
        return -1;
    }

    //遍历整表，并释放每个节点链上的空间
    for (i = 0; i < hash_table->size; i++) {
        head = hash_table->table[i];
        if (head == NULL) {
            continue;
        }

        if (clear_list_space(head) != 0) {
            LOG_ERR("hash_traverse_all failed cause clear_list_space failed.\n");
            return -1;
        }
    }

    //释放哈希桶所申请的空间
    FREE(hash_table->table);
    hash_table->size = 0;
    hash_table->count = 0;

    FREE(hash_table);

    return 0;
}
