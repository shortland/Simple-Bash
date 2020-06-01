#include "debug.h"

static int debug_on = 0;

void debug_enable() {
    debug_on = 1;
}

int debug(const char *str, ...) {
    va_list arg;
    int num;

    if (debug_on == 0) {
        return -1;
    }

    va_start(arg, str);

    fprintf(stderr, "\033[1;31mDEBUG: ");
    num = vfprintf(stderr, str, arg);
    fprintf(stderr, "\033[0m");

    va_end(arg);

    fflush(stderr);

    return num;
}
