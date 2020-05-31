#include "parse_command.h"

string_list *parse_command_to_string_list(char *command) {
    if (command == NULL) {
        return NULL;
    }

    return string_list_from_delim(command, " ");
}

/**
 * Returns whether or not the string_list command has the '&' character - 
 * implying that it should be a background job.
 * 
 * @returns
 * -1 if it doesn't
 * 1 if it does
 */
int __has_bg_flag(string_list *command) {
    for (int i = 0; i < command->size; i++) {
        if (strcmp(command->strings[i], BACKGROUND_KEY) == 0) {
            return 1;
        }
    }

    return -1;
}

/**
 * Determine the output redirect file for the command
 * TODO: get jsut the file name and remove the arrow
 * @returns paramters 
 */
char *__get_output_redirect(string_list *command) {
    char *pos;
    for (int i = 0; i < command->size; i++) {
        if (strstr(command->strings[i], OUTPUT_REDIRECT_KEY) != NULL && strstr(command->strings[i], OUTPUT_ERROR_REDIRECT_KEY) == NULL) {
            debug("found output redirect key");

            return strstr(command->strings[i], OUTPUT_REDIRECT_KEY) + 1;
        }
    }

    return NULL;
}

/**
 * Determine the output redirect file for the command
 * TODO: get jsut the file name and remove the arrow
 * @returns paramters 
 */
char *__get_output_error_redirect(string_list *command) {
    for (int i = 0; i < command->size; i++) {
        if (strstr(command->strings[i], OUTPUT_ERROR_REDIRECT_KEY) != NULL) {
            debug("found output redirect key");

            return strstr(command->strings[i], OUTPUT_ERROR_REDIRECT_KEY) + 2;
        }
    }

    return NULL;
}

/**
 * Determine the input redirect file for the command
 * TODO: get just the file name and remove the arrow
 * @returns paramters 
 */
char *__get_input_redirect(string_list *command) {
    for (int i = 0; i < command->size; i++) {
        if (strstr(command->strings[i], INPUT_REDIRECT_KEY) != NULL) {
            debug("found input redirect key");

            return strstr(command->strings[i], INPUT_REDIRECT_KEY) + 1;
        }
    }

    return NULL;
}

/**
 * Get the command (first parameter - at least currently) of the string list.
 * @returns binary executable (command) string value
 */
char *__get_binary_name(string_list *command) {
    return strdup(command->strings[0]);
}

typedef struct bin_param {
    int num;
    char **params;
    int exited;
} bin_param;

/**
 * Get the command's parameters - basically don't include the binary name (first param)
 * and ignore input and output redirection params and the '&' bg key
 */
bin_param *__get_binary_params(string_list *command) {
    bin_param *bp = malloc(sizeof(bin_param));
    bp->num = 0;
    bp->exited = 0;

    if (command->size == 1) {
        return bp;
    }

    // space to hold at least one parameter
    bp->params = malloc(1 * sizeof(char *));

    int i;
    for (i = 1; i < command->size; i++) {
        if (strstr(command->strings[i], INPUT_REDIRECT_KEY) != NULL || strstr(command->strings[i], OUTPUT_REDIRECT_KEY) != NULL || strcmp(command->strings[i], BACKGROUND_KEY) == 0) {
            // debug("found ending key... no longer looking for parameters\n");
            // debug("note: this assumes that an input/output redirect and background(&) would be at the end of the paramter list\n");

            bp->num = i - 1;

            return bp;
        }

        debug("found a valid parameter of: '%s'\n", command->strings[i]);

        if (i > 1) {
            bp->params = realloc(bp->params, (1 + i) * sizeof(char *));
        }

        /**
         * Determine whether the command parameter is actually a variable we need to parse.
         */
        char *real_arg = command->strings[i];
        if (command->strings[i][0] == VARIABLE_START_KEY) {
            if (strcmp(real_arg, LAST_RETURN_KEY) == 0) {
                debug("should replace last return key\n");
                sprintf(real_arg, "%d", get_last_return_value());
            } else {
                debug("this parameter: '%s', is a variable\n", real_arg);
                parse_path_debug_env_variables();

                if ((real_arg = parse_path_get_env(strchr(command->strings[i], VARIABLE_START_KEY) + 1)) == NULL) {
                    real_arg = command->strings[i];
                }

                debug("this variable: '%s', is now a value\n", real_arg);
            }
        }

        bp->params[i - 1] = malloc(strlen(real_arg) + 1);
        memcpy(bp->params[i - 1], real_arg, strlen(real_arg) + 1);
    }

    bp->num = i - 1;

    return bp;
}

commander *parse_command_from_string_list(string_list *command) {
    if (command == NULL) {
        return NULL;
    }

    commander *cmd = malloc(sizeof(commander));

    cmd->job_id = jobs++;
    cmd->bgfg = __has_bg_flag(command);
    cmd->started = -1;
    cmd->finished = -1;
    cmd->running = -1;
    cmd->exit_code = -1;  // set it later if finished == 1 set exit code. or get it only when finished == 1 too.
    cmd->raw_command = malloc(sizeof(string_list));
    memcpy(cmd->raw_command, command, sizeof(string_list));

    cmd->bin_dir = NULL;  // set in executor
    cmd->bin = __get_binary_name(command);

    bin_param *param = __get_binary_params(command);
    if (param->num == 0) {
        // cmd->bin_params = ;
        cmd->num_bin_params = 0;
    } else {
        cmd->bin_params = param->params;
        cmd->num_bin_params = param->num;
    }

    cmd->output_redirect = __get_output_redirect(command);
    cmd->output_error_redirect = __get_output_error_redirect(command);
    cmd->input_redirect = __get_input_redirect(command);

    return cmd;
}

void parse_command_debug_commander(commander *cmd) {
    if (cmd == NULL) {
        debug("commander was null.\n");

        return;
    }

    debug2("cmd; job_id: '%d'.\n", cmd->job_id);
    debug2("cmd; bgfg: '%d'. (-1 fg; 1 bg)\n", cmd->bgfg);
    debug2("cmd; started: '%d'. (-1 no, 1 yes)\n", cmd->started);
    debug2("cmd; finished: '%d'. (-1 no, 1 yes)\n", cmd->finished);
    debug2("cmd; running: '%d'. (-1 no, 1 yes)\n", cmd->running);
    debug2("cmd; exit_code: '%d'. (-1 default)\n", cmd->exit_code);
    debug2("cmd; raw_command: ...\n");
    string_list_debug(cmd->raw_command);
    debug2("cmd; bin_dir: '%s'\n", cmd->bin_dir);
    debug2("cmd; bin: '%s'\n", cmd->bin);
    debug2("cmd; num_bin_params: '%d'\n", cmd->num_bin_params);
    debug2("cmd; bin_params: ...\n");
    pointer_pointer_debug(cmd->bin_params, cmd->num_bin_params);
    debug2("cmd; output_redirect: '%s'\n", cmd->output_redirect);
    debug2("cmd; output_error_redirect: '%s'\n", cmd->output_error_redirect);
    debug2("cmd; input_redirect: '%s'\n", cmd->input_redirect);

    return;
}
