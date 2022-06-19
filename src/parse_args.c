/*
 * @Descripttion: 解析输入参数
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#include "parse_args.h"
#include <time.h>

/////////////////////////////////////解析命令的辅助函数/////////////////////////////////////

/*
 *@brief 判断命令动作类型
 *@param [in]command    命令字符串
 *@param [in]len        命令长度
 */
static command_type_t check_cmd_type(const char* command, int len)
{
    command_type_t type = invalid_command;

    // add
    if (len == strlen(ADD_COMMAND_STR) && strncmp(command, ADD_COMMAND_STR, len) == 0) {
        type = add_command;
        goto out;
    }

    // del
    if (len == strlen(DEL_COMMAND_STR) && strncmp(command, DEL_COMMAND_STR, len) == 0) {
        type = del_command;
        goto out;
    }

    // mod
    if (len == strlen(MOD_COMMAND_STR) && strncmp(command, MOD_COMMAND_STR, len) == 0) {
        type = mod_command;
        goto out;
    }

    // find
    if (len == strlen(FIND_COMMAND_STR) && strncmp(command, FIND_COMMAND_STR, len) == 0) {
        type = find_command;
        goto out;
    }

    // show
    if (len == strlen(SHOW_COMMAND_STR) && strncmp(command, SHOW_COMMAND_STR, len) == 0) {
        type = show_command;
        goto out;
    }

    // exit
    if (len == strlen(EXIT_COMMAND_STR) && strncmp(command, EXIT_COMMAND_STR, len) == 0) {
        type = exit_command;
        goto out;
    }

    // help
    if (len == strlen(HELP_COMMAND_STR) && strncmp(command, HELP_COMMAND_STR, len) == 0) {
        type = help_command;
        goto out;
    }

out:
    return type;
}

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
        if (work_id_str[i] < '0' || work_id_str[i] > '9') {
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
                strncpy(buffer, date_str, i);
                buffer[i] = '\0';
                year = atoi(buffer);
                start_index = i + 1;
                continue;
            }

            if (spot_cnt == 2) {
                memset(buffer, 0, sizeof(buffer));
                strncpy(buffer, date_str + start_index, i - start_index);
                buffer[i - start_index] = '\0';
                month = atoi(buffer);
                start_index = i + 1;
                continue;
            }
        }

        if (date_str[i] < '0' || date_str[i] > '9') {
            LOG_WARN("Input date %s is invalid.", date_str);
            return false;
        }
    }
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, date_str + start_index, i - start_index);
    buffer[i - start_index] = '\0';
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
static bool check_name_dep_pos_validity(const char* src_str, int len)
{
    //要求全英文
    for (int i = 0; i < len; i++) {
        if (!((src_str[i] >= 'a' && src_str[i] <= 'z') || (src_str[i] >= 'A' && src_str[i] <= 'Z'))) {
            LOG_WARN("Employee name、department or position must be English character.");
            return false;
        }
    }

    return true;
}

/*
 *@brief 记录输入的排序索引参数匹配的类型
 *@param [in]buffer 参数命令
 *@param [in]len     参数长度
 *@param [out]type     排序类型
 *@return true 成功 false 失败
 */
static bool convert_param_to_sort_type(const char* buffer, int len, info_element_type* type)
{
    bool ret = false;

    //目前仅支持日期和工号排序
    if (len == strlen(SORT_PARAM_WORKID) && (strncmp(buffer, SORT_PARAM_WORKID, len) == 0)) {
        (*type) |= EMPLOYEE_WORKID;
        ret = true;
    } else if (len == strlen(SORT_PARAM_DATE) && (strncmp(buffer, SORT_PARAM_DATE, len) == 0)) {
        (*type) |= EMPLOYEE_DATE;
        ret = true;
    }

    return ret;
}

/*
 *@brief 将输入的员工参数保存至结构体中，并记录参数类型
 *@param [in]buffer 参数命令
 *@param [in]len     参数长度
 *@param [out]info   保存的结构体
 *@param [in]type     参数类型
 *@return true 成功 false 失败
 */
