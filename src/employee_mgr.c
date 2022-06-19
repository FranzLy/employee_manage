/*
 * @Descripttion: 管理员工信息
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */

#include "employee_mgr.h"

/*
 *@brief 插入一个员工信息
 *@param [in]info 员工信息
 *@return true 成功 false 失败
 */
bool insert_one_employee(const employee_info_type_t* info)
{
    hash_node_t node;

    if (s_hash_table == NULL && (s_hash_table = hash_create(HASH_SIZE)) == NULL) {
        LOG_ERR("Create hash table used to store info failed.");
        return false;
    }

    if (s_hash_table->count == MAX_EMPLOYEE_NUM) {
        LOG_WARN("Up to max employee num %d, cannot add one more.", MAX_EMPLOYEE_NUM);
        return false;
    }

    memset(&node, 0, sizeof(hash_node_t));
    memcpy(&(node.info), info, sizeof(employee_info_type_t));
    node.next = NULL;

    if (hash_insert_node(s_hash_table, &node) != 0) {
        LOG_ERR("Insert hash node to hash table failed.");
        return false;
    }
    LOG_DEBUG("after insert , hash table count = %d\n", s_hash_table->count);

    return true;
}

/*
 *@brief 删除一个员工信息
 *@param [in]work_id 工号
 *@return true 成功 false 失败
 */
bool delete_one_employee_by_id(const char* work_id)
{
    if (s_hash_table == NULL) {
        LOG_WARN("Hash table is NULL.");
        return false;
    }

    if (hash_delete_node(s_hash_table, atoi(work_id)) != 0) {
        LOG_ERR("Delete one node failed.");
        return false;
    }

    return true;
}

/*
 *@brief 修改一个员工信息
 *@param [in]info 员工信息
 *@param [in]type 修改类型
 *@return true 成功 false 失败
 */
bool modify_one_employee_info(employee_info_type_t* info, info_element_type type)
{
    hash_node_t* node = NULL;

    if (info == NULL) {
        return false;
    }

    node = hash_search_node(s_hash_table, atoi(info->work_id));
    if (node == NULL) {
        return false;
    }

    if (type & EMPLOYEE_NAME) {
        memset(node->info.name, 0, MAX_NAME_LEN);
        strncpy(node->info.name, info->name, strlen(info->name));
        node->info.name[strlen(info->name)] = '\0';
    }

    if (type & EMPLOYEE_DATE) {
        memset(&(node->info.date), 0, sizeof(entry_date_type_t));
        memcpy(&(node->info.date), &(info->date), sizeof(entry_date_type_t));
    }

    if (type & EMPLOYEE_DEPARTMENT) {
        memset(node->info.department, 0, MAX_DEPARTMENT_LEN);
        strncpy(node->info.department, info->department, strlen(info->department));
        node->info.department[strlen(info->department)] = '\0';
    }

    if (type & EMPLOYEE_POSITION) {
        memset(node->info.position, 0, MAX_POSITION_LEN);
        strncpy(node->info.position, info->position, strlen(info->position));
        node->info.position[strlen(info->position)] = '\0';
    }

    return true;
}

/*
 *@brief 根据工号查找一个员工信息
 *@param [in]work_id 工号
 *@return true 成功 false 失败
 */
static bool find_one_employee_by_id(const char* work_id)
{
    hash_node_t* node = NULL;

    if (work_id == NULL) {
        return false;
    }

    node = hash_search_node(s_hash_table, atoi(work_id));
    if (node == NULL) {
        return false;
    }

    show_one_employee_info(&(node->info));

    return true;
}

/*
 *@brief 根据类型查找一个员工信息
 *@param [in]matched_info 输入的匹配参数
 *@return true 成功 false 失败
 */
bool find_employee_by_type(matched_info_type_t* matched_info)
{
    hash_node_t *cur, *next;
    employee_info_type_t* info;
    info_element_type type;

    if (matched_info == NULL) {
        return false;
    }

    info = &(matched_info->info);
    type = matched_info->param_type;

    //如果参数只包含work id，则直接用work id检索
    if (type == EMPLOYEE_WORKID) {
        return find_one_employee_by_id(info->work_id);
    }

    //开始遍历，并找出符合条件的元素，加入匹配链表中
    if (hash_traverse_all(s_hash_table, compare_employee_info, matched_info) != 0) {
        return false;
    }

    if (matched_info->sort_type != EMPLOYEE_INVALID) {
        //排序后输出
        sort_matched_info(matched_info->sort_type);
    }

    return true;
}

/*
 *@brief 遍历员工信息
 *@return true 成功 false 失败
 */
bool show_all_employee_info()
{
    return (hash_traverse_all(s_hash_table, compare_employee_info, NULL) == 0);
}