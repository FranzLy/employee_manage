#include "parse_args.h"
#include <time.h>

/////////////////////////////////////解析命令的辅助函数///////////////////////////////////

/*
 *@brief 解析用户输入单行命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
bool parse_command(const char* command, int len)/* 不要用首字母 */
{
    char cmd_name[10] = { 0 };
    bool ret = false;

    //参数判断
    if (command == NULL) {
        LOG_WARN("Input content is NULL.");
        return ret;
    }

    //按命令分别进行处理
    switch (command[0]) {
    case 'a':
        ret = parse_add_command(command, len);
        break;
    case 'd':
        ret = parse_del_command(command, len);
        break;
    case 'm':
        ret = parse_mod_command(command, len);
        break;
    case 'f':
        ret = parse_find_command(command, len);
        break;
    case 's':
        ret = parse_show_command(command, len);
        break;
    case 'e':
        ret = parse_exit_command(command, len);
        break;
    case 'h':
        ret = parse_help_command(command, len);
        break;
    case '\n':
        ret = true;
        break;
    default:
        LOG_WARN("First character is no sense.");
        break;
    }

    return ret;
}

/*
 *@brief 解析用户输入添加命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
bool parse_add_command(const char* add_cmd, int len)
{
    int param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    ret = check_cmd_name(add_cmd, len, ADD_COMMAND, &param_index);
    if (!ret) {
        LOG_WARN("Wrong command. Maybe you want to input add?");
        show_add_usage();
        goto out;
    }

    if (add_cmd[param_index] != '-') {
        LOG_WARN("Too few parameters for adding info as a employee.");
        goto out;
    }

    ret = convert_cmd_param(add_cmd, param_index, len, &matched_info);
    if (!ret) {
        goto out;
    }

    if (matched_info.sort_type != 0) {
        LOG_WARN("Add employee info cannot sort.");
        ret = false;
        goto out;
    }

    if (((matched_info.param_type & EMPLOYEE_WORKID) == 0) || ((matched_info.param_type & EMPLOYEE_NAME)) == 0) {
        LOG_WARN("Add employee info must input work id and name.");
        ret = false;
        goto out;
    }

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
bool parse_del_command(const char* del_cmd, int len)
{
    int param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    ret = check_cmd_name(del_cmd, len, DEL_COMMAND, &param_index);
    if (!ret) {
        LOG_WARN("Wrong command. Maybe you want to input del?");
        show_del_usage();
        goto out;
    }

    if (del_cmd[param_index] != '-') {
        LOG_WARN("Too few parameters for deleting info.");
        goto out;
    }

    ret = convert_cmd_param(del_cmd, param_index, len, &matched_info);
    if (!ret) {
        goto out;
    }

    if (matched_info.sort_type != 0) {
        LOG_WARN("Delete employee info cannot sort.");
        ret = false;
        goto out;
    }

    if (matched_info.param_type != EMPLOYEE_WORKID) {
        LOG_WARN("Delete employee info only need work id.");
        ret = false;
        goto out;
    }

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
bool parse_mod_command(const char* mod_cmd, int len)
{
    int param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    ret = check_cmd_name(mod_cmd, len, MOD_COMMAND, &param_index);
    if (!ret) {
        LOG_WARN("Wrong command. Maybe you want to input mod?");
        show_mod_usage();
        goto out;
    }

    if (mod_cmd[param_index] != '-') {
        LOG_WARN("Too few parameters for modifying info.");
        goto out;
    }

    ret = convert_cmd_param(mod_cmd, param_index, len, &matched_info);
    if (!ret) {
        goto out;
    }

    if (matched_info.sort_type != 0) {
        LOG_WARN("Modify employee info cannot sort.");
        ret = false;
        goto out;
    }

    if ((((matched_info.param_type) & EMPLOYEE_WORKID) == 0) || (((matched_info.param_type) & ~EMPLOYEE_WORKID) == 0)) {
        LOG_WARN("Modify employee info must input work id and one other employee info.");
        ret = false;
        goto out;
    }

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
bool parse_find_command(const char* find_cmd, int len)
{
    int param_index = 0;
    bool ret = false;
    matched_info_type_t matched_info = { 0 };

    ret = check_cmd_name(find_cmd, len, FIND_COMMAND, &param_index);
    if (!ret) {
        LOG_WARN("Wrong command. Maybe you want to input find?");
        show_find_usage();
        goto out;
    }

    if (find_cmd[param_index] != '-') {
        LOG_WARN("Too few parameters for finding info.");
        goto out;
    }

    ret = convert_cmd_param(find_cmd, param_index, len, &matched_info);
    if (!ret) {
        goto out;
    }

    if (matched_info.sort_type == (EMPLOYEE_WORKID | EMPLOYEE_DATE)) {
        LOG_WARN("Only support one sort index.");
        ret = false;
        goto out;
    }

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
bool parse_show_command(const char* show_cmd, int len)
{
    int param_index = 0;
    bool ret = false;
    int i;

    ret = check_cmd_name(show_cmd, len, SHOW_COMMAND, &param_index);
    if (!ret) {
        LOG_WARN("Wrong command. Maybe you want to input show?");
        show_trav_usage();
        goto out;
    }

    for (i = param_index; i < len; i++) {
        if (!(show_cmd[i] == ' ' || show_cmd[i] == '\t' || show_cmd[i] == '\n')) {
            LOG_WARN("Wrong command. If you want to show all the info, just input show.");
            ret = false;
            show_trav_usage();
            goto out;
        }
    }

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
bool parse_exit_command(const char* exit_cmd, int len)
{
    int i, param_index = 0;
    bool ret = false;

    ret = check_cmd_name(exit_cmd, len, EXIT_COMMAND, &param_index);
    if (!ret) {
        LOG_WARN("Wrong command. Maybe you want to input exit?");
        show_exit_usage();
        goto out;
    }

    for (i = param_index; i < len; i++) {
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
bool parse_help_command(const char* help_cmd, int len)
{
    int i, param_index = 0;
    bool ret = false;

    ret = check_cmd_name(help_cmd, len, HELP_COMMAND, &param_index);
    if (!ret) {
        LOG_WARN("Wrong command. Maybe you want to input help?");
        show_help_usage();
        goto out;
    }

    for (i = param_index; i < len; i++) {
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

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////解析命令的辅助函数/////////////////////////////////////

/*
 *@brief 检查输入的动作命令
 *@param [in]command 参数命令
 *@param [in]len     参数长度
 *@param [in]cmd_name     匹配的参数
 *@param [out]param_index     匹配命令后的下一个位置
 *@return true 成功 false 失败
 */
