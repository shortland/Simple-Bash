#include "internal_command/pwd.h"

int internal_command_pwd(void) {
    char current_path[MAX_PATH];

    if (getcwd(current_path, MAX_PATH) == NULL) {
        return 1;
    }

    fprintf(stdout, "%s\n", current_path);

    return 0;
}
