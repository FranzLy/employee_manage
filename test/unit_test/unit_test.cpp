/*
 * @Descripttion: 单元测试文件
 * @version:
 * @Author: liyu
 * @Date: 2022-06-18 19:30:48
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "log.h"

#ifdef __cplusplus
};
#endif
#include <gtest/gtest.h>

log_level_t g_log_level = LOG_LEVEL_DEBUG;

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}