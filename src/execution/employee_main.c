/*
 * @Descripttion: 主函数，循环读取用户输入
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#include "log.h"
#include "parse_args.h"

log_level_t g_log_level = LOG_LEVEL_INFO;

int main(int argc, char* argv[])
{
    LOG_DEBUG("Start debug mode:\n");
    char input[BUFSIZ] = { 0 };
    int ret_val = -1;

    if (argc != 1) {
        LOG_WARN("exec action: ./employee_manager ");
        exit(1);
    }

    //初始化所有命令
    init_all_command();

    //循环读取用户输入
    while (true) {
        //读取输入
        memset(input, 0, BUFSIZ);
        ret_val = read(STDIN_FILENO, input, BUFSIZ);
        LOG_DEBUG("User input = %s\n", input);
        if (ret_val < 0) {
            LOG_ERR("Input is invalid. The error is %s", strerror(errno));
            exit(1);
        }

        //解析命令
        if (!process_command_request(input, ret_val)) {
            return;
        }
    }

    exit(0);
}
