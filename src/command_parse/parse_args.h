/*
 * @Descripttion: 解析输入参数
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "command_def.h"
#include "employee_mgr.h"
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

///> 全局的命令信息
command_info_t g_cmd_info[MAX_COMMAND];

///> 初始化所有命令
void init_all_command();

///> 处理用户输入请求信息
bool process_command_request(const char* command, int len);

#endif /* PARSE_ARGS_H */