/*
 * @Descripttion: 管理员工信息
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */

#include "employee_mgr.h"

static hash_table_t* s_hash_table = NULL;

/*
 *@brief 插入一个员工信息
 *@param [in]info 员工信息
 *@return true 成功 false 失败
 */
bool insert_one_employee(const employee_info_t* info)
{
    if (info == NULL) {
        return false;
    }

    if (s_hash_table == NULL
        && (s_hash_table = hash_create(HASH_SIZE, sizeof(employee_info_t), update_employee_info, compare_employee_info)) == NULL) {
        LOG_ERR("Init hash table used to store employee info failed.");
        return false;
    }

    return (hash_insert_node(s_hash_table, atoi(info->work_id), info) == 0);
}

/*
 *@brief 删除一个员工信息
 *@param [in]work_id 工号
 *@return true 成功 false 失败
 */
bool delete_one_employee(int work_id)
{
    return (hash_delete_node(s_hash_table, work_id) == 0);
}

/*
 *@brief 修改一个员工信息
 *@param [in]info 员工信息
 *@return true 成功 false 失败
 */
bool modify_one_employee_info(const employee_info_t* info)
{
    if (info == NULL) {
        return false;
    }
    return (hash_update_node(s_hash_table, atoi(info->work_id), info) == 0);
}

/*
 *@brief 根据工号查找一个员工信息
 *@param [in]work_id 工号
 *@return 员工信息
 */
employee_info_t* find_employee_by_id(int work_id)
{
    return (employee_info_t*)hash_search_node(s_hash_table, work_id);
}

/*
 *@brief 根据类型查找一个员工信息
 *@param [in]info 输入的匹配参数
 *@param [out]匹配的员工个数
 *@return 匹配的员工信息
 */
employee_info_t** find_employee_by_info(const employee_info_t* info, int* count)
{
    if (info == NULL) {
        return false;
    }
    return (employee_info_t**)hash_traverse_all_nodes(s_hash_table, count, info);
}

/*
 *@brief 遍历员工信息
 *@param [out]匹配的员工个数
 *@return 匹配的员工信息
 */
employee_info_t** traverse_employee_info(int* count)
{
    return (employee_info_t**)hash_traverse_all_nodes(s_hash_table, count, NULL);
}

/*
 *@brief  清除所有员工信息
 *@return true 成功 false 失败
 */
bool clear_all_employee_info()
{
    return (hash_destory(s_hash_table) == 0);
}
