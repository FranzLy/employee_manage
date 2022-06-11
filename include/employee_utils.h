#ifndef EMPLOYEE_UTILS_H
#define SF_EMPLOYEE_UTILS_H

#include "employee_def.h"
#include "hash_data.h"
#include <stdbool.h>

///> 比较员工信息
void compare_employee_info(employee_info_type_t* info, matched_info_type_t* matched_info);

///> 插入匹配链表
void insert_matched_info(employee_info_type_t* info);

///> 根据类型排序
void sort_matched_info(info_element_type sort_type);

///> 比较工号
int compare_work_id(employee_info_type_t* first, employee_info_type_t* second);

///> 比较日期
int compare_entry_date(employee_info_type_t* first, employee_info_type_t* second);

///> 显示单个员工信息
void show_one_employee_info(employee_info_type_t* info);

#endif /* EMPLOYEE_UTILS_H */