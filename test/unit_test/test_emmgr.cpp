/*
 * @Descripttion:覆盖测试员工信息管理类函数的边界条件,如入参为NULL等
 * @version:
 * @Author: liyu
 * @Date: 2022-06-24 23:08:22
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "employee_mgr.h"
#include "log.h"

#ifdef __cplusplus
};
#endif

#include <gtest/gtest.h>
#include <iostream>

TEST(Test_EmployeeMgr, test_employee_mgr)
{
    int count = 0;

    ///>添加员工
    ASSERT_EQ(insert_one_employee(NULL) == false, true);

    ///> 修改员工
    ASSERT_EQ(modify_one_employee_info(NULL) == false, true);

    ///> 查找员工
    EXPECT_TRUE(find_employee_by_info(NULL, &count) == NULL);
}