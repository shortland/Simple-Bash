#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "debug.h"
#include "executor.h"
#include "parse_path.h"

volatile sig_atomic_t has_completed;

void signals_job_done(int sig);

int signals_readline_operator();

#endif
