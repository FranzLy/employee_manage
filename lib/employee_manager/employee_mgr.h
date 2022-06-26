/*
 * @Descripttion: 管理员工信息
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#ifndef EMPLOYEE_MGR_H
#define EMPLOYEE_MGR_H

#include "employee_utils.h"
#include "log.h"

///> 插入一个员工信息
bool insert_one_employee(const employee_info_t* info);

///> 删除一个员工信息
bool delete_one_employee(int work_id);

///> 修改一个员工信息
bool modify_one_employee_info(const employee_info_t* info);

///> 根据员工id查找信息
employee_info_t* find_employee_by_id(int work_id);

///> 根据过滤信息查找一个员工信息
employee_info_t** find_employee_by_info(const employee_info_t* info, int* count);

///> 遍历获取所有员工信息
employee_info_t** traverse_employee_info(int* count);

///> 清除所有员工信息
bool clear_all_employee_info();

#endif /* EMPLOYEE_MGR_H */