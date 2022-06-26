/*
 * @Descripttion: 主函数，循环读取用户输入
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#include "log.h"
#include "parse_args.h"

log_level_t g_log_level = LOG_LEVEL_DEBUG;

int main(int argc, char* argv[])
{
    int i = 0;
    char* input[] = {
        "add -i 11111 -n Zhangsan -d 2022.05.07 -D ZTA -p engineer",
        "add -i 22222 -n Zhangsan -d 2021.05.07 -D ZTA -p engineer",
        "add -i 33333 -n Zhangsan -d 2020.05.07 -D ZTA -p engineer",
        "add -i 44444 -n Zhangsan -d 2019.05.07 -D ZTA -p engineer",
        "del -i 11111",
        "mod -i 22222 -n lisi",
        "find -i 33333",
        "find -n Zhangsan -s date",
        "show"
    };
    char buffer[BUFSIZ];
    int ret_val = -1;
    int size = sizeof(input) / sizeof(char*);
    LOG_INFO("size = %d", size);

    //初始化所有命令
    init_all_command();

    for (int i = 0; i < size; i++) {
        LOG_INFO("input[%d] = %s",i, input[i]);
        COPY_STRING(buffer,input[i],strlen(input[i]));
        //解析命令
        if (!process_command_request(buffer, strlen(buffer))) {
            return;
        }
    }
    clear_all_employee_info();

    return 0;
}