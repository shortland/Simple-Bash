#include "string_list.h"

/**
 * Creates a new string_list. 
 * NOTE: The string at the initial index must be allocated. and the string must then be copied over.
 * 
 * @return a new string list array with length 1
 */
string_list *string_list_new() {
    string_list *n_string_list = NULL;
    int initial_size = 1;

    if ((n_string_list = malloc(sizeof(string_list))) == NULL) {
        debug("error: unable to allocate space for n_string_list\n");
        return NULL;
    }

    n_string_list->size = initial_size;

    if ((n_string_list->strings = malloc(initial_size * sizeof(char *))) == NULL) {
        debug("error: unable to allocate space for string list strings\n");
        return NULL;
    }

    return n_string_list;
}

/**
 * Directly insert & allocate space for a string at a specified index.
 * 
 * Assumes that the index has been already pre-allocated. 
 */
int string_list_direct_insert(string_list *list, int index, char *string) {
    if ((list->strings[index] = malloc(strlen(string) + 1)) == NULL) {
        debug("error: unable to allocate space for list strings indices\n");
        return -1;
    }

    /** initialize memory */
    memset(list->strings[index], 0, strlen(string) + 1);

    /** copy over contents of string */
    debug2("the string i'm direct inserting is: '%s', at index: %d\n", string, index);
    memcpy(list->strings[index], string, strlen(string) + 1);

    return 0;
}

/**
 * Creates a new string list - but unlike the above function, 
 * this one takes in a string as a paramter and initializes the first value of the string list with the specified string.
 */
string_list *string_list_from(char *string) {
    string_list *list = NULL;

    if ((list = string_list_new()) == NULL) {
        return NULL;
    }

    if (string_list_direct_insert(list, 0, string) != 0) {
        return NULL;
    }

    return list;
}

/**
 * Push a single new string obj to the list.
 * The argument string can be de-allocated (free'd) b/c this function makes a copy of it.
 */
void string_list_push(string_list *list, char *string) {
    int new_size = list->size + 1;
    char **new_alloc = NULL;

    debug2("growing stringlist to size: %d\n", new_size);

    /** resize the old size list size */
    if ((new_alloc = realloc(list->strings, new_size * sizeof(char *))) == NULL) {
        fprintf(stderr, "error: unable to reallocate space for new_alloc\n");
        return;
    }

    if (new_alloc == NULL) {
        fprintf(stderr, "error: unable to enlarge string_list with push().\n");
        return;
    }

    list->strings = new_alloc;
    list->size++;

    debug2("about to insert string: '%s', at index: %d\n", string, list->size - 1);

    if (string_list_direct_insert(list, list->size - 1, string) != 0) {
        return;
    }

    debug2("after insertion, string is: '%s'\n", list->strings[list->size - 1]);

    return;
}

/**
 * Debug-Print out the contents of the string_list data struct and all of the strings stored in its array.
 */
void string_list_debug(string_list *list) {
    debug2("String List Debug:\n");

    if (list == NULL) {
        debug2("Error: String List is NULL\n");
        return;
    }

    debug("List Size: %d\n", list->size);

    for (int i = 0; i < list->size; i++) {
        debug2("Index: %d\n", i);
        debug2("V-Length: %lu\n", strlen(list->strings[i]));
        debug2("Value: %s\n", list->strings[i]);
        debug2("---- / END STRING_LIST_DEBUG() / ----\n");
    }

    return;
}

/**
 * String list from a specified delimited
 */
string_list *string_list_from_delim(char *string, char *delim) {
    string_list *list = NULL;
    char *token = NULL;

    if ((token = strtok(string, delim)) == NULL) {
        return NULL;
    }

    debug("String list from delimiter; string: '%s', delimited: '%s'.\n", string, delim);

    if ((list = string_list_from(token)) == NULL) {
        return NULL;
    }

    while (token != NULL) {
        debug("the parsed token is: %s\n", token);

        if ((token = strtok(NULL, delim)) == NULL) {
            break;
        }

        string_list_push(list, token);
    }

    return list;
}
