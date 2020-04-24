#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "string_list.h"
#include "parse_command.h"

typedef struct executor_jobs
{
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
