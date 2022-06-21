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

    //确保哈希表存在
    if (s_hash_table == NULL && (s_hash_table = hash_create(HASH_SIZE)) == NULL) {
        LOG_ERR("Create hash table used to store info failed.");
        return false;
    }

    //判断当前存储的员工个数是否超过最大值
    if (s_hash_table->count == MAX_EMPLOYEE_NUM) {
        LOG_WARN("Up to max employee num %d, cannot add one more.", MAX_EMPLOYEE_NUM);
        return false;
    }

    //创建节点存储员工信息
    memset(&node, 0, sizeof(hash_node_t));
    memcpy(&(node.info), info, sizeof(employee_info_type_t));
    node.next = NULL;

    //插入
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
    //确保哈希表不为空
    if (s_hash_table == NULL) {
        LOG_WARN("Hash table is NULL.");
        return false;
    }

    //按工号查找删除该员工的信息
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

    //按工号查找
    node = hash_search_node(s_hash_table, atoi(info->work_id));
    if (node == NULL) {
        return false;
    }

    //修改name
    if (type & EMPLOYEE_NAME) {
        memset(node->info.name, 0, MAX_NAME_LEN);
        strncpy(node->info.name, info->name, strlen(info->name));
        node->info.name[strlen(info->name)] = '\0';
    }

    //修改date
    if (type & EMPLOYEE_DATE) {
        memset(&(node->info.date), 0, sizeof(entry_date_type_t));
        memcpy(&(node->info.date), &(info->date), sizeof(entry_date_type_t));
    }

    //修改department
    if (type & EMPLOYEE_DEPARTMENT) {
        memset(node->info.department, 0, MAX_DEPARTMENT_LEN);
        strncpy(node->info.department, info->department, strlen(info->department));
        node->info.department[strlen(info->department)] = '\0';
    }

    //修改position
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

    //确保work id非空
    if (work_id == NULL) {
        return false;
    }

    //按工号在哈希表中查找
    node = hash_search_node(s_hash_table, atoi(work_id));
    if (node == NULL) {
        return false;
    }

    //显示
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
    //匹配内存申请释放

    //传入指针参数检查非空
    if (matched_info == NULL) {
        return false;
    }

    //如果参数只包含work id，则直接用work id检索
    if (matched_info->param_type == EMPLOYEE_WORKID) {
        return find_one_employee_by_id(matched_info->info.work_id);
    }

    //开始遍历，并找出符合条件的元素
    if (hash_traverse_all(s_hash_table, compare_employee_info, matched_info) != 0) {
        return false;
    }

    //检查是否需要排序输出
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
    //遍历过程中，利用compare_employee_info()函数进行比较元素
    return (hash_traverse_all(s_hash_table, compare_employee_info, NULL) == 0);
}