static bool convert_param_to_info(const char* buffer, int len, matched_info_type_t* matched_info, info_element_type type)
{
    bool ret = false;

    //工号
    if (type == EMPLOYEE_WORKID) {
        if (matched_info->param_type & EMPLOYEE_WORKID) {
            LOG_ERR("Cannot set same work id over one time!");
            ret = false;
            goto out;
        }
        (matched_info->param_type) |= EMPLOYEE_WORKID;

        ret = check_id_validity(buffer, len);
        if (!ret) {
            goto out;
        }
        strncpy(matched_info->info.work_id, buffer, len);
        matched_info->info.work_id[len] = '\0';
    }

    //日期
    if (type == EMPLOYEE_DATE) {
        if (matched_info->param_type & EMPLOYEE_DATE) {
            LOG_ERR("Cannot set same date over one time!");
            ret = false;
            goto out;
        }
        (matched_info->param_type) |= EMPLOYEE_DATE;

        ret = check_date_validity(buffer, len, &(matched_info->info.date));
        if (!ret) {
            goto out;
        }
    }

    //姓名
    if (type == EMPLOYEE_NAME) {
        if (matched_info->param_type & EMPLOYEE_NAME) {
            LOG_ERR("Cannot set same name over one time!");
            ret = false;
            goto out;
        }
        (matched_info->param_type) |= EMPLOYEE_NAME;

        ret = check_name_dep_pos_validity(buffer, len);
        if (!ret) {
            goto out;
        }

        strncpy(matched_info->info.name, buffer, len);
        matched_info->info.name[len] = '\0';
    }

    //部门
    if (type == EMPLOYEE_DEPARTMENT) {
        if (matched_info->param_type & EMPLOYEE_DEPARTMENT) {
            LOG_ERR("Cannot set same department over one time!");
            ret = false;
            goto out;
        }
        (matched_info->param_type) |= EMPLOYEE_DEPARTMENT;

        ret = check_name_dep_pos_validity(buffer, len);
        if (!ret) {
            goto out;
        }

        strncpy(matched_info->info.department, buffer, len);
        matched_info->info.department[len] = '\0';
    }

    //职位
    if (type == EMPLOYEE_POSITION) {
        if (matched_info->param_type & EMPLOYEE_POSITION) {
            LOG_ERR("Cannot set same position over one time!");
            ret = false;
            goto out;
        }
        (matched_info->param_type) |= EMPLOYEE_POSITION;

        ret = check_name_dep_pos_validity(buffer, len);
        if (!ret) {
            goto out;
        }
        strncpy(matched_info->info.position, buffer, len);
        matched_info->info.position[len] = '\0';
    }

out:
    return ret;
}

