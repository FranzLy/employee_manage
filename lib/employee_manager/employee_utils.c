/*
 * @Descripttion: 管理员工信息
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */

#include "employee_utils.h"

/*
 *@brief 比较工号
 *@param [in]first second 员工信息
 */
static int compare_work_id(employee_info_t** first, employee_info_t** second)
{
    return (atoi((*first)->work_id) > atoi((*second)->work_id));
}

/*
 *@brief 比较日期
 *@param [in]first second 员工信息
 */
static int compare_entry_date(employee_info_t** first, employee_info_t** second)
{
    entry_date_type_t *first_date = &((*first)->date), *second_date = &((*second)->date);

    if (first_date->year != second_date->year) {
        return (first_date->year > second_date->year);
    }

    if (first_date->month != second_date->month) {
        return (first_date->month > second_date->month);
    }

    return (first_date->day >= second_date->day);
}

/*
 *@brief 判断字符串是否相等
 *@param [in]src dst 字符串
 *@return 比较结果
 */
static bool is_string_equal(const char* src, const char* dst)
{
    if (src == NULL && dst == NULL) {
        return true;
    }

    if (src == NULL || dst == NULL) {
        return false;
    }

    if (strlen(src) == strlen(dst) && (strncmp(src, dst, strlen(src)) == 0)) {
        return true;
    }

    return false;
}

/*
 *@brief 判断日期是否相等
 *@param [in]src dst 日期
 *@return 比较结果
 */
static bool is_date_equal(const entry_date_type_t* src, const entry_date_type_t* dst)
{
    if (src == NULL && dst == NULL) {
        return true;
    }

    if (src == NULL || dst == NULL) {
        return false;
    }

    if ((src->year == dst->year) && (src->month == dst->month) && (src->day == src->day)) {
        return true;
    }

    return false;
}

/*
 *@brief 比较员工信息的回调函数
 *param [in]src 存储的员工信息
 *param [in]dst  输入的员工信息
 */
bool compare_employee_info(const employee_info_t* src, const employee_info_t* dst)
{
    employee_attr_type_t attr_type;

    if (dst == NULL) {
        return true;
    }

    if (src == NULL) {
        return false;
    }

    //比较姓名
    if ((dst->attr_type & EMPLOYEE_NAME) && !is_string_equal(src->name, dst->name)) {
        return false;
    }

    //比较入职日期
    if ((dst->attr_type & EMPLOYEE_DATE) && !is_date_equal(&(src->date), &(dst->date))) {
        return false;
    }

    //比较部门
    if ((dst->attr_type & EMPLOYEE_DEPARTMENT) && !is_string_equal(src->department, dst->department)) {
        return false;
    }

    //比较职位
    if ((dst->attr_type & EMPLOYEE_POSITION) && !is_string_equal(src->position, dst->position)) {
        return false;
    }

    return true;
}

/*
 *@brief 更新员工信息的回调函数
 *param [in]src 存储的员工信息
 *param [in]dst  输入的员工信息
 */
void update_employee_info(employee_info_t* src, const employee_info_t* dst)
{
    if (src == NULL || dst == NULL) {
        LOG_INFO("update_employee_info() do nothing cause dst is null.");
        return;
    }
    memset(src, 0, sizeof(employee_info_t));

    //更新工号
    COPY_STRING(src->work_id, dst->work_id, strlen(dst->work_id));

    //若输入姓名,则该字段不为空,更新
    if (strlen(dst->name) != 0) {
        COPY_STRING(src->name, dst->name, strlen(dst->name));
    }

    //若输入日期,则该字段不为空,更新
    if (!IS_EMPTY_DATE(dst->date)) {
        src->date.year = dst->date.year;
        src->date.month = dst->date.month;
        src->date.day = dst->date.day;
    }

    //若输入部门,则该字段不为空,更新
    if (strlen(dst->department) != 0) {
        COPY_STRING(src->department, dst->department, strlen(dst->department));
    }

    //若输入职位,则该字段不为空,更新
    if (strlen(dst->position) != 0) {
        COPY_STRING(src->position, dst->position, strlen(dst->position));
    }

    //调试日志
    LOG_DEBUG("src size = %d, dst size = %d", sizeof(*src), sizeof(*dst));
    if (g_log_level == LOG_LEVEL_DEBUG) {
        show_one_employee_info(dst);
        show_one_employee_info(src);
    }
}

/*
 *@brief 排序匹配信息
 *@param [in]matched_info 数组
 *@param [in]matched_cnt 数组大小
 *@param [in]sort_type 排序类型
 */
void sort_matched_info(employee_info_t** matched_info, int matched_cnt, sort_type_t sort_type)
{
    if (matched_info == NULL || matched_cnt == 0) {
        return;
    }

    //排序按类型排序
    if (sort_type & SORT_BY_ID) {
        qsort(matched_info, matched_cnt, sizeof(employee_info_t*), compare_work_id);
    } else if (sort_type & SORT_BY_DATE) {
        qsort(matched_info, matched_cnt, sizeof(employee_info_t*), compare_entry_date);
    }
}

/*
 *@brief 显示单个员工信息
 */
void show_one_employee_info(employee_info_t* info)
{
    if (info == NULL) {
        return;
    }

    LOG_INFO("employee info: work id: %s, name: %s, date:%d.%d.%d, department:%s, position:%s",
        info->work_id, info->name, info->date.year, info->date.month, info->date.day, info->department, info->position);
}

/*
 *@brief 显示匹配的员工信息
 */
void show_matched_employee_info(employee_info_t** matched_info, int matched_cnt)
{
    if (matched_info == NULL || matched_cnt == 0) {
        return;
    }

    for (int i = 0; i < matched_cnt; i++) {
        show_one_employee_info(matched_info[i]);
    }
}