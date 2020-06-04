#ifndef READLINE_H
#define READLINE_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "debug.h"
#include "signals.h"

typedef int readline_sig_hook(void);

void readline_set_sig_hook(readline_sig_hook hook);

char *readline(char *prompt, int fd);

#endif
