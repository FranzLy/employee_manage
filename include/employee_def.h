/*
 * @Descripttion: 定义员工信息必要结构
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#ifndef EMPLOYEE_DEF_H
#define EMPLOYEE_DEF_H

#define ESTABLISHED_YEAR (2000) //公司成立年份

#define MAX_EMPLOYEE_NUM (10000) //最大员工数目

#define MAX_CHAR_BUFFER_LEN (128)
#define MAX_NAME_LEN MAX_CHAR_BUFFER_LEN
#define MAX_DEPARTMENT_LEN MAX_CHAR_BUFFER_LEN
#define MAX_POSITION_LEN MAX_CHAR_BUFFER_LEN
#define WORK_ID_BUF_LEN 6

///> 日期
typedef struct _entry_date_type_t {
    int year;
    int month;
    int day;
} entry_date_type_t;

///> 员工信息
typedef struct _employee_info_type_t {
    char work_id[WORK_ID_BUF_LEN];
    char name[MAX_NAME_LEN];
    entry_date_type_t date;
    char department[MAX_DEPARTMENT_LEN];
    char position[MAX_POSITION_LEN];
} employee_info_type_t;

///> 员工信息类型
typedef enum _info_element_type {
    EMPLOYEE_INVALID = 0,
    EMPLOYEE_WORKID = 1 << 0,
    EMPLOYEE_NAME = 1 << 1,
    EMPLOYEE_DATE = 1 << 2,
    EMPLOYEE_DEPARTMENT = 1 << 3,
    EMPLOYEE_POSITION = 1 << 4,
}info_element_type;

///> 匹配信息
typedef struct _matched_info_type_t {
    employee_info_type_t info;
    info_element_type param_type;
    info_element_type sort_type;
} matched_info_type_t;

#endif /* EMPLOYEE_DEF_H */