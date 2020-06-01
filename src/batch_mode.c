#include "batch_mode.h"

int batch_mode_run(char *filename, string_list *bin_list, char **env_list) {
    char *input_line = NULL;
    FILE *in_file = NULL;

    if ((in_file = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "error: unable to open file for reading\n");
        return 1;
    }

    /**
     * Read in a line of text
     */
    while ((input_line = readline(PROMPT, fileno(in_file))) != NULL) {
        string_list *cmd = NULL;
        int executor_ret;

        debug("input read: '%s'\n", input_line);

        if ((cmd = parse_command_to_string_list(strdup(input_line))) == NULL) {
            continue;
        }

        /** Debug */
        string_list_debug(cmd);

        /** Check output of executor */
        if ((executor_ret = executor_exec_command(cmd, bin_list, env_list)) == COMMAND_RETURN_EXIT) {
            return 0;
        } else if (executor_ret == COMMAND_RETURN_RETRY) {
            /** error should be given by executor */
            continue;
        } else if (executor_ret == COMMAND_RETURN_NOT_FOUND) {
            fprintf(stderr, "smash: command not found: %s\n", cmd->strings[0]);
            continue;
        } else if (executor_ret == COMMAND_RETURN_COMMENT) {
            continue;
        } else if (executor_ret == COMMAND_RETURN_INTERNAL_CMD) {
            debug("finished executing internal command\n");
            continue;
        } else {
            continue;
        }
    }

    return 1;
}
