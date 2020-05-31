#include "executor.h"

#include "command_list.h"

static executor_jobs *execd_job_list;

executor_jobs *executor_execd_head() {
    return execd_job_list;
}

/**
 * Called once to initialize the list of running jobs.
 * Simply creates the **execd_job_list by initializing the first index to 0.
 */
void executor_init_execd() {
    execd_job_list = malloc(1 * sizeof(executor_jobs));
    execd_job_list->cmd = NULL;
    execd_job_list->next = NULL;
    execd_job_list->prev = NULL;

    return;
}

/**
 * Add a new job to the head of the job list.
 */
void executor_push_execd(commander *cmd) {
    executor_jobs *new = malloc(1 * sizeof(executor_jobs));

    // populate the new one
    new->cmd = cmd;
    new->next = execd_job_list;
    new->prev = NULL;

    // push up the old one
    execd_job_list->prev = new;

    // set the head to the new one
    execd_job_list = new;

    return;
}

/**
 * Remove an existing job from the job list.
 */
void executor_pop_execd(int job_id) {
    executor_jobs *current = execd_job_list;

    while (current->cmd != NULL) {
        if (current->cmd->job_id == job_id) {
            /** Pop the current job from the list */

            if (current->prev == NULL && current->next == NULL) {
                /** This is the initial null node */
                debug("error: this shouldn't be reachable, technically null node has no cmd, so shouldn't be able to match job_id...\n");

                return;
            }

            if (current->prev == NULL) {
                /** This is the head node (popping head) */
                debug2("found node at the head\n");
                current->next->prev = NULL;
                execd_job_list = current->next;
                free(current);

                return;
            }

            if (current->next == NULL) {
                /** This is the tail node */
                debug2("found node at the tail\n");
                current->prev->next = NULL;
                free(current);

                return;
            }

            if (current->next != NULL && current->prev != NULL) {
                /** This is a center node */
                debug2("found node at the center\n");
                current->prev->next = current->next;
                current->next->prev = current->prev;
                free(current);

                return;
            }
        }

        /** Go to next job */
        if (current->next == NULL) {
            debug("couldn't find job with that id\n");
            break;
        }

        current = current->next;
    }

    return;
}

/**
 * Execute the specified command. This'll add it to the execd_job_list
 */
void executor_exec_bin_command(commander *cmd, string_list *command) {
    pid_t pid;

    if ((pid = fork()) == 0) {
        /**
         * Set the process group id to the current process id.
         */
        if (setpgid(getpid(), getpid()) != 0) {
            debug("unable to set the process group id\n");

            return;
        }

        /**
         * Prepare the command & it's arguments
         */

        /** create the entire executable path, binary-dir + binary-name. */
        char *dest = malloc(strlen(cmd->bin_dir) + strlen(cmd->bin) + 1);
        memcpy(dest, cmd->bin_dir, strlen(cmd->bin_dir));

        /** add a '/' between the binary-dir and binary-name */
        dest[strlen(cmd->bin_dir)] = '/';
        dest[strlen(cmd->bin_dir) + 1] = 0;

        /** finally, concatenate */
        char *full_command = strcat(dest, cmd->bin);

        /** command argument prepping - put name of executable path as first argument */
        char **command_args = malloc(2 * sizeof(char **));
        command_args[0] = malloc(strlen(full_command) + 1);
        memcpy(command_args[0], full_command, strlen(full_command));
        command_args[0][strlen(full_command)] = '\0';

        /** more arguments to parse. */
        if (cmd->num_bin_params > 0) {
            // command_args = realloc(cmd->bin_params, (cmd->num_bin_params + 1) * sizeof(char *));
            command_args = pointer_pointer_merge(command_args, 1, cmd->bin_params, cmd->num_bin_params);
        }

        /** +2 b/c 1 for the initial array, and 1 more so that i can get to last index */
        debug("this: %d, should be 1 larger than the size specified by pp merge\n", (cmd->num_bin_params + 2));
        command_args = realloc(command_args, (cmd->num_bin_params + 2) * sizeof(char **));
        command_args[cmd->num_bin_params + 1] = malloc(sizeof(NULL));
        command_args[cmd->num_bin_params + 1] = NULL;

        pointer_pointer_debug(command_args, cmd->num_bin_params + 2);

        /**
         * Change input fd
         */
        char *input_path = cmd->input_redirect;
        int fd_in = -1;
        if (input_path != NULL) {
            debug("getting stdin input from a file: '%s'\n", input_path);

            if ((fd_in = open(input_path, O_RDONLY)) == -1) {
                fprintf(stderr, "error: unable to open input file\n");
                exit(errno);
            }
        }

        /**
         * The final file to output stdout data to;
         * if it's not the last command, then should pipe data to next process...
         */
        char *output_path = cmd->output_redirect;
        int fd_out = -1;
        if (output_path != NULL) {
            debug("placing stdout output to file: '%s'\n", output_path);

            if ((fd_out = open(output_path, O_WRONLY | O_TRUNC | O_CREAT, 00777)) == -1) {
                fprintf(stderr, "error: unable to open stdout output file\n");
                exit(errno);
            }
        }

        /**
         * The final file to output stderr data to;
         * if it's not the last command, then should pipe data to next process...
         */
        char *output_err_path = cmd->output_error_redirect;
        int fd_err_out = -1;
        if (output_err_path != NULL) {
            debug("placing stderr output to file: '%s'\n", output_err_path);

            if ((fd_err_out = open(output_err_path, O_WRONLY | O_TRUNC | O_CREAT, 00777)) == -1) {
                fprintf(stderr, "error: unable to open stderr output file\n");
                exit(errno);
            }
        }

        /**
         * Now replace stdin with fd_in if necessary
         */
        if (fd_in != -1) {
            if (dup2(fd_in, fileno(stdin)) == -1) {
                fprintf(stderr, "error: unable to replace infile with fd.\n");
                exit(errno);
            }

            close(fd_in);
        }

        /**
         * Replace stdout with fd_out if necessary
         */
        if (fd_out != -1) {
            if (dup2(fd_out, fileno(stdout)) == -1) {
                fprintf(stderr, "error: unable to replace outfile with fd.");
                exit(errno);
            }

            close(fd_out);
        }

        /**
         * Replace stdout with fd_out if necessary
         */
        if (fd_err_out != -1) {
            if (dup2(fd_err_out, fileno(stderr)) == -1) {
                fprintf(stderr, "error: unable to replace outerr with fd.");
                exit(errno);
            }

            close(fd_err_out);
        }

        /**
         * Execute the command & it's arguments
         */
        errno = 0;
        debug("RUNNING: %s\n", full_command);
        if (execvp(full_command, command_args) == -1) {
            fprintf(stderr, "error: execvp failed to execute, errno: '%d'", errno);
            exit(errno);
        }
    }

    debug("exec new commands - parent pid: %d (spawned child: %d)\n", getpid(), pid);
    cmd->pid = pid;

    /**
     * Push the command we just began running into the running jobs list.
     */
    executor_push_execd(cmd);

    return;
}

