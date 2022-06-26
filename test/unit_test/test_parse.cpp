/*
 * @Descripttion: 命令解析单测
 * @version:
 * @Author: liyu
 * @Date: 2022-06-24 00:35:58
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "log.h"
#include "parse_args.h"
#include <string.h>

#ifdef __cplusplus
};
#endif

#include <gtest/gtest.h>
#include <iostream>

class Test_Cli : public ::testing::Test {
protected:
    Test_Cli() { }

    virtual void SetUp()
    {
        std::cout << "Test_Cli setup" << std::endl;
    }

    virtual void TearDown()
    {
        std::cout << "Test_Cli teardown" << std::endl;
    }
};

TEST_F(Test_Cli, test_parse_command)
{
    char user_input[BUFSIZ] = { 0 };

    ///> 初始化所有命令
    init_all_command();

    ///> 测试错误输入
    // 1. 不存在的命令
    ASSERT_FALSE(process_command_request(NULL, 0));
    snprintf(user_input, BUFSIZ, "%s\n", "netstat");
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));
    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "addd");
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));
    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "ADD");
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    // 2. 命令格式不正确
    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add"); //缺少参数
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -p"); //缺少参数
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111"); //缺少参数
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -n zhangsan"); //缺少参数
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -i 22222"); //参数重复
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -n zhangsan -n lisi"); //参数重复
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -d 2019.02.02 -d 2019.02.10"); //参数重复
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -D ZTA -D CWPP"); //参数重复
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -p engineer -p QR"); //参数重复
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 1234a"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 111 -n zhangsan"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n _1234sfs"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n zhangsan -s date"); //参数错误:添加时不允许排序
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n zhangsan -d 2019.02.29"); //参数错误:不存在的日期
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n zhangsan -d 2019.02.29.a"); //参数错误:不存在的日期
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n zhangsan -d 1999.1.1"); //参数错误:非法日期
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n zhangsan -d 2022.11.11"); //参数错误:非法日期
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "del -n zhangsan"); //参数错误:删除仅可指定工号
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "del -i 11111 -s date"); //参数错误:删除时不允许排序
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "mod -n zhangsan"); //参数错误:修改需指定工号
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "mod -i 11111 -s date"); //参数错误:修改时不允许排序
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -l nothing"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -s name"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "show -n zhangsan"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "show -s date"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "show -s id"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "show -l DEBUG"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "help -n zhangsan"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "help -s date"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "help -l DEBUG"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "exit -n zhangsan"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "exit -s date"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "exit -l DEBUG"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "log -n zhangsan"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "log -s date"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "log -l"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -n zhangsan -s date -s id"); //参数错误
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "a b c d e f g h i j k l m n o p q r s t"); //参数过多
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    ///> 测试正确输入
    // 1.换行符
    ASSERT_TRUE(process_command_request("\n", 1));

    // 2.添加命令
    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -i 11111");//查找不存在的员工:失败
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -n zhangsan");//查找不存在的员工:失败
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "del -i 11111");//删除不存在的员工:失败
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n zhangsan -d 2022.02.10");//添加员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 11111 -n zhangsan -d 2022.02.10");//添加已存在的员工:失败
    ASSERT_FALSE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 22222 -n zhangsan -d 2021.02.10");//添加员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 33333 -n zhangsan -d 2022.03.10");//添加员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 01245 -n zhangsan -d 2022.03.11");//添加员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "add -i 95270 -n zhaowu");//添加员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "mod -i 95270 -n lisi -d 2017.03.27 -D ZTA -p Engineer");//修改员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -i 11111");//查找员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -n zhangsan -s date");//过滤查找员工并排序:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -n zhangsan -s id");//过滤查找员工并排序:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -n zhangsan");//按姓名过滤查找员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -d 2022.03.11");//按日期过滤查找员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -D ZTA");//按部门过滤查找员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "find -p Engineer");//按职位过滤查找员工:成功
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "log -l DEBUG");//设置日志等级
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "help");//帮助命令
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "show");//遍历
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input)));

    /* bzero(user_input, BUFSIZ);
    snprintf(user_input, BUFSIZ, "%s\n", "exit");
    ASSERT_TRUE(process_command_request(user_input, strlen(user_input))); */

    clear_all_employee_info();//清理员工信息
}
