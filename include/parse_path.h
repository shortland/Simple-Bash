#ifndef PARSE_PATH_H
#define PARSE_PATH_H

#include "string_list.h"

/**
 * Set of functions that are useful in parsing out the PATH variable from main's envp variable.
 * 
 * Look in this file's relative .c file for function definitions.
 */

typedef struct env_params
{
    char *key;
    char *value;
} env_params;

static env_params **env_variables;

// deprecated in favor of 'parse_path_get_env()'
// char *parse_path_string(char *envp[], const char *env_key);

string_list *parse_path_bin_dirs(char *path_str);

void parse_path_all_env_params(char *envp[]);

void parse_path_debug_env_variables();

char *parse_path_get_env(char *key);

int get_last_return_value();

void set_last_return_value(int code);

#endif
