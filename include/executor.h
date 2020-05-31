#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command_return_list.h"
#include "debug.h"
#include "globals.h"
#include "parse_command.h"
#include "parse_path.h"
#include "pointer_pointer_helper.h"
#include "string_list.h"

typedef struct executor_jobs {
    commander *cmd;
    struct executor_jobs *next;
    struct executor_jobs *prev;
} executor_jobs;

/**
 * Non-circular.
 */
executor_jobs *executor_execd_head();

int executor_exec_command(string_list *command, string_list *bin_list);

void executor_init_execd();

void executor_pop_execd(int job_id);

void executor_push_execd(commander *cmd);

void executor_debug_execd();

#endif
