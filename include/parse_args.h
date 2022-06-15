#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "employee_mgr.h"
#include <stdlib.h>
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

#define MAX_PARAM_NUM (6)//最大参数个数为6,包含员工各类信息以及排序

/////////////////////////////////////解析命令的函数/////////////////////////////////////

///> 解析用户输入单行命令
bool parse_command(const char* command, int len);

#endif /* PARSE_ARGS_H */