#include "employee_utils.h"

/*
 *@brief 比较员工信息的回调函数
 *param [in]info 存储的员工信息
 *param [in]matched_info 输入的参数
 */
void compare_employee_info(employee_info_type_t* info, matched_info_type_t* matched_info)
{
    bool matched = true;
    info_element_type param_type = EMPLOYEE_INVALID, sort_type = EMPLOYEE_INVALID;
    employee_info_type_t* cmp_info = NULL;

    if (info == NULL) {
        return;
    }

    if (matched_info == NULL) {
        goto out;
    }

    param_type = matched_info->param_type;
    sort_type = matched_info->sort_type;
    cmp_info = &(matched_info->info);

    if (param_type == EMPLOYEE_INVALID) {
        goto out;
    }

    //检查匹配工号
    if (param_type & EMPLOYEE_WORKID) {
        if (strlen(info->work_id) == strlen(cmp_info->work_id)
            && strncmp(info->work_id, cmp_info->work_id, strlen(cmp_info->work_id)) == 0) {
            matched &= true;
        } else {
            matched &= false;
        }
    }

    //检查匹配姓名
    if (param_type & EMPLOYEE_NAME) {
        if (strlen(info->name) == strlen(cmp_info->name)
            && strncmp(info->name, cmp_info->name, strlen(cmp_info->name)) == 0) {
            matched &= true;
        } else {
            matched &= false;
        }
    }

    //检查匹配部门
    if (param_type & EMPLOYEE_DEPARTMENT) {
        if (strlen(info->department) == strlen(cmp_info->department)
            && strncmp(info->department, cmp_info->department, strlen(cmp_info->department)) == 0) {
            matched &= true;
        } else {
            matched &= false;
        }
    }

    //检查匹配职位
    if (param_type & EMPLOYEE_POSITION) {
        if (strlen(info->position) == strlen(cmp_info->position)
            && strncmp(info->position, cmp_info->position, strlen(cmp_info->position)) == 0) {
            matched &= true;
        } else {
            matched &= false;
        }
    }

    //检查匹日期
    if (param_type & EMPLOYEE_DATE) {
        if (info->date.year == cmp_info->date.year
            && info->date.month == cmp_info->date.month
            && info->date.day == cmp_info->date.day) {
            matched &= true;
        } else {
            matched &= false;
        }
    }

out:
    if (matched) {
        if (sort_type == EMPLOYEE_INVALID) {
            //不需要排序，直接输出
            show_one_employee_info(info);
        } else {
            //需要排序，则需将元素插入匹配链表中
            insert_matched_info(info);
        }
    }
}

/*
 *@brief 插入匹配链表
 *@param [in]info 员工信息
 */
void insert_matched_info(employee_info_type_t* info)
{
    hash_node_t *head, *prev;

    //参数检查
    if (info == NULL) {
        LOG_ERR("insert_matched_info failed cause param is invalid.");
        return;
    }

    //按照最大员工人数申请匹配数组
    if (s_matched_arr == NULL) {
        s_matched_arr = (employee_info_type_t**)malloc(sizeof(employee_info_type_t*) * MAX_EMPLOYEE_NUM);
        if (s_matched_arr == NULL) {
            LOG_ERR("malloc for s_matched_arr failed.");
            return;
        }
        memset(s_matched_arr, 0, sizeof(employee_info_type_t*) * MAX_EMPLOYEE_NUM);
    }

    //记录匹配元素的地址
    s_matched_arr[s_matched_count] = info;
    s_matched_count++;
}

/*
 *@brief 排序匹配信息
 *@param [in]sort_type 排序类型
 */
void sort_matched_info(info_element_type sort_type)
{
    int i = 0;

    if (s_matched_arr == NULL || s_matched_count == 0) {
        return;
    }

    //排序后输出
    if (sort_type & EMPLOYEE_WORKID) {
        qsort(s_matched_arr, s_matched_count, sizeof(employee_info_type_t*), compare_work_id);
    } else if (sort_type & EMPLOYEE_DATE) {
        qsort(s_matched_arr, s_matched_count, sizeof(employee_info_type_t*), compare_entry_date);
    }
    for (; i < s_matched_count; i++) {
        show_one_employee_info(s_matched_arr[i]);
    }

    //清空数组
    free(s_matched_arr);
    s_matched_arr = NULL;
    s_matched_count = 0;
}

/*
 *@brief 比较工号
 *@param [in]first second 员工信息
 */
int compare_work_id(employee_info_type_t** first, employee_info_type_t** second)
{
    return (atoi((*first)->work_id) > atoi((*second)->work_id));
}

/*
 *@brief 比较日期
 *@param [in]first second 员工信息
 */
int compare_entry_date(employee_info_type_t** first, employee_info_type_t** second)
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
 *@brief 显示单个员工信息
 */
void show_one_employee_info(employee_info_type_t* info)
{
    if (info == NULL) {
        return;
    }

    LOG_INFO("employee info: work id: %s, name: %s, date:%d.%d.%d, department:%s, position:%s",
        info->work_id, info->name, info->date.year, info->date.month, info->date.day, info->department, info->position);
}