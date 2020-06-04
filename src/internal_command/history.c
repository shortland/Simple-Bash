#include "internal_command/history.h"

int internal_command_history(char *home_path, char *history_file) {
    char *input_line = NULL;
    int command_count = 1;
    FILE *in_file = NULL;
    char path[MAX_PATH];

    sprintf(path, "%s/%s", home_path, history_file);

    if ((in_file = fopen(path, "rab+")) == NULL) {
        fprintf(stderr, "error: unable to open history file for reading: %s\n", path);
        return 1;
    }

    debug("wanting to print internal command history.\n");
    while ((input_line = readline(NULL, fileno(in_file))) != NULL) {
        fprintf(stdout, "%-5d", command_count);
        fprintf(stdout, "%s\n", input_line);

        command_count++;
    }

    fclose(in_file);

    return 0;
}

int internal_command_history_write(char *home_path, char *history_file, string_list *command) {
    FILE *in_file = NULL;
    char *raw_cmd = NULL;
    char path[MAX_PATH];

    if ((raw_cmd = string_list_string(command)) == NULL) {
        fprintf(stderr, "warning: unable to get full raw command from string list.\n");
    }

    sprintf(path, "%s/%s", home_path, history_file);

    if ((in_file = fopen(path, "a+")) == NULL) {
        fprintf(stderr, "error: unable to open history file for reading: %s\n", path);
        return 1;
    }

    if (write(fileno(in_file), raw_cmd, strlen(raw_cmd)) != strlen(raw_cmd)) {
        fprintf(stderr, "error: short write when writing to history file.\n");
        return 1;
    }

    if (write(fileno(in_file), "\n", 1) != 1) {
        fprintf(stderr, "error: short write when writing to history file.\n");
        return 1;
    }

    fclose(in_file);

    return 0;
}
