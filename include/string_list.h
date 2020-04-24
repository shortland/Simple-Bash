#ifndef STRING_LIST_H
#define STRING_LIST_H

/**
 * String list structure - holds the size of the string and it's related string[] array.
 */
typedef struct string_list
{
    int size;
    char **strings;
} string_list;

/**
 * Creates a new string_list. 
 * NOTE: The string at the initial index must be allocated. and the string must then be copied over.
 * 
 * @return a new string list array with length 1
 */
string_list *string_list_new();

/**
 * Directly insert & allocate space for a string at a specified index.
 * 
 * Assumes that the index has been already pre-allocated. 
 */
void string_list_direct_insert(string_list *list, int index, char *string);

/**
 * Creates a new string list - but unlike the above function, 
 * this one takes in a string as a paramter and initializes the first value of the string list with the specified string.
 */
string_list *string_list_from(char *string);

/**
 * Push a single new string obj to the list.
 * The argument string can be de-allocated (free'd) b/c this function makes a copy of it.
 */
void string_list_push(string_list *list, char *string);

/**
 * Debug-Print out the contents of the string_list data struct and all of the strings stored in its array.
 */
void string_list_debug(string_list *list);

/**
 * String list from a specified delimited
 */
string_list *string_list_from_delim(char *string, char *delim);

#endif
