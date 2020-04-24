#ifndef PARSE_PATH_H
#define PARSE_PATH_H

#include "string_list.h"

/**
 * Set of functions that are useful in parsing out the PATH variable from main's envp variable.
 * 
 * Look in this file's relative .c file for function definitions.
 */

char *parse_path_string(char *envp[], const char *env_key);

string_list *parse_path_bin_dirs(char *path_str);

#endif
