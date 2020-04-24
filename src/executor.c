#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "executor.h"
#include "debug.h"
#include "command_list.h"
#include "command_return_list.h"
#include "parse_command.h"

void executor_exec_bin_command(char *bin_dir, string_list *command)
{
    int pipe_amt = 0;
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        /**
         * Set the process group id to the current process id.
         */
        if (setpgid(getpid(), getpid()) != 0)
        {
            debug("unable to set the process group id\n");

            return;
        }
    }

    debug("exec new commands - parent pid: %d (spawned child: %d)\n", getpid(), pid);

    return;
}

char *executor_find_binary(char *command, string_list *bin_list)
{
    if (bin_list == NULL)
    {
        fprintf(stderr, "error: no binary list available from parsed PATH dir.\n");

        return NULL;
    }

    struct dirent *de;

    for (int i = 0; i < bin_list->size; i++)
    {
        DIR *dr = opendir(bin_list->strings[i]);
        if (dr == NULL)
        {
            // fprintf(stderr, "error: could not open specified directory.\n");
            debug("error trying to open directory: '%s'\n", bin_list->strings[i]);

            return NULL;
        }

        while ((de = readdir(dr)) != NULL)
        {
            if (strcmp(de->d_name, "..") != 0 && strcmp(de->d_name, ".") != 0)
            {
                // debug("opened directory: '%s'\n", de->d_name);
                if (strcmp(de->d_name, command) == 0)
                {
                    debug("found bin_list dirent with matching bin name\n");
                    closedir(dr);

                    return bin_list->strings[i];
                }
            }
        }

        closedir(dr);
    }

    return NULL;
}

int executor_exec_command(string_list *command, string_list *bin_list)
{
    if (command == NULL)
    {
        return COMMAND_RETURN_RETRY;
    }

    /**
     * Further parse the command, into something a little more complex than just a string list.
     */
    commander *cmd = parse_command_from_string_list(command);

    /**
     * Initially, may want to hardcode some commands into the shell.
     * First check for these specified commands.
     * Then if still not found - try to find the command in one of the /bin/ dirs.
     */

    // exit the prog.
    if (strcmp(command->strings[0], COMMAND_EXIT) == 0)
    {
        return COMMAND_RETURN_EXIT;
    }

    // // change dir using chdir()
    // if (strcmp(command->strings[0], COMMAND_CD) == 0)
    // {
    //     // TODO:
    //     return COMMAND_RETURN_EXIT;
    // }

    // // show current dir using getcwd()
    // if (strcmp(command->strings[0], COMMAND_PWD) == 0)
    // {
    //     // TODO:
    //     return COMMAND_RETURN_EXIT;
    // }

    // Since not matching our commands - search in bin dirs.
    char *bin_dir;
    if ((bin_dir = executor_find_binary(command->strings[0], bin_list)) == NULL)
    {
        if (command->strings[0][0] == '#')
        {
            return COMMAND_RETURN_COMMENT;
        }

        return COMMAND_RETURN_NOT_FOUND;
    }
    else
    {
        cmd->bin_dir = bin_dir;

        debug("found binary path at: '%s'\n", bin_dir);

        /**
         * Call this after finding and setting the binary.
         */
        parse_command_debug_commander(cmd);

        executor_exec_bin_command(bin_dir, command);

        return COMMAND_RETURN_SUCCESS;
    }

    return COMMAND_RETURN_RETRY;
}
