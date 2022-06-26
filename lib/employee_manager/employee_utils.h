/*
 * @Descripttion: 管理员工信息
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#ifndef EMPLOYEE_UTILS_H
#define SF_EMPLOYEE_UTILS_H

#include "employee_info.h"
#include "hash_table.h"
#include <stdbool.h>

///> 存储员工信息的全局哈希表
static hash_table_t* s_hash_table; 

///> 比较员工信息
bool compare_employee_info(const employee_info_t* src, const employee_info_t* dst);

///> 更新员工信息
void update_employee_info(employee_info_t* src, const employee_info_t* dst);

///> 根据类型排序
void sort_matched_info(employee_info_t** matched_info, int matched_cnt, sort_type_t sort_type);

///> 显示单个员工信息
void show_one_employee_info(employee_info_t* info);

///> 显示全部匹配的员工信息
void show_matched_employee_info(employee_info_t** matched_info, int matched_cnt);

#endif /* EMPLOYEE_UTILS_H */