bool check_cmd_name(const char* command, int len, char* cmd_name, int* param_index)
{
    bool ret = false;
    int i = 0;
    char buffer[MAX_CHAR_BUFFER_LEN] = { 0 };

    for (; i < len; i++) {
        if (command[i] == ' ' || command[i] == '\n') {
            break;
        }
    }
    strncpy(buffer, command, i);
    buffer[i] = '\0';

    if (i == strlen(cmd_name) && strncmp(buffer, cmd_name, strlen(cmd_name)) == 0) {
        *param_index = i + 1;
        ret = true;
    }

    return ret;
}

/*
 *@brief 检查输入参数
 *@param [in]param 参数命令
 *@param [in]len     参数长度
 *@param [out]info_element_type     参数类型
 *@param [out]sort_flag     是否排序
 *@return true 成功 false 失败
 */
bool check_cmd_param(const char* param, int len, info_element_type* elem_type, int* sort_flag)
{
    bool ret = false;
    char buffer[MAX_CHAR_BUFFER_LEN] = { 0 };

    switch (param[0]) {
    case 'i':
        if (elem_type) {
            (*elem_type) = EMPLOYEE_WORKID;
            ret = true;
        }
        break;
    case 'n':
        if (elem_type) {
            (*elem_type) = EMPLOYEE_NAME;
            ret = true;
        }
        break;
    case 'd':
        if (elem_type) {
            (*elem_type) = EMPLOYEE_DATE;
            ret = true;
        }
        break;
    case 'D':
        if (elem_type) {
            (*elem_type) = EMPLOYEE_DEPARTMENT;
            ret = true;
        }
        break;
    case 'p':
        if (elem_type) {
            (*elem_type) = EMPLOYEE_POSITION;
            ret = true;
        }
        break;
    case 's':
        if (sort_flag) {
            (*sort_flag) = 1;
            ret = true;
        }
        break;
    default:
        break;
    }

    return ret;
}

/*
 *@brief 检查work id 是否合法
 *@param [in]work_id_str 参数命令
 *@param [in]len     参数长度
 *@return true 成功 false 失败
 */
bool check_id_validity(const char* work_id_str, int len)
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
bool check_date_validity_helper(int year, int month, int day)
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
bool check_date_validity(const char* date_str, int len, entry_date_type_t* date)
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
bool check_name_dep_pos_validity(const char* src_str, int len)
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
 *@brief 将参数转化为适用的结构
 *@param [in]command 参数命令
 *@param [in]param_index     起始位置
 *@param [in]len     参数长度
 *@param [out]matched_info     匹配结果
 *@return true 成功 false 失败
 */
