#include <stdlib.h>
#include <string.h>

#include "pointer_pointer_helper.h"
#include "debug.h"

void pointer_pointer_debug(char **array, int length)
{
    if (length <= 0)
    {
        return;
    }

    for (int i = 0; i < length; i++)
    {
        debug("pp: '%s'\n", array[i]);
    }
}

char **pointer_pointer_merge(char **ptr1, int len1, char **ptr2, int len2)
{
    char **merged = malloc(sizeof(char **) * (len1 + len2));
    debug("size total of new pp merge is: %d\n", (len1 + len2));

    int c = 0;

    for (int i = 0; i < len1; i++)
    {
        merged[c] = malloc(strlen(ptr1[i]) + 1);
        memcpy(merged[c], ptr1[i], strlen(ptr1[i]));

        c++;
    }

    for (int i = 0; i < len2; i++)
    {
        merged[c] = malloc(strlen(ptr2[i]) + 1);
        memcpy(merged[c], ptr2[i], strlen(ptr2[i]));

        c++;
    }

    return merged;
}
