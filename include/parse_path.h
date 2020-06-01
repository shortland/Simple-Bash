#ifndef PARSE_PATH_H
#define PARSE_PATH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "globals.h"
#include "pointer_pointer_helper.h"
#include "string_list.h"

/**
 * Set of functions that are useful in parsing out the PATH variable from main's envp variable.
 * 
 * Look in this file's relative .c file for function definitions.
 */

typedef struct env_params {
    char *key;
    char *value;
} env_params;

static env_params **env_variables;

string_list *parse_path_bin_dirs(char *path_str);

char **parse_path_all_env_params(char *envp[]);

void parse_path_debug_env_variables();

char *parse_path_get_env(char *key);

int get_last_return_value();

void set_last_return_value(int code);

#endif
