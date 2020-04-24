#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "string_list.h"

int executor_exec_command(string_list *command, string_list *bin_list);

#endif
