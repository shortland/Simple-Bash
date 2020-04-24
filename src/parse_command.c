#include <stdlib.h>

#include "parse_command.h"
#include "string_list.h"

string_list *parse_command_to_string_list(char *command)
{
    if (command == NULL)
    {
        return NULL;
    }

    return string_list_from_delim(command, " ");
}
