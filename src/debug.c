#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "debug.h"

static int debug_on = 0;

void debug_enable()
{
    debug_on = 1;
}

int debug(const char *str, ...)
{
    int num;
    va_list arg;

    va_start(arg, str);
    fprintf(stderr, "\033[1;31m");
    num = vfprintf(stderr, str, arg);
    fprintf(stderr, "\033[0m");
    va_end(arg);

    return num;
}
