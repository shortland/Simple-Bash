#include "pointer_pointer_helper.h"

/**
 * Merges two arrays,
 * Must supply the lengths of each of the arrays
 */
char **pointer_pointer_merge(char **ptr1, int len1, char **ptr2, int len2) {
    char **merged = malloc(sizeof(char **) * (len1 + len2));
    debug("size total of new pp merge is: %d\n", (len1 + len2));

    int c = 0;

    for (int i = 0; i < len1; i++) {
        merged[c] = malloc(strlen(ptr1[i]) + 1);
        memcpy(merged[c], ptr1[i], strlen(ptr1[i]));
        merged[c][strlen(ptr1[i])] = '\0';

        c++;
    }

    for (int i = 0; i < len2; i++) {
        merged[c] = malloc(strlen(ptr2[i]) + 1);
        memcpy(merged[c], ptr2[i], strlen(ptr2[i]));
        merged[c][strlen(ptr2[i])] = '\0';

        c++;
    }

    return merged;
}

/**
 * Returns the length of a null delimited array
 */
int __len_pp_null(char **array) {
    int i = 0;

    if (array == NULL) {
        fprintf(stderr, "error: unable to get length of null delimited array. was already null array.");
        return -1;
    }

    while (array[i] != NULL) {
        ++i;
    }

    return i;
}

/**
 * Duplicates a null delimited array (retains null delim)
 */
char **pointer_pointer_dup(char **array) {
    char **allocd;
    int length;

    if ((length = __len_pp_null(array)) == -1) {
        fprintf(stderr, "error: unable to get size of null delim array\n");
        return NULL;
    }

    if ((allocd = malloc(sizeof(char **) * (length + 1))) == NULL) {
        fprintf(stderr, "error: unable to allocate space for pointer dupe\n");
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        allocd[i] = malloc(strlen(array[i]) + 1);

        memcpy(allocd[i], array[i], strlen(array[i]));

        allocd[i][strlen(array[i])] = '\0';
    }

    allocd[length] = malloc(sizeof(char));
    allocd[length][0] = '\0';

    return allocd;
}

/**
 * Debug prints out the contents of an array, given a specified length
 */
void pointer_pointer_debug(char **array, int length) {
    if (array == NULL) {
        debug2("pp: array given was null\n");
        return;
    }

    if (length == 0 || length < -1) {
        return;
    }

    debug2("pp: printing out contents of pointer pointer\n");

    /** When given -1, will print until null value */
    if (length == -1) {
        int i = 0;

        while (array[i] != NULL) {
            debug2("pp: '%s'\n", array[i]);
            ++i;
        }
    }

    for (int i = 0; i < length; i++) {
        if (array[i] == NULL) {
            debug2("pp: prematurely stopping print of array contents... due to null value.\n");
            break;
        }

        debug2("pp: '%s'\n", array[i]);
    }

    debug2("pp: finished printing pp\n");

    return;
}
