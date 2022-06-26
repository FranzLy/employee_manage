/*
 * @Descripttion: Cli命令解析相关结构
 * @version:
 * @Author: liyu
 * @Date: 2022-06-23 06:53:09
 */
#ifndef command_DEF_H
#define command_DEF_H

#include "employee_info.h"
#include "log.h"
#include <stdbool.h>
//#include <stdio.h>

///> 最大参数个数为16,包含员工各类信息以及排序
#define MAX_PARAM_NUM (16)

///> 命令字符串
#define ADD_COMMAND_STR "add"
#define DEL_COMMAND_STR "del"
#define MOD_COMMAND_STR "mod"
#define FIND_COMMAND_STR "find"
#define SHOW_COMMAND_STR "show"
#define EXIT_COMMAND_STR "exit"
#define HELP_COMMAND_STR "help"
#define LOG_COMMAND_STR "log"

///> 排序参数字符串
#define SORT_PARAM_DATE "date"
#define SORT_PARAM_WORKID "id"

///> 日志等级字符串
#define LOG_ERR_STR "ERR"
#define LOG_WARN_STR "WARN"
#define LOG_INFO_STR "INFO"
#define LOG_DEBUG_STR "DEBUG"

///> 命令类型
typedef enum _command_type_t {
    INVALID_COMMAND = 0, //无效命令

    ADD_COMMAND, //增
    DEL_COMMAND, //删
    MOD_COMMAND, //改
    FIND_COMMAND, //查
    SHOW_COMMAND, //遍历

    EXIT_COMMAND, //退出
    HELP_COMMAND, //帮助
    LOG_COMMAND, //日志

    MAX_COMMAND, //最大命令个数
} command_type_t;

///> 排序类型
typedef enum _sort_type_t {
    SORT_BY_NONE, //不排序
    SORT_BY_ID, //按工号排序
    SORT_BY_DATE, //按日期排序
} sort_type_t;

///> 请求信息
typedef struct _request_info_t {
    command_type_t command; //指令类型
    employee_info_t info; //员工信息
    sort_type_t sort_type; //排序类型
    log_level_t log_level;
} request_info_t;

///> 命令的执行函数指针
typedef bool (*command_exec_func)(const request_info_t* request);

///> 命令信息
typedef struct _command_info_t {
    char* name;
    request_info_t* request;
    command_exec_func exec;
} command_info_t;

#define INIT_COMMAND(_cmd_info, _cmd_type, _name, _exec) \
    do {                                                 \
        _cmd_info[_cmd_type].name = _name;                 \
        _cmd_info[_cmd_type].exec = _exec;                 \
        _cmd_info[_cmd_type].request = NULL;               \
    } while (false)

#endif /* command_DEF_H */