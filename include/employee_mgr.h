#ifndef EMPLOYEE_MGR_H
#define EMPLOYEE_MGR_H

#include "common.h"
#include "employee_def.h"
#include "hash_data.h"
#include "employee_utils.h"

hash_table_t* s_hash_table;

///> 插入一个员工信息
bool insert_one_employee(const employee_info_type_t* info);

///> 删除一个员工信息
bool delete_one_employee_by_id(const char* work_id);

///> 修改一个员工信息
bool modify_one_employee_info(employee_info_type_t* info, info_element_type type);

///> 根据工号查找一个员工信息
bool find_one_employee_by_id(const char* work_id);

///> 根据类型查找一个员工信息
bool find_employee_by_type(matched_info_type_t* matched_info);

///> 遍历员工信息
bool show_all_employee_info();

#endif /* EMPLOYEE_MGR_H */