#include "hash_data.h"

////////////////////////针对单个哈希链表的操作//////////////////////////////////////////////

int hash_destory_node(hash_node_t* head)
{
    hash_node_t *cur = NULL, *next = NULL;

    if (head == NULL) {
        LOG_ERR("hash_destory_node failed cause list is null.");
        return -1;
    }

    cur = head->next;
    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    head->next = NULL;

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////

////////////////////////针对整张哈希表的操作//////////////////////////////////////////////

/*
 *@brief 创建哈希表
 *@param [in]table_size 哈希表数组大小
 *@return 哈希表指针
 */
hash_table_t* hash_create(unsigned int table_size)
{
    hash_table_t* ret_table = NULL;

    //参数检查
    if (table_size <= 0) {
        LOG_ERR("hash_create failed cause given table size %d is invalid.", table_size);
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
        free(ret_table);
        LOG_ERR("hash_create failed cause malloc table element failed.");
        goto out;
    }

    //将表置零，然后初始化表的大小
    memset(ret_table->table, 0, table_size * sizeof(hash_node_t*));
    ret_table->size = table_size;
    ret_table->count = 0;

out:
    return ret_table;
}

/*
 *@brief 哈希表中插入节点
 *@param [in]hash_table 哈希表
 *@param [in]hash_node  哈希节点
 *@return 0 插入成功 -1 插入失败
 */
int hash_insert_node(hash_table_t* hash_table, hash_node_t* hash_node)
{
    int key;
    hash_node_t *head, *prev;

    //参数检查
    if (hash_table == NULL || hash_node == NULL) {
        LOG_ERR("hash_insert_node failed cause param is invalid.");
        return -1;
    }

    key = (atoi(hash_node->info.work_id)) % (hash_table->size);
    head = hash_table->table[key];
    prev = head;

    //插入数据
    while (head) { /* if key is already stroed, update its value */
        if (strncmp(head->info.work_id, hash_node->info.work_id, strlen(hash_node->info.work_id)) == 0) {
            LOG_ERR("Employee %s has been added.", hash_node->info.work_id);
            return -1;
        }
        prev = head;
        head = head->next;
    }

    if (head == NULL) { /* if key has not been stored, then add it */

        hash_node_t* node = malloc(sizeof(hash_node_t));
        if (NULL == node) {
            free(node);
            node = NULL;
            return -1;
        }

        memset(node, 0, sizeof(hash_node_t));
        memcpy(&(node->info), &(hash_node->info), sizeof(employee_info_type_t));
        node->next = NULL;

        if (prev == NULL) {
            hash_table->table[key] = node;
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
 *@param [in]key_value  key值
 *@return 0 删除成功 -1 删除失败
 */
int hash_delete_node(hash_table_t* hash_table, int key_value)
{
    int key_index;
    hash_node_t *prev, *cur;

    //参数检查
    if (hash_table == NULL) {
        LOG_ERR("hash_delete_node failed cause param is invalid.");
        return -1;
    }

    //查找到该节点
    key_index = key_value % (hash_table->size);
    cur = hash_table->table[key_index];
    prev = cur;
    while (cur) {
        if (key_value == atoi(cur->info.work_id)) {
            if (cur == prev) {
                hash_table->table[key_index] = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            cur = NULL;

            hash_table->count--;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }

    LOG_ERR("hash_delete_node failed cause no node match key_value %d.", key_index);
    return -1;
}

/*
 *@brief 哈希表中查找节点
 *@param [in]hash_table 哈希表
 *@param [in]key_value   键值
 *@return 成功：键值为key_value的节点    失败：NULL
 */
hash_node_t* hash_search_node(hash_table_t* hash_table, int key_value)
{
    hash_node_t* ret_node = NULL;
    int key_index;

    //参数检查
    if (hash_table == NULL) {
        LOG_ERR("hash_search_node failed cause param is invalid.");
        goto out;
    }

    key_index = key_value % (hash_table->size);
    ret_node = hash_table->table[key_index];
    while (ret_node) {
        if ((atoi(ret_node->info.work_id)) == key_value) {
            break;
        }
        ret_node = ret_node->next;
    }

out:
    return ret_node;
}

/*
 *@brief 遍历整张哈希表
 *@param [in]hash_table 哈希表
 *@return 0 成功 -1 失败
 */
int 
hash_traverse_all(hash_table_t* hash_table,void (*func)(employee_info_type_t*, matched_info_type_t* ),matched_info_type_t* matched_info)
{
    int i;
    hash_node_t* cur;

    //参数检查
    if (hash_table == NULL) {
        LOG_ERR("hash_search_node failed cause param is invalid.");
        return -1;
    }

    //遍历
    for (i = 0; i < hash_table->size; i++) {
        cur = hash_table->table[i];

        while(cur){
            func(&(cur->info),matched_info);
            cur = cur->next;
        }
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
        if (head->next == NULL) {
            continue;
        }

        if (hash_destory_node(head) != 0) {
            LOG_ERR("hash_traverse_all failed cause hash_destory_node failed.\n");
            return -1;
        }
    }

    free(hash_table->table);
    hash_table->table = NULL;
    hash_table->size = 0;
    hash_table->count = 0;

    free(hash_table);
    hash_table = NULL;

    return 0;
}