static bool convert_cmd_param_helper(int param_cnt, char* param_val, matched_info_type_t* matched_info)
{
    bool ret = false;
    int opt;
    const char* optstring = "i:n:d:D:p:s:"; //

    optind = 1; //令全局变量optind=1,使得getopt从数组的index=1处开始检索
    while ((opt = getopt(param_cnt, param_val, optstring)) != -1) {
        switch (opt) {
        case 'i': // work id
            LOG_DEBUG("opt is i, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), matched_info, EMPLOYEE_WORKID);
            if (!ret) {
                goto out;
            }

            break;
        case 'n': // name
            LOG_DEBUG("opt is n, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), matched_info, EMPLOYEE_NAME);
            if (!ret) {
                goto out;
            }

            break;
        case 'd': // date
            LOG_DEBUG("opt is d, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), matched_info, EMPLOYEE_DATE);
            if (!ret) {
                goto out;
            }

            break;
        case 'D': // department
            LOG_DEBUG("opt is D, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), matched_info, EMPLOYEE_DEPARTMENT);
            if (!ret) {
                goto out;
            }

            break;
        case 'p': // position
            LOG_DEBUG("opt is p, oprarg is: %s\n", optarg);

            ret = convert_param_to_info(optarg, strlen(optarg), matched_info, EMPLOYEE_POSITION);
            if (!ret) {
                goto out;
            }

            break;
        case 's': // sort
            LOG_DEBUG("opt is s, oprarg is: %s\n", optarg);
            ret = convert_param_to_sort_type(optarg, strlen(optarg), &(matched_info->sort_type));
            if (!ret) {
                goto out;
            }
            break;
        case '?': // invalid param
            LOG_ERR("error optopt: %c\n", optopt);
            LOG_ERR("error opterr: %d\n", opterr);
            break;
        }
    }

out:
    return ret;
}

/*
 *@brief 将参数转化为适用的结构
 *@param [in]command 参数命令
 *@param [in]param_index     起始位置
 *@param [in]len     参数长度
 *@param [out]matched_info     匹配结果
 *@return true 成功 false 失败
 */
static bool convert_cmd_param(const char* command, int param_index, int len, matched_info_type_t* matched_info)
{
    bool ret = false;
    char *out_str = NULL, *tmp_str = NULL, *dim = "\' \'\'\t\'\'\n\'"; //分隔符为空格或制表符或换行符
    int param_cnt = 1; // getopt函数的optind=，会从数组的index=1处开始检索
    char* param_val[2 * MAX_PARAM_NUM + 1] = { 0 }; //最大输入参数和值的个数

    //判空
    if (matched_info == NULL) {
        LOG_ERR("Input param is NULL.");
        goto out;
    }

    //分割参数
    out_str = strtok_r((char*)command, dim, &tmp_str);
    while (out_str != NULL) {
        if (param_cnt == (2 * MAX_PARAM_NUM + 1)) {
            LOG_ERR("Input too many params.");
            goto out;
        }

        //确保有每种参数类型都确有值输入，例如-i 后必须跟work id
        if (((param_cnt % 2) != 0) && (out_str[0] != '-')) {
            LOG_ERR("Input too many values for one param type.");
            goto out;
        }

        //保存参数值
        param_val[param_cnt] = (char*)malloc(MAX_CHAR_BUFFER_LEN);
        if (param_val[param_cnt] == NULL) {
            LOG_ERR("malloc for param_val[%d] failed.", param_cnt);
        };
        memset(param_val[param_cnt], 0, MAX_CHAR_BUFFER_LEN);
        strncpy(param_val[param_cnt], out_str, strlen(out_str));
        LOG_DEBUG("param_val[%d]=%s", param_cnt, param_val[param_cnt]);

        //分割出下一个字符串
        out_str = strtok_r(NULL, dim, &tmp_str);
        param_cnt++;
    }

    //转换参数
    ret = convert_cmd_param_helper(param_cnt, param_val, matched_info);
    if (!ret) {
        goto out;
    }

out:
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
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool parse_add_command(const char* add_cmd, int len)
{
    int i = 0, param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    //找到参数的命令跟着的参数的起始位置，以'-'为参数起始标记
    for (; i < len; i++) {
        if (add_cmd[i] == '-') {
            param_index = i;
            break;
        }
    }

    //转化参数
    ret = convert_cmd_param(add_cmd, param_index, len, &matched_info);
    if (!ret) {
        LOG_WARN("Too few parameters for adding info as a employee.");
        show_add_usage();
        goto out;
    }

    //添加员工信息不允许排序
    if (matched_info.sort_type != 0) {
        LOG_WARN("Add employee info cannot sort.");
        ret = false;
        goto out;
    }

    //必须包含id和name
    if (((matched_info.param_type & EMPLOYEE_WORKID) == 0) || ((matched_info.param_type & EMPLOYEE_NAME)) == 0) {
        LOG_WARN("Add employee info must input work id and name.");
        ret = false;
        goto out;
    }

    //插入员工信息
    ret = insert_one_employee(&(matched_info.info));
    if (!ret) {
        LOG_ERR("Add one employee info failed.");
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入删除命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool parse_del_command(const char* del_cmd, int len)
{
    int i = 0, param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    //找到参数的命令跟着的参数的起始位置，以'-'为参数起始标记
    for (; i < len; i++) {
        if (del_cmd[i] == '-') {
            param_index = i;
            break;
        }
    }

    //转化参数
    ret = convert_cmd_param(del_cmd, param_index, len, &matched_info);
    if (!ret) {
        LOG_WARN("Too few parameters for deleting info.");
        show_del_usage();
        goto out;
    }

    //删除员工信息不允许排序
    if (matched_info.sort_type != 0) {
        LOG_WARN("Delete employee info cannot sort.");
        ret = false;
        goto out;
    }

    //删除员工信息必须仅包含work id
    if (matched_info.param_type != EMPLOYEE_WORKID) {
        LOG_WARN("Delete employee info only need work id.");
        ret = false;
        goto out;
    }

    //删除员工信息
    ret = delete_one_employee_by_id(matched_info.info.work_id);
    if (!ret) {
        LOG_ERR("Delete one employee info failed.");
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入修改命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool parse_mod_command(const char* mod_cmd, int len)
{
    int i = 0, param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    //找到参数的命令跟着的参数的起始位置，以'-'为参数起始标记
    for (; i < len; i++) {
        if (mod_cmd[i] == '-') {
            param_index = i;
            break;
        }
    }

    //转化参数
    ret = convert_cmd_param(mod_cmd, param_index, len, &matched_info);
    if (!ret) {
        LOG_WARN("Too few parameters for modifying info.");
        show_mod_usage();
        goto out;
    }

    //修改员工信息不允许排序
    if (matched_info.sort_type != 0) {
        LOG_WARN("Modify employee info cannot sort.");
        ret = false;
        goto out;
    }

    //必须包好work id 和其他的一项参数
    if ((((matched_info.param_type) & EMPLOYEE_WORKID) == 0) || (((matched_info.param_type) & ~EMPLOYEE_WORKID) == 0)) {
        LOG_WARN("Modify employee info must input work id and one other employee info.");
        ret = false;
        goto out;
    }

    //修改员工信息
    ret = modify_one_employee_info(&(matched_info.info), matched_info.param_type);
    if (!ret) {
        LOG_ERR("Modify one employee info failed.");
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入查找命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool parse_find_command(const char* find_cmd, int len)
{
    int i = 0, param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    //找到参数的命令跟着的参数的起始位置，以'-'为参数起始标记
    for (; i < len; i++) {
        if (find_cmd[i] == '-') {
            param_index = i;
            break;
        }
    }

    //转化参数
    ret = convert_cmd_param(find_cmd, param_index, len, &matched_info);
    if (!ret) {
        LOG_WARN("Too few parameters for finding info.");
        show_find_usage();
        goto out;
    }

    //仅支持work id或 date排序
    if (matched_info.sort_type == (EMPLOYEE_WORKID | EMPLOYEE_DATE)) {
        LOG_WARN("Only support one sort index.");
        ret = false;
        goto out;
    }

    //查找
    ret = find_employee_by_type(&matched_info);
    if (!ret) {
        LOG_ERR("Find one employee info by type failed.");
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入全部显示命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool parse_show_command(const char* show_cmd, int len)
{
    bool ret = false;
    int i = 0;

    //检查输入命令尾部，不需有参数
    for (; i < len; i++) {
        if (!(show_cmd[i] == ' ' || show_cmd[i] == '\t' || show_cmd[i] == '\n')) {
            LOG_WARN("Wrong command. If you want to show all the info, just input show.");
            ret = false;
            show_trav_usage();
            goto out;
        }
    }

    //遍历员工信息
    ret = show_all_employee_info();
    if (!ret) {
        LOG_ERR("Show all employee info failed.");
    }

out:
    return ret;
}

/*
 *@brief 解析用户输入退出命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool parse_exit_command(const char* exit_cmd, int len)
{
    int i = 0;
    bool ret = false;

    //检查输入命令尾部，不需有参数
    for (; i < len; i++) {
        if (!(exit_cmd[i] == ' ' || exit_cmd[i] == '\t' || exit_cmd[i] == '\n')) {
            LOG_WARN("Wrong command. If you want to exit the program, just input exit.");
            ret = false;
            show_exit_usage();
            goto out;
        }
    }

    LOG_INFO("Exit the program with your command.");
    exit(0);

out:
    return ret;
}

/*
 *@brief 解析用户输入帮助命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
static bool parse_help_command(const char* help_cmd, int len)
{
    int i = 0;
    bool ret = false;

    //检查输入命令尾部，不需有参数
    for (; i < len; i++) {
        if (!(help_cmd[i] == ' ' || help_cmd[i] == '\t' || help_cmd[i] == '\n')) {
            ret = false;
            LOG_WARN("Wrong command. If you want to get help, just input help.");
            show_help_usage();
            goto out;
        }
    }

    ret = true;
    show_usage();

out:
    return ret;
}

/*
 *@brief 解析用户输入单行命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
bool parse_command(const char* command, int len)
{
    char cmd_name[MAX_CHAR_BUFFER_LEN] = { 0 };
    bool ret = false;
    int i = 0, start_index = 0;
    command_type_t cmd_type = invalid_command;

    //参数判断
    if (command == NULL) {
        LOG_WARN("Input content is NULL.");
        return ret;
    }

    //跳过前面的空白字符
    for (; i < len; i++) {
        if (!(command[i] == ' ' || command[i] == '\t' || command[i] == '\n')) {
            break;
        }
    }
    if (i == len) {
        ret = true;
        return ret;
    }

    //解析命令参数
    start_index = i;
    for (i = start_index + 1; i < len; i++) {
        if (command[i] == ' ' || command[i] == '\t' || command[i] == '\n') {
            break;
        }
    }
    LOG_DEBUG(" start_index = %d, i = %d \n", start_index, i);
    strncpy(cmd_name, command + start_index, i - start_index);
    cmd_name[i - start_index] = '\0';
    LOG_DEBUG("input command = %s\n", cmd_name);

    //获取命令类型
    cmd_type = check_cmd_type(cmd_name, strlen(cmd_name));

    //按命令分别进行处理
    switch (cmd_type) {
    case add_command:
        ret = parse_add_command(command + i, len - i);
        break;
    case del_command:
        ret = parse_del_command(command + i, len - i);
        break;
    case mod_command:
        ret = parse_mod_command(command + i, len - i);
        break;
    case find_command:
        ret = parse_find_command(command + i, len - i);
        break;
    case show_command:
        ret = parse_show_command(command + i, len - i);
        break;
    case exit_command:
        ret = parse_exit_command(command + i, len - i);
        break;
    case help_command:
        ret = parse_help_command(command + i, len - i);
        break;
    default:
        LOG_WARN("Invalid command type.");
        break;
    }

    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