char *executor_find_binary(char *command, string_list *bin_list) {
    if (bin_list == NULL) {
        fprintf(stderr, "error: no binary list available from parsed PATH dir.\n");

        return NULL;
    }

    struct dirent *de;

    for (int i = 0; i < bin_list->size; i++) {
        DIR *dr = opendir(bin_list->strings[i]);
        if (dr == NULL) {
            debug("error trying to open directory: '%s'\n", bin_list->strings[i]);

            return NULL;
        }

        while ((de = readdir(dr)) != NULL) {
            if (strcmp(de->d_name, "..") != 0 && strcmp(de->d_name, ".") != 0) {
                // debug("opened directory: '%s'\n", de->d_name);
                if (strcmp(de->d_name, command) == 0) {
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

int executor_exec_command(string_list *command, string_list *bin_list) {
    if (command == NULL) {
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

    /** $ exit - exit the prog. */
    if (strcmp(command->strings[0], COMMAND_EXIT) == 0) {
        return COMMAND_RETURN_EXIT;
    }

    /** $ pwd - get the cwd of the shell, via getcwd() */
    if (strcmp(command->strings[0], COMMAND_PWD) == 0) {
        char current_path[MAX_PATH];
        if (getcwd(current_path, MAX_PATH) == NULL) {
            fprintf(stderr, "error: unable to get current path\n");
            set_last_return_value(COMMAND_RETURN_RETRY);
            return COMMAND_RETURN_RETRY;
        }

        fprintf(stdout, "%s\n", current_path);

        return COMMAND_RETURN_INTERNAL_CMD;
    }

    /** $ cd - change dir using chdir() */
    if (strcmp(command->strings[0], COMMAND_CD) == 0) {
        char *chdir_to;
        if ((chdir_to = parse_path_get_env(ENV_HOME_KEY)) == NULL) {
            chdir_to = ROOT_PATH;
        }

        /** If there is an argument present, then try to cd to that dir instead of default home dir above */
        if (command->size > 1) {
            chdir_to = command->strings[1];
        }

        /** Attempt to change dir */
        if (chdir(chdir_to) != 0) {
            fprintf(stderr, "error: unable to change directory to '%s'\n", chdir_to);
            set_last_return_value(COMMAND_RETURN_RETRY);
            return COMMAND_RETURN_RETRY;
        }

        return COMMAND_RETURN_INTERNAL_CMD;
    }

    /** Since not matching any builtin commands - search in bin dirs. */
    char *bin_dir;
    if ((bin_dir = executor_find_binary(command->strings[0], bin_list)) == NULL) {
        if (command->strings[0][0] == '#') {
            return COMMAND_RETURN_COMMENT;
        }

        set_last_return_value(COMMAND_RETURN_NOT_FOUND);
        return COMMAND_RETURN_NOT_FOUND;
    }

    cmd->bin_dir = bin_dir;

    debug("found binary path at: '%s'\n", bin_dir);

    /**
     * Execute the binary w/ it's arguments and other misc. info.
     * Call this after finding and setting the binary.
     */
    // parse_command_debug_commander(cmd);
    executor_exec_bin_command(cmd, command);
    executor_debug_execd();

    return COMMAND_RETURN_SUCCESS;
}

/**
 * Debug print out the current jobs list
 */
void executor_debug_execd() {
    debug2("DEBUG EXECUTOR EXECD JOBS LIST\n");

    executor_jobs *current = execd_job_list;

    while (current->cmd != NULL) {
        debug2("CURRENT: '%p'\n", current);
        debug2("NEXT: '%p'\n", current->next);
        debug2("PREV: '%p'\n", current->prev);
        debug2("COMMANDER DEBUG: ...\n");
        parse_command_debug_commander(current->cmd);
        debug2("---- / END EXECUTOR_DEBUG_EXECD() /----\n");

        if (current->next == NULL) {
            break;
        }

        /** Iterate to next job */
        current = current->next;
    }
}
