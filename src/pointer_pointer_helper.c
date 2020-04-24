#include <stdlib.h>

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
