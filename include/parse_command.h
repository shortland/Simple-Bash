#ifndef PARSE_COMMAND_H
#define PARSE_COMMAND_H

#include "string_list.h"

static int jobs;

typedef struct commander
{
    int job_id;

    int bgfg; // -1 = is foreground job, 1 is background job.

    pid_t pid;     // should only have non-zero pid if it's been started
    int started;   // 1 = has started, -1 = has not started.
    int finished;  // 1 = finsihsed, -1 = has no finished.
    int running;   // 1 = running, -1 = not running.
    int exit_code; // exit code of this command after it finished running

    string_list *raw_command;

    char *bin_dir;               // e.g.) '/usr/local/bin'
    char *bin;                   // e.g.) 'echo'
    char **bin_params;           // e.g.) '-s -x'
    int num_bin_params;          // e.g.) '2' (need to keep track of size of param array above)
    char *output_redirect;       // e.g.) '>someOutput'
    char *output_error_redirect; // e.g.) '2>somefile'
    char *input_redirect;        // e.g.) '<someInputFile'
} commander;

string_list *parse_command_to_string_list(char *command);

commander *parse_command_from_string_list(string_list *command);

void parse_command_debug_commander(commander *cmd);

#endif
