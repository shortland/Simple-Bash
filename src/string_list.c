#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_list.h"
#include "debug.h"

string_list *string_list_new()
{
    int initial_size = 1;

    string_list *n_string_list = malloc(sizeof(string_list));

    n_string_list->size = initial_size;
    n_string_list->strings = malloc(initial_size * sizeof(char *));

    return n_string_list;
}

void string_list_direct_insert(string_list *list, int index, char *string)
{
    // now allocate space for specified string
    list->strings[index] = malloc(strlen(string) + 1);

    // initialize memory
    memset(list->strings[index], 0, strlen(string) + 1);

    // copy over contents of string
    debug("the string i'm direct inserting is: '%s', at index: %d\n", string, index);
    memcpy(list->strings[index], string, strlen(string) + 1);

    return;
}

string_list *string_list_from(char *string)
{
    // create the new string list
    string_list *list = string_list_new();

    string_list_direct_insert(list, 0, string);

    return list;
}

void string_list_push(string_list *list, char *string)
{
    int new_size = list->size + 1;
    debug("growing stringlist to size: %d\n", new_size);

    // resize the old size list size
    char **new_alloc = realloc(list->strings, new_size * sizeof(char *));
    if (new_alloc == NULL)
    {
        fprintf(stderr, "error: unable to enlarge string_list with push().\n");
        return;
    }

    list->strings = new_alloc;

    list->size++;

    debug("about to insert string: '%s', at index: %d\n", string, list->size - 1);
    string_list_direct_insert(list, list->size - 1, string);
    debug("after insertion, string is: '%s'\n", list->strings[list->size - 1]);

    return;
}

void string_list_debug(string_list *list)
{
    debug("String List Debug:\n");

    if (list == NULL)
    {
        debug("Error: String List is NULL\n");
        return;
    }

    debug("List Size: %d\n", list->size);

    for (int i = 0; i < list->size; i++)
    {
        debug("Index: %d\n", i);
        debug("V-Length: %lu\n", strlen(list->strings[i]));
        debug("Value: %s\n", list->strings[i]);
        debug("---- ----\n");
    }

    return;
}

string_list *string_list_from_delim(char *string, char *delim)
{
    char *token = NULL;
    token = strtok(string, delim);

    if (token == NULL)
    {
        // fprintf(stderr, "error: unable to parse strtok string_list_from_delim\n");
        return NULL;
    }

    debug("String list from delimiter; string: '%s', delimited: '%s'.\n", string, delim);

    string_list *list = string_list_from(token);

    while (token != NULL)
    {
        debug("the parsed token is: %s\n", token);

        if ((token = strtok(NULL, delim)) == NULL)
        {
            break;
        }

        string_list_push(list, token);
    }

    return list;
}
