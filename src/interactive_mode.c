#include "interactive_mode.h"

int interactive_mode_run(int argc, char *argv[], string_list *bin_list, char **env_vars) {
    char *input_line = NULL;

    /**
     * Read in a line of text
     */
    while ((input_line = readline(PROMPT, fileno(stdin))) != NULL) {
        string_list *cmd = NULL;
        int executor_ret;

        debug("input read: '%s'\n", input_line);

        if ((cmd = parse_command_to_string_list(strdup(input_line))) == NULL) {
            fprintf(stdout, "\n");
            continue;
        }

        /** Debug */
        string_list_debug(cmd);

        /** Check output of executor */
        if ((executor_ret = executor_exec_command(cmd, bin_list, env_vars)) == COMMAND_RETURN_EXIT) {
            fprintf(stdout, "\n");
            return 0;
        } else if (executor_ret == COMMAND_RETURN_RETRY) {
            /** error should be given by executor */
            fprintf(stdout, "\n");
            continue;
        } else if (executor_ret == COMMAND_RETURN_NOT_FOUND) {
            fprintf(stderr, "smash: command not found: %s\n", cmd->strings[0]);
            fprintf(stdout, "\n");
            continue;
        } else if (executor_ret == COMMAND_RETURN_COMMENT) {
            fprintf(stdout, "\n");
            continue;
        } else if (executor_ret == COMMAND_RETURN_INTERNAL_CMD) {
            debug("finished executing internal command\n");
            fprintf(stdout, "\n");
            continue;
        } else {
            debug("finally just continue\n");

            /**
             * Wait for previous job to finish.
             * Must get most recently created job first... Then check if it's done.
             */
            executor_jobs *newest;
            if ((newest = executor_newest_job()) == NULL) {
                debug("there is no existing newest job\n");
            } else {
                debug("the newest job was started on: %d\n", newest->cmd->started);
                debug("waiting for job to complete before prompting\n");

                /** When a job finishes, break out of waiting and allow prompt */
                while (has_completed != 1)
                    ;

                debug("technically job done so can prompt\n");
            }

            fprintf(stdout, "\n");

            continue;
        }
    }

    return 1;
}
