#include "interactive_mode.h"

int interactive_mode_run(int argc, char *argv[], string_list *bin_list) {
    /**
     * Read in a line of text
     */
    char *input_line;
    while ((input_line = readline(PROMPT, fileno(stdin))) != NULL) {
        debug("input read: '%s'\n", input_line);

        string_list *cmd = parse_command_to_string_list(strdup(input_line));
        if (cmd == NULL) {
            continue;
        }
        string_list_debug(cmd);

        int executor_ret = executor_exec_command(cmd, bin_list);
        if (executor_ret == COMMAND_RETURN_EXIT) {
            return 0;
        } else if (executor_ret == COMMAND_RETURN_RETRY) {
            // error should be given by executor
            continue;
        } else if (executor_ret == COMMAND_RETURN_NOT_FOUND) {
            fprintf(stderr, "smash: command not found: %s\n", cmd->strings[0]);
            continue;
        } else if (executor_ret == COMMAND_RETURN_COMMENT) {
            continue;
        } else if (executor_ret == COMMAND_RETURN_INTERNAL_CMD) {
            debug("finished executing internal command\n");
            continue;
        }
    }

    return 1;
}
