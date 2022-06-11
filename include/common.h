#ifndef COMMON_H
#define COMMON_H

#define LOG_INFO(fmt, args...)                                                   \
    do {                                                                         \
        {                                                                        \
            (void)printf("[INFO][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                        \
    } while (0)

#define LOG_WARN(fmt, args...)                                                   \
    do {                                                                         \
        {                                                                        \
            (void)printf("[WARN][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                        \
    } while (0)

#define LOG_ERR(fmt, args...)                                                   \
    do {                                                                          \
        {                                                                         \
            (void)printf("[ERROR][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                         \
    } while (0)

#define LOG_DEBUG(fmt, args...)                                                   \
    do {                                                                          \
        {                                                                         \
            (void)printf("[DEBUG][%s:%d] " fmt "\n", __func__, __LINE__, ##args); \
        }                                                                         \
    } while (0)

#endif
