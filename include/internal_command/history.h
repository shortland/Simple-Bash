#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readline.h"
#include "string_list.h"

int internal_command_history(char *home_path, char *history_file);

int internal_command_history_write(char *home_path, char *history_file, string_list *command);

#endif
