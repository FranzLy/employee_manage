/*
 * @Descripttion: 主函数，循环读取用户输入
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#include "parse_args.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    LOG_DEBUG("Start debug mode:\n");
    
    char input[BUFSIZ];
    int ret_val = -1;

    if (argc != 1) {
        LOG_WARN("exec action: ./employee_manager ");
        exit(1);
    }

    //循环读取用户输入
    while (true) {

        memset(input, 0, BUFSIZ);
        ret_val = read(STDIN_FILENO, input, BUFSIZ);
        if (ret_val < 0) {
            LOG_ERR("Input is invalid. The error is %s", strerror(errno));
            exit(1);
        }

        if (!parse_command(input, ret_val)) {
            break;
        }
    }

    exit(0);
}