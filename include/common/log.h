/*
 * @Descripttion: 日志
 * @version:
 * @Author: liyu
 * @Date: 2022-06-15 05:27:18
 */
#ifndef LOG_H
#define LOG_H

/*
 *@brief 日志等级
 */
typedef enum _log_level_t {
    LOG_LEVEL_ERR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_MAX,
} log_level_t;

/*
 *@brief 判断日志等级是否有效
 */
#define IS_VALID_LOG_LEVEL(log_level)  (log_level >= LOG_LEVEL_ERR && log_level < LOG_LEVEL_MAX)

/*
 *@brief 当前日志等级
 */
extern log_level_t g_log_level;

/* 信息日志*/
#define LOG_INFO(fmt, args...)                                                   \
    do {                                                                         \
        if (g_log_level >= LOG_LEVEL_INFO) {                                     \
            (void)printf("[INFO][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                        \
    } while (0)

/* 告警日志 */
#define LOG_WARN(fmt, args...)                                                   \
    do {                                                                         \
        if (g_log_level >= LOG_LEVEL_WARN) {                                     \
            (void)printf("[WARN][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                        \
    } while (0)

/* 错误日志 */
#define LOG_ERR(fmt, args...)                                                     \
    do {                                                                          \
        if (g_log_level >= LOG_LEVEL_ERR) {                                       \
            (void)printf("[ERROR][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                         \
    } while (0)

/* 调试日志 */
#define LOG_DEBUG(fmt, args...)                                                   \
    do {                                                                          \
        if (g_log_level >= LOG_LEVEL_DEBUG) {                                     \
            (void)printf("[DEBUG][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                         \
    } while (0)

#endif /* LOG_H */
