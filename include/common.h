#ifndef COMMON_H
#define COMMON_H

typedef enum _log_level_t {
    LOG_LEVEL_ERR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} log_level_t;

/*
 *@brief 当前日志等级
 */
#define LOG_LEVEL LOG_LEVEL_INFO

#define LOG_INFO(fmt, args...)                                                   \
    do {                                                                         \
        if (LOG_LEVEL >= LOG_LEVEL_ERR) {                                        \
            (void)printf("[INFO][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                        \
    } while (0)

#define LOG_WARN(fmt, args...)                                                   \
    do {                                                                         \
        if (LOG_LEVEL >= LOG_LEVEL_WARN){                                                                        \
            (void)printf("[WARN][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                        \
    } while (0)

#define LOG_ERR(fmt, args...)                                                     \
    do {                                                                          \
        if (LOG_LEVEL >= LOG_LEVEL_ERR){                                                                         \
            (void)printf("[ERROR][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                         \
    } while (0)

#define LOG_DEBUG(fmt, args...)                                                   \
    do {                                                                          \
        if (LOG_LEVEL >= LOG_LEVEL_DEBUG){                                                                         \
            (void)printf("[DEBUG][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                         \
    } while (0)

#endif
