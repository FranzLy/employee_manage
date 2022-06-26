/*
 * @Descripttion:
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#ifndef EMPLOYEE_INFO_H
#define EMPLOYEE_INFO_H

#define ESTABLISHED_YEAR (2000) //公司成立年份

#define MAX_EMPLOYEE_NUM (10000) //最大员工数目

#define MAX_CHAR_BUFFER_LEN (1024)
#define MAX_NAME_LEN MAX_CHAR_BUFFER_LEN
#define MAX_DEPARTMENT_LEN MAX_CHAR_BUFFER_LEN
#define MAX_POSITION_LEN MAX_CHAR_BUFFER_LEN
#define WORK_ID_BUF_LEN (6)

///> 日期
typedef struct _entry_date_type_t {
    int year;
    int month;
    int day;
} entry_date_type_t;

///> 判断日期是否为空,即未输入
#define IS_EMPTY_DATE(date) ((date.year == 0 && date.month == 0 && date.day == 0))

///> 员工信息类型
typedef enum _employee_attr_type_t {
    EMPLOYEE_INVALID = 0,
    EMPLOYEE_WORKID = 1 << 0,
    EMPLOYEE_NAME = 1 << 1,
    EMPLOYEE_DATE = 1 << 2,
    EMPLOYEE_DEPARTMENT = 1 << 3,
    EMPLOYEE_POSITION = 1 << 4,
} employee_attr_type_t;

///> 员工信息
typedef struct _employee_info_t {
    char work_id[WORK_ID_BUF_LEN];
    char name[MAX_NAME_LEN];
    entry_date_type_t date;
    char department[MAX_DEPARTMENT_LEN];
    char position[MAX_POSITION_LEN];
    employee_attr_type_t attr_type;
} employee_info_t;

#endif /* EMPLOYEE_INFO_H */