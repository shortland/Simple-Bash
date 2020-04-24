#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "debug.h"
#include "parse_path.h"
#include "string_list.h"
#include "io.h"
#include "readline.h"
#include "parse_command.h"
#include "executor.h"
#include "command_return_list.h"
#include "globals.h"
#include "signals.h"

int main(int argc, char *argv[], char *envp[])
{
    /**
     * Parses out all the environment variables for later ease of retrieval using parse_path_get_env()
     */
    parse_path_all_env_params(envp);

    char *path_string = parse_path_get_env(ENV_PATH_KEY);
    if (path_string == NULL)
    {
        fprintf(stderr, "error: unable to get path string. Is there one? - Check globals.h to configure.\n");

        return -1;
    }

    /**
     * Parse directories that have binaries
     */
    string_list *bin_list = parse_path_bin_dirs(path_string);

    /**
     * Initialize the jobs list
     * -> Initializes a list which contains currently running job info.
     * 
     * Must call before hooking readline.
     */
    executor_init_execd();

    /**
     * Set sig hook function in readline
     */
    struct sigaction sc;
    sc.sa_handler = signals_job_done;
    sc.sa_flags = 0;
    sigemptyset(&sc.sa_mask);

    if (sigaction(SIGCHLD, &sc, NULL) == -1)
    {
        debug("unable to set SIGCHLD handler\n");
        return -1;
    }

    readline_set_sig_hook(signals_readline_operator);

    /**
     * Read in a line of text
     */
    char *input_line;
    while ((input_line = readline(PROMPT)) != NULL)
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
        else if (executor_ret == COMMAND_RETURN_RETRY)
        {
            // error should be given by executor
            continue;
        }
        else if (executor_ret == COMMAND_RETURN_NOT_FOUND)
        {
            fprintf(stderr, "smash: command not found: %s\n", cmd->strings[0]);
            continue;
        }
        else if (executor_ret == COMMAND_RETURN_COMMENT)
        {
            continue;
        }
        else if (executor_ret == COMMAND_RETURN_INTERNAL_CMD)
        {
            debug("finished executing internal command\n");
            continue;
        }
    }

    return 0;
}
