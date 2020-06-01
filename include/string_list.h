#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

/**
 * String list structure - holds the size of the string and it's related string[] array.
 */
typedef struct string_list {
    int size;
    char **strings;
} string_list;

string_list *string_list_new();

int string_list_direct_insert(string_list *list, int index, char *string);

string_list *string_list_from(char *string);

void string_list_push(string_list *list, char *string);

void string_list_debug(string_list *list);

string_list *string_list_from_delim(char *string, char *delim);

#endif
