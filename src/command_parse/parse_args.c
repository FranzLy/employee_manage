/*
 * @Descripttion: 解析输入参数
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#include "parse_args.h"
#include "common.h"
#include "log.h"

#include <time.h>

/////////////////////////////////////解析命令的辅助函数/////////////////////////////////////
/*
 *@brief 检查work id 是否合法
 *@param [in]work_id_str 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool check_id_validity(const char* work_id_str, int len)
{
    //工号要求5位数
    if (len != WORK_ID_BUF_LEN - 1) {
        LOG_WARN("Input work id must be like 07406 meanning length is %d.", WORK_ID_BUF_LEN - 1);
        return false;
    }

    //纯数字
    for (int i = 0; i < len; i++) {
        if (isdigit(work_id_str[i]) == 0) {
            LOG_WARN("Input work id is invalid.");
            return false;
        }
    }

    return true;
}

/*
 *@brief 检查date 是否合法
 *@param [in]year month day 年月日
 *@return true 成功 false 失败
 */
static bool check_date_validity_helper(int year, int month, int day)
{
    int month_buf[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //月份修正表
    time_t cur_time;
    struct tm* p_time;

    //获取当前时间
    time(&cur_time);
    p_time = gmtime(&cur_time);

    //判断是否早于公司成立年份或超过当前年份
    if (year < ESTABLISHED_YEAR || year > (1900 + p_time->tm_year)) {
        LOG_WARN("Date info is invalid. Reason: year is invalid.");
        return false;
    }

    //闰年2月+1天
    if (month == 2) {
        (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) ? month_buf[1] += 1 : month_buf[1];
    }

    //判断月份日期是否合法
    if (month > 12 || month < 1 || day > month_buf[month - 1] || day < 1) {
        LOG_WARN("Date info is invalid. Reason: month or day is invalid.");
        return false;
    }

    //判断是否超过当前日期
    if (year == (1900 + p_time->tm_year)) {
        if ((month > (1 + p_time->tm_mon)) || (month == (1 + p_time->tm_mon) && day > p_time->tm_mday)) {
            LOG_WARN("Date info is invalid. Reason: month or day is over current date.");
            return false;
        }
    }

    return true;
}

/*
 *@brief 检查date 是否合法
 *@param [in]date_str 参数命令
 *@param [in]len     参数长度
 *@param [out]date   保存的日期
 *@return true 成功 false 失败
 */
static bool check_date_validity(const char* date_str, int len, entry_date_type_t* date)
{
    int i, start_index = 0;
    int spot_cnt = 0;
    int year, month, day;
    char buffer[16] = { 0 };

    //获取日期年月日
    for (i = 0; i < len; i++) {
        if (date_str[i] == '.') {
            spot_cnt++;

            if (spot_cnt == 1) {
                COPY_STRING(buffer,date_str , i );
                year = atoi(buffer);
                start_index = i + 1;
                continue;
            }

            if (spot_cnt == 2) {
                COPY_STRING(buffer,date_str + start_index, i - start_index);
                month = atoi(buffer);
                start_index = i + 1;
                continue;
            }
        }

        if (isdigit(date_str[i]) == 0) {
            LOG_WARN("Input date %s is invalid.", date_str);
            return false;
        }
    }
    COPY_STRING(buffer,date_str + start_index, i - start_index);
    day = atoi(buffer);

    //检查日期合法性
    if (!check_date_validity_helper(year, month, day)) {
        return false;
    }

    //保存
    date->year = year;
    date->month = month;
    date->day = day;

    return true;
}

/*
 *@brief 检查姓名 部门 职位 是否合法
 *@param [in]src_str 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool check_str_validity(const char* src_str, int len) // str
{
    //要求全英文
    for (int i = 0; i < len; i++) {
        if (isalpha(src_str[i]) == 0) {
            LOG_WARN("Employee name、department or position must be English character.");
            return false;
        }
    }

    return true;
}

/*
 *@brief 转化日志参数
 *@param [in]buffer     参数命令
 *@param [in]len        参数长度
 *@param [out]log_level 日志等级
 *@return true 成功 false 失败
 */
static bool convert_param_to_log(const char* buffer, int len, log_level_t* log_level)
{
    bool ret = false;
    int i = g_log_level;
    // LOG_LEVEL_ERR = 0,LOG_LEVEL_WARN = 1,LOG_LEVEL_INFO =2 ,LOG_LEVEL_DEBUG =3
    char* log_strs[] = { LOG_ERR_STR, LOG_WARN_STR, LOG_INFO_STR, LOG_DEBUG_STR };

    if (buffer == NULL || len == 0) {
        goto out;
    }

    for (i = LOG_LEVEL_ERR; i < LOG_LEVEL_MAX; i++) {
        if (strlen(log_strs[i]) == len && strncmp(buffer, log_strs[i], len) == 0) {
            break;
        }
    }

    if (i == LOG_LEVEL_MAX) {
        *log_level = g_log_level;
        ret = false;
    } else {
        *log_level = i;
        ret = true;
    }

out:
    return ret;
}

/*
 *@brief 记录输入的排序索引参数匹配的类型
 *@param [in]buffer 参数命令
 *@param [in]len     参数长度
 *@param [out]type     排序类型
 *@return true 成功 false 失败
 */
static bool convert_param_to_sort_type(const char* buffer, int len, sort_type_t* type)
{
    bool ret = false;

    //目前仅支持日期和工号排序
    if (len == strlen(SORT_PARAM_WORKID) && (strncmp(buffer, SORT_PARAM_WORKID, len) == 0)) {
        (*type) |= SORT_BY_ID;
        ret = true;
    } else if (len == strlen(SORT_PARAM_DATE) && (strncmp(buffer, SORT_PARAM_DATE, len) == 0)) {
        (*type) |= SORT_BY_DATE;
        ret = true;
    }

    return ret;
}

/*
 *@brief 将输入的员工参数保存至结构体中，并记录参数类型
 *@param [in]buffer 参数命令
 *@param [in]len     参数长度
 *@param [out]request   保存的结构体
 *@param [in]type     参数类型
 *@return true 成功 false 失败
 */
static bool convert_param_to_info(const char* buffer, int len, request_info_t* request, employee_attr_type_t type)
{
    bool ret = false;

    switch (type) {
    case EMPLOYEE_WORKID: //工号
        if (request->info.attr_type & EMPLOYEE_WORKID) {
            LOG_ERR("Cannot set same work id over one time!");
            ret = false;
            break;
        }
        (request->info.attr_type) |= EMPLOYEE_WORKID;

        //检查合法性并赋值
        ret = check_id_validity(buffer, len);
        if (!ret) {
            break;
        }
        COPY_STRING(request->info.work_id,buffer,len);
        break;
    case EMPLOYEE_DATE:
        if (request->info.attr_type & EMPLOYEE_DATE) {
            LOG_ERR("Cannot set same date over one time!");
            ret = false;
            break;
        }
        (request->info.attr_type) |= EMPLOYEE_DATE;

        //检查合法性并赋值
        ret = check_date_validity(buffer, len, &(request->info.date));
        break;
    case EMPLOYEE_NAME: //姓名
    case EMPLOYEE_DEPARTMENT: //部门
    case EMPLOYEE_POSITION: //职位
        if (request->info.attr_type & type) {
            LOG_ERR("Cannot set same name over one time!");
            ret = false;
            break;
        }
        (request->info.attr_type) |= type;

        //检查合法性并赋值
        ret = check_str_validity(buffer, len);
        if (!ret) {
            break;
        }
        if (type == EMPLOYEE_NAME) {
            COPY_STRING(request->info.name,buffer,len);
        } else if (type == EMPLOYEE_DEPARTMENT) {
            COPY_STRING(request->info.department,buffer,len);
        } else {
            COPY_STRING(request->info.position,buffer,len);
        }
        break;
    default:
        break;
    }

    return ret;
}

/*
 *@brief 转化命令的所有参数
 *@param [in]param_cnt 参数个数
 *@param [in]param_val     参数值
 *@param [out]request   保存的结构体
 *@return true 成功 false 失败
 */
static bool convert_cmd_all_params(int param_cnt, char** param_val, request_info_t* request)
{
    bool ret = true;
    int opt;
    const char* optstring = "i:n:d:D:p:s:l:"; //

    optind = 1; //令全局变量optind=1,使得getopt从数组的index=1处开始检索
    while ((opt = getopt(param_cnt, param_val, optstring)) != -1) {
        switch (opt) {
        case 'i': // work id
            LOG_DEBUG("opt is i, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), request, EMPLOYEE_WORKID);
            if (!ret) {
                goto out;
            }

            break;
        case 'n': // name
            LOG_DEBUG("opt is n, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), request, EMPLOYEE_NAME);
            if (!ret) {
                goto out;
            }

            break;
        case 'd': // date
            LOG_DEBUG("opt is d, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), request, EMPLOYEE_DATE);
            if (!ret) {
                goto out;
            }

            break;
        case 'D': // department
            LOG_DEBUG("opt is D, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), request, EMPLOYEE_DEPARTMENT);
            if (!ret) {
                goto out;
            }

            break;
        case 'p': // position
            LOG_DEBUG("opt is p, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), request, EMPLOYEE_POSITION);
            if (!ret) {
                goto out;
            }

            break;
        case 's': // sort
            LOG_DEBUG("opt is s, oprarg is: %s\n", optarg);
            ret = convert_param_to_sort_type(optarg, strlen(optarg), &(request->sort_type));
            if (!ret) {
                goto out;
            }
            break;
        case 'l':
            LOG_DEBUG("opt is l, oprarg is: %s\n", optarg);
            ret = convert_param_to_log(optarg, strlen(optarg), &(request->log_level));
            if (!ret) {
                goto out;
            }
            break;
        case '?': // invalid param
            ret = false;
            LOG_ERR("error optopt: %c\n", optopt);
            LOG_ERR("error opterr: %d\n", opterr);
            break;
        }
    }

out:
    return ret;
}

/*
 *@brief 获取命令类型
 *@param [in]command 参数命令
 *@return 命令类型
 */
static command_type_t get_command_type(char* command)
{
    LOG_DEBUG("command = %s", command);
    command_type_t ret = INVALID_COMMAND;

    //在全局命令中匹配类型,若匹配上,则转化相应参数
    for (int i = INVALID_COMMAND + 1; i < MAX_COMMAND; i++) {
        if (strlen(command) == strlen(g_cmd_info[i].name)
            && strncmp(command, g_cmd_info[i].name, strlen(command)) == 0) {
            ret = (command_type_t)i;
            break;
        }
    }

    return ret;
}

/*
 *@brief 分割输入
 *@details 此函数内部会为param_val动态申请内存,使用者需手动释放
 *@param [in]command 参数命令
 *@param [in]len 参数长度
 *@param [in]param_val 保存参数
 *@return 参数个数,-1代表参数过多
 */
static int split_input_args(const char* command, int len, char** param_val)
{
    int ret = 0, param_cnt = 0;
    char *out_str = NULL, *tmp_str = NULL, *dim = "\' \'\'\t\'\'\n\'"; //分隔符为空格或制表符或换行符

    //分割参数
    out_str = strtok_r((char*)command, dim, &tmp_str);
    while (out_str != NULL) {
        if (param_cnt >= MAX_PARAM_NUM) {
            LOG_ERR("Input too many params.");
            param_cnt = -1;
            break;
        }

        //保存参数值
        param_val[param_cnt] = (char*)malloc(MAX_CHAR_BUFFER_LEN);
        if (param_val[param_cnt] == NULL) {
            LOG_ERR("malloc for param_val[%d] failed.", param_cnt);
            param_cnt = 0;
            break;
        };
        COPY_STRING(param_val[param_cnt],out_str,strlen(out_str));
        LOG_DEBUG("param_val[%d]=%s", param_cnt, param_val[param_cnt]);

        //分割出下一个字符串
        out_str = strtok_r(NULL, dim, &tmp_str);
        param_cnt++;
    }
    LOG_DEBUG("param_cnt =%d", param_cnt);

    ret = param_cnt;
    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////用于显示程序用法的函数//////////////////////////////////

///> 显示添加用法
static void show_add_usage()
{
    LOG_INFO("--------------------Add one employee info:---------------------------");
    LOG_INFO("\t add -i work_id -n name [-d date] [-D department] [-p position]");
    LOG_INFO("Examples: ");
    LOG_INFO("\t add -i 00001 -n Zhangsan -d 2022.05.11 -D development -p engineer ");
    LOG_INFO("\t add -i 00001 -n Zhangsan ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示删除用法
static void show_del_usage()
{
    LOG_INFO("--------------------Delete one employee info(by work_id):------------");
    LOG_INFO("\t del - i work_id ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t del -i 00001 ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示修改用法
static void show_mod_usage()
{
    LOG_INFO("--------------------Modify one employee info(except work_id):--------");

    LOG_INFO("\t mod - i work_id[-n name][-d date][-D department][-p position] ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t mod -i 00001 -n Lisi ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示查找用法
static void show_find_usage()
{
    LOG_INFO("-------------------Find one employee info(also can sort):-------");
    LOG_INFO("\t find [-i work_id] [- n name] [-s date] [-s id] ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t find -i 00001 ");
    LOG_INFO("\t find -n Zhangsan ");
    LOG_INFO("\t find -n Zhangsan -d 2021.06.30 ");
    LOG_INFO("\t find -n Zhangsan -s date ");
    LOG_INFO("\t find -n Zhangsan -s id ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示遍历用法
static void show_trav_usage()
{
    LOG_INFO("------------------Show all employees info:-------------------- ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t show ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示遍历用法
static void show_log_usage()
{
    LOG_INFO("------------------Change log mode [mode is ERR=0 WARN=1 INFO=2 DEBUG=3]:-------------------- ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t log -l INFO ");
    LOG_INFO("\t log -l DEBUG ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示退出用法
static void show_exit_usage()
{
    LOG_INFO("------------------Exit the program----------------------------------- ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t exit");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示帮助用法
static void show_help_usage()
{
    LOG_INFO("------------------Use help command----------------------------------- ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t help");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示全部用法
static void show_usage()
{
    LOG_INFO("employee_manager usage:");
    show_add_usage();
    show_del_usage();
    show_mod_usage();
    show_find_usage();
    show_trav_usage();
    show_help_usage();
    show_exit_usage();
}

/////////////////////////////////////解析命令的函数///////////////////////////////////

/*
 *@brief 解析用户输入添加命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 */
static bool process_add_command(const request_info_t* request)
{
    bool ret = false;

    //添加员工信息不允许排序
    if (request->sort_type != SORT_BY_NONE) {
        LOG_WARN("Add employee info cannot sort.");
        show_add_usage();
        goto out;
    }

    //必须包含id和name
    if (((request->info.attr_type & EMPLOYEE_WORKID) == 0) || ((request->info.attr_type & EMPLOYEE_NAME)) == 0) {
        LOG_WARN("Add employee info must input work id and name.");
        show_add_usage();
        goto out;
    }

    //插入员工信息
    ret = insert_one_employee(&(request->info));
    if (!ret) {
        LOG_ERR("Add one employee info failed.");
        show_add_usage();
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入删除命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 */
static bool process_del_command(const request_info_t* request)
{
    bool ret = false;

    //删除员工信息不允许排序
    if (request->sort_type != SORT_BY_NONE) {
        LOG_WARN("Delete employee info cannot sort.");
        show_del_usage();
        goto out;
    }

    //删除员工信息必须仅包含work id
    if (request->info.attr_type != EMPLOYEE_WORKID) {
        LOG_WARN("Delete employee info only need work id.");
        show_del_usage();
        goto out;
    }

    //删除员工信息
    ret = delete_one_employee(atoi(request->info.work_id));
    if (!ret) {
        LOG_ERR("Delete one employee info failed.");
        show_del_usage();
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入修改命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 */
static bool process_mod_command(const request_info_t* request)
{
    bool ret = false;

    //修改员工信息不允许排序
    if (request->sort_type != SORT_BY_NONE) {
        LOG_WARN("Modify employee info cannot sort.");
        show_mod_usage();
        goto out;
    }

    //必须包含work id 和其他的一项参数
    if ((((request->info.attr_type) & EMPLOYEE_WORKID) == 0) || (((request->info.attr_type) & ~EMPLOYEE_WORKID) == 0)) {
        LOG_WARN("Modify employee info must input work id and one other employee info.");
        show_mod_usage();
        goto out;
    }

    //修改员工信息
    ret = modify_one_employee_info(&(request->info));
    if (!ret) {
        LOG_ERR("Modify one employee info failed.");
        show_mod_usage();
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入查找命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 */
static bool process_find_command(const request_info_t* request)
{
    bool ret = false;
    employee_info_t **matched_infoes = NULL, *info = NULL;
    int matched_cnt = 0;

    //仅支持work id或 date排序
    LOG_INFO("sort type = %d", request->sort_type);
    if (request->sort_type == (EMPLOYEE_WORKID | EMPLOYEE_DATE)) {
        LOG_WARN("Only support one sort index.");
        show_find_usage();
        goto out;
    }

    //若查找条件中包含员工id,则直接利用id查找
    if (request->info.attr_type & EMPLOYEE_WORKID) {
        info = find_employee_by_id(atoi(request->info.work_id));
        if (info == NULL) {
            LOG_ERR("Find one employee info by work id failed.");
            show_find_usage();
            goto out;
        }
        show_one_employee_info(info);
        ret = true;
        goto out;
    }

    //查找
    matched_infoes = find_employee_by_info(&(request->info), &matched_cnt);
    LOG_INFO("Matched employee number = %d", matched_cnt);
    if (matched_infoes == NULL) {
        LOG_ERR("Find one employee info by type failed.");
        show_find_usage();
        goto out;
    }
    sort_matched_info(matched_infoes, matched_cnt, request->sort_type);

filter:
    show_matched_employee_info(matched_infoes, matched_cnt);
    FREE(matched_infoes);
    ret = true;
out:
    return ret;
}

/*
 *@brief 解析用户输入全部显示命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 */
static bool process_show_command(const request_info_t* request)
{
    bool ret = false;
    employee_info_t** matched_info = NULL;
    int matched_cnt = 0;

    // show不需要其他参数
    if (request->info.attr_type != EMPLOYEE_INVALID
        || request->sort_type != SORT_BY_NONE
        || IS_VALID_LOG_LEVEL(request->log_level)) {
        LOG_WARN("Wrong command. If you want to show all the employee info, just input 'show'.");
        show_trav_usage();
        goto out;
    }

    //遍历员工信息
    matched_info = traverse_employee_info(&matched_cnt);
    if (matched_info == NULL) {
        LOG_ERR("Show all employee info failed.");
        show_trav_usage();
    }

    //显示
    LOG_INFO("Matched employee number = %d", matched_cnt);
    show_matched_employee_info(matched_info, matched_cnt);
    ret = true;

    //释放
    FREE(matched_info);

out:
    return ret;
}

/*
 *@brief 解析用户输入全部日志命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 */
static bool process_log_command(const request_info_t* request)
{
    bool ret = false;

    // log 不需要其他参数
    if (request->info.attr_type != EMPLOYEE_INVALID || request->sort_type != SORT_BY_NONE) {
        LOG_WARN("Wrong command. If you want to change log level, just input log.");
        show_log_usage();
        goto out;
    }

    //设置当前日志等级
    g_log_level = request->log_level;
    ret = true;
    LOG_INFO("Current log level is %d", g_log_level);

out:
    return ret;
}

/*
 *@brief 解析用户输入退出命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 */
static bool process_exit_command(const request_info_t* request)
{
    bool ret = false;

    // exit 不需要其他参数
    if (request->info.attr_type != EMPLOYEE_INVALID
        || request->sort_type != SORT_BY_NONE
        || IS_VALID_LOG_LEVEL(request->log_level)) {
        LOG_WARN("Wrong command. If you want to exit the program, just input exit.");
        show_exit_usage();
        goto out;
    }

    //清理员工信息,然后退出
    LOG_INFO("Exit the program with your command.");
    clear_all_employee_info(s_hash_table);
    exit(0);

out:
    return ret;
}

/*
 *@brief 解析用户输入帮助命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 *@return true 成功 false 失败
 */
static bool process_help_command(const request_info_t* request)
{
    bool ret = false;

    // help 不需要其他参数
    if (request->info.attr_type != EMPLOYEE_INVALID
        || request->sort_type != SORT_BY_NONE
        || IS_VALID_LOG_LEVEL(request->log_level)) {
        LOG_WARN("Wrong command. If you want to get help, just input help.");
        show_help_usage();
        goto out;
    }

    // 输出所有用法
    ret = true;
    show_usage();

out:
    return ret;
}

/*
 *@brief 初始化所有命令
 */
void init_all_command()
{
    memset(g_cmd_info, 0, sizeof(g_cmd_info));

    //增
    INIT_COMMAND(g_cmd_info, ADD_COMMAND, ADD_COMMAND_STR, process_add_command);

    //删
    INIT_COMMAND(g_cmd_info, DEL_COMMAND, DEL_COMMAND_STR, process_del_command);

    //改
    INIT_COMMAND(g_cmd_info, MOD_COMMAND, MOD_COMMAND_STR, process_mod_command);

    //查
    INIT_COMMAND(g_cmd_info, FIND_COMMAND, FIND_COMMAND_STR, process_find_command);

    //遍历
    INIT_COMMAND(g_cmd_info, SHOW_COMMAND, SHOW_COMMAND_STR, process_show_command);

    //帮助
    INIT_COMMAND(g_cmd_info, HELP_COMMAND, HELP_COMMAND_STR, process_help_command);

    //日志
    INIT_COMMAND(g_cmd_info, LOG_COMMAND, LOG_COMMAND_STR, process_log_command);

    //退出
    INIT_COMMAND(g_cmd_info, EXIT_COMMAND, EXIT_COMMAND_STR, process_exit_command);
}

/*
 *@brief 解析用户输入单行命令
 *@param [in]request 用户输入信息
 *@return true 成功 false 失败
 *@return true 成功 false 失败
 */
bool process_command_request(const char* command, int len)
{
    bool ret = false;
    command_info_t cmd_info = { 0 };
    char* param_val[MAX_PARAM_NUM] = { 0 };
    int param_cnt = 0;
    command_type_t cmd_type;

    //参数判断
    if (command == NULL) {
        LOG_WARN("Input content is NULL.");
        goto out;
    }

    //为输入内容请求空间
    cmd_info.request = (request_info_t*)malloc(sizeof(request_info_t));
    if (!cmd_info.request) {
        LOG_ERR("Malloc for cmd_info.request failed.");
        goto out;
    }
    memset(cmd_info.request, 0, sizeof(request_info_t));
    cmd_info.request->info.attr_type = EMPLOYEE_INVALID;
    cmd_info.request->sort_type = SORT_BY_NONE;
    cmd_info.request->log_level = LOG_LEVEL_MAX;

    //分割用户输入字符串
    param_cnt = split_input_args(command, len, param_val);
    if (param_cnt == 0) { //用户未输入有效字符,例如直接换行,则直接返回
        ret = true;
        goto out;
    } else if (param_cnt == -1) { //用户输入参数过多
        ret = false;
        goto out;
    }

    //判断参数类型
    if ((cmd_type = get_command_type(param_val[0])) == INVALID_COMMAND) {
        ret = false;
        goto out;
    }

    //解析命令参数
    ret = convert_cmd_all_params(param_cnt, param_val, cmd_info.request);
    if (!ret) {
        LOG_DEBUG("Input invalid params!");
        goto out;
    }

    //按命令类型执行相应的回调函数
    ret = g_cmd_info[cmd_type].exec(cmd_info.request);

out:
    for (int i = 0; i < param_cnt; i++) {
        FREE(param_val[i]);
    }
    FREE(cmd_info.request);
    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
