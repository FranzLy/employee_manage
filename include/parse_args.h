#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "employee_mgr.h"
#include "stdlib.h"
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#define ADD_COMMAND_STR "add"
#define DEL_COMMAND_STR "del"
#define MOD_COMMAND_STR "mod"
#define FIND_COMMAND_STR "find"
#define SHOW_COMMAND_STR "show"
#define EXIT_COMMAND_STR "exit"
#define HELP_COMMAND_STR "help"

typedef enum _command_type_t {
    add_command = 0,
    del_command,
    mod_command,
    find_command,
    show_command,
    exit_command,
    help_command,
    invalid_command,
} command_type_t;

#define SORT_PARAM_DATE "date"
#define SORT_PARAM_WORKID "id"

/////////////////////////////////////解析命令的函数/////////////////////////////////////

///> 解析用户输入单行命令
bool parse_command(const char* command, int len);

///> 解析用户输入添加命令
bool parse_add_command(const char* add_cmd, int len);

///> 解析用户输入删除命令
bool parse_del_command(const char* del_cmd, int len);

///> 解析用户输入修改命令
bool parse_mod_command(const char* mod_cmd, int len);

///> 解析用户输入查找命令
bool parse_find_command(const char* find_cmd, int len);

///> 解析用户输入全部显示命令
bool parse_show_command(const char* show_cmd, int len);

///> 解析用户输入退出命令
bool parse_exit_command(const char* exit_cmd, int len);

///> 解析用户输入帮助命令
bool parse_help_command(const char* help_cmd, int len);

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////解析命令的辅助函数/////////////////////////////////////

///> 检查输入的动作命令
command_type_t check_cmd_type(const char* command, int len);

bool check_cmd_name(const char* command, int len, char* cmd_name, int* param_index);

///> 检查输入参数
bool check_cmd_param(const char* param, int len, info_element_type* type, int* sort_flag);

///> 检查work id 是否合法
bool check_id_validity(const char* work_id_str, int len);

///> 检查date 是否合法
bool check_date_validity(const char* date_str, int len, entry_date_type_t* date);

///> 检查姓名 部门 职位 是否合法
bool check_name_dep_pos_validity(const char* src_str, int len);

///> 将参数转化为适用的结构
bool convert_cmd_param(const char* command, int param_index, int len, matched_info_type_t* matched_info);

///> 记录输入的排序索引参数匹配的类型
bool convert_param_to_sort_type(const char* buffer, int len, info_element_type* type);

///> 将输入的员工参数保存至结构体中，并记录参数类型
bool convert_param_to_info(const char* buffer, int len, employee_info_type_t* info, info_element_type type);

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////用于显示程序用法的函数//////////////////////////////////

///> 显示添加用法
void show_add_usage();

///> 显示删除用法
void show_del_usage();

///> 显示修改用法
void show_mod_usage();

///> 显示查找用法
void show_find_usage();

///> 显示遍历用法
void show_trav_usage();

///> 显示退出用法
void show_exit_usage();

///> 显示帮助用法
void show_help_usage();

///> 显示全部用法
void show_usage();

////////////////////////////////////////////////////////////////////////////////////////

#endif /* PARSE_ARGS_H */