/*
 * @Descripttion:
 * @version:
 * @Author: liyu
 * @Date: 2022-06-23 06:34:03
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

/* 拷贝字符串 */
#define COPY_STRING(_dst, _src, _len)   \
    do {                             \
        memset(_dst, 0, sizeof(_dst)); \
        strncpy(_dst, _src, _len);      \
        _dst[_len] = '\0';             \
    } while (0)

/* 释放内存 */
#define FREE(ptr)      \
    if (ptr != NULL) { \
        free(ptr);     \
        ptr = NULL;    \
    }

#endif /* COMMON_H */