bool convert_cmd_param(const char* command, int param_index, int len, matched_info_type_t* matched_info)
{
    
    int space_cnt = 0, start_index = 0;
    bool param_flag = false;
    bool ret = false;
    char buffer[MAX_CHAR_BUFFER_LEN] = { 0 };
    info_element_type tmp_type = 0;

    //判空
    if(matched_info==NULL){
        LOG_ERR("Input param is NULL.");
        goto out;
    }

    //分割参数 如 -i 12345 -n Zhangsan
    for (int i = param_index; i < len; i++) {
        // '-' 代表新的参数的起始位置
        if (command[i] == '-') {
            start_index = i;
            param_flag = true;
        }

        // 记录空格
        if (command[i] == ' ' || command[i] == '\n') {
            space_cnt++;
        }

        // 参数结尾
        if ((space_cnt == 2) && param_flag) {
            int param_len = i - start_index - 2;
            int sort_flag = 0;

            //检查参数
            ret = check_cmd_param(command + start_index + 1, param_len, &tmp_type, &sort_flag);
            if (!ret) {
                LOG_INFO("Input invalid element format.");
                goto out;
            }

            //记录参数
            memset(buffer, 0, MAX_CHAR_BUFFER_LEN);
            strncpy(buffer, command + start_index + 3, param_len);
            buffer[param_len - 1] = '\0';

            //判断是否排序
            if (sort_flag == 1) {
                LOG_INFO("sort type = %s", buffer);
                ret = convert_param_to_sort_type(buffer, strlen(buffer), &(matched_info->sort_type));
                if (!ret) {
                    goto out;
                }
                sort_flag = 0;
                continue;
            }

            //转化参数
            ret = convert_param_to_info(buffer, strlen(buffer), &(matched_info->info), tmp_type);
            if (!ret) {
                goto out;
            }
            //记录参数类型
            (matched_info->param_type) |= tmp_type;

            space_cnt = 0;
            param_flag = false;
        }
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
bool convert_param_to_sort_type(const char* buffer, int len, info_element_type* type)
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
bool convert_param_to_info(const char* buffer, int len, employee_info_type_t* info, info_element_type type)
{
    bool ret = false;

    //工号
    if (type == EMPLOYEE_WORKID) {
        ret = check_id_validity(buffer, len);
        if (!ret) {
            goto out;
        }
        strncpy(info->work_id, buffer, len);
        info->work_id[len] = '\0';
    }

    //日期
    if (type == EMPLOYEE_DATE) {
        ret = check_date_validity(buffer, len, &(info->date));
        if (!ret) {
            goto out;
        }
    }

    //姓名
    if (type == EMPLOYEE_NAME) {
        ret = check_name_dep_pos_validity(buffer, len);
        if (!ret) {
            goto out;
        }

        strncpy(info->name, buffer, len);
        info->name[len] = '\0';
    }

    //部门
    if (type == EMPLOYEE_DEPARTMENT) {
        ret = check_name_dep_pos_validity(buffer, len);
        if (!ret) {
            goto out;
        }

        strncpy(info->department, buffer, len);
        info->department[len] = '\0';
    }

    //职位
    if (type == EMPLOYEE_POSITION) {
        ret = check_name_dep_pos_validity(buffer, len);
        if (!ret) {
            goto out;
        }
        strncpy(info->position, buffer, len);
        info->position[len] = '\0';
    }

out:
    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////用于显示程序用法的函数//////////////////////////////////

///> 显示添加用法
void show_add_usage()
{
    LOG_INFO("--------------------Add one employee info:---------------------------");
    LOG_INFO("\t add -i work_id -n name [-d date] [-D department] [-p position]");
    LOG_INFO("Examples: ");
    LOG_INFO("\t add -i 00001 -n Zhangsan -d 2022.05.11 -D development -p engineer ");
    LOG_INFO("\t add -i 00001 -n Zhangsan ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示删除用法
void show_del_usage()
{
    LOG_INFO("--------------------Delete one employee info(by work_id):------------");
    LOG_INFO("\t del - i work_id ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t del -i 00001 ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示修改用法
void show_mod_usage()
{
    LOG_INFO("--------------------Modify one employee info(except work_id):--------");

    LOG_INFO("\t mod - i work_id[-n name][-d date][-D department][-p position] ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t mod -i 00001 -n Lisi ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示查找用法
void show_find_usage()
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
void show_trav_usage()
{
    LOG_INFO("------------------Show all employees info:-------------------- ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t show ");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示退出用法
void show_exit_usage()
{
    LOG_INFO("------------------Exit the program----------------------------------- ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t exit");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示帮助用法
void show_help_usage()
{
    LOG_INFO("------------------Use help command----------------------------------- ");
    LOG_INFO("Examples: ");
    LOG_INFO("\t help");
    LOG_INFO("---------------------------------------------------------------------\n");
}

///> 显示全部用法
void show_usage()
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