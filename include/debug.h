#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define debug(S, ...)                                                     \
    do                                                                    \
    {                                                                     \
        fprintf(stderr, "\033[1;33mDEBUG: %s:%s:%d \033[0m " S, __FILE__, \
                __extension__ __FUNCTION__, __LINE__, ##__VA_ARGS__);     \
    } while (0)
#else
#define debug(S, ...)
#endif

#endif
