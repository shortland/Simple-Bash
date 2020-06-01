#include "io.h"

void io_print_files_in_dir(char *path) {
    struct dirent *de = NULL;
    DIR *dr = NULL;

    if ((dr = opendir(path)) == NULL) {
        fprintf(stderr, "error: could not open specified directory.\n");
        return;
    }

    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, "..") != 0 && strcmp(de->d_name, ".") != 0) {
            printf("%s\n", de->d_name);
        }
    }

    closedir(dr);

    return;
}
