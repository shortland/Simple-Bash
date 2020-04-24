#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "parse_path.h"
#include "string_list.h"
#include "io.h"
#include "readline.h"
#include "parse_command.h"
#include "executor.h"
#include "command_return_list.h"

int main(int argc, char *argv[], char *envp[])
{
    char *prompt = "smash> ";
    char *path_string = parse_path_string(envp);

    debug("the path string has value: %s\n", path_string);

    if (path_string == NULL)
    {
        fprintf(stderr, "error: unable to get path string. Is there one? - Check globals.h to configure.\n");
        return -1;
    }

    /**
     * Parse directories that have binaries
     */
    string_list *bin_list = parse_path_bin_dirs(path_string);
    // string_list_debug(bin_list);
    // for (int i = 0; i < bin_list->size; i++)
    // {
    //     io_print_files_in_dir(bin_list->strings[i]);
    // }

    /**
     * Read in a line of text
     */
    char *input_line;
    while ((input_line = readline(prompt)) != NULL)
    {
        debug("input read: '%s'\n", input_line);

        string_list *cmd = parse_command_to_string_list(strdup(input_line));
        if (cmd == NULL)
        {
            continue;
        }
        // string_list_debug(cmd);

        int executor_ret = executor_exec_command(cmd, bin_list);
        if (executor_ret == COMMAND_RETURN_EXIT)
        {
            return -1;
        }
        else if (executor_ret == COMMAND_RETURN_RETRY || executor_ret == COMMAND_RETURN_NOT_FOUND)
        {
            fprintf(stderr, "smash: command not found: %s\n", cmd->strings[0]);
            continue;
        }
    }

    return 0;
}
