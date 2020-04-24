#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parse_path.h"
#include "debug.h"
#include "globals.h"
#include "string_list.h"

/**
 * Parse out the path string from the array of environment variables.
 * 
 * @return char *string - representing the list of all directories where executable binaries exist. Typically delimited by ':'.
 * @return NULL - when no env path variable found - may need to configure globals.h for system. 
 */
char *parse_path_string(char *envp[])
{
    /**
     * Split the envp variable by new line, then split each new line by "=" sign.
     */

    int i = 0;
    char *token = NULL;
    char *path_value = NULL;

    while (envp[i] != NULL)
    {
        token = strtok(envp[i], ENV_PATH_DELIM);

        /**
         * Token is only null if no = is found.
         * No = is found when no valid key-value env variable set.
         */
        if (token == NULL)
        {
            break;
        }

        /**
         * If the token is == "PATH" then we've found the correct K-V pair in the env list.
         */
        if (strcmp(token, ENV_PATH_KEY) == 0)
        {
            path_value = strtok(NULL, ENV_PATH_DELIM);
            break;
        }

        i++;
    }

    return path_value;
}

/**
 * Split the path variable contents by ':' to retrieve each of its individual directory paths.
 * 
 * @return char **array - array of strings - each representing the path to a bin executable dir.
 */
string_list *parse_path_bin_dirs(char *path_str)
{
    char *token = NULL;
    token = strtok(path_str, BIN_EXEC_DELIM);

    if (token == NULL)
    {
        fprintf(stderr, "error: unable to parse bin executable directory paths.\n");
        return NULL;
    }

    string_list *bin_list = string_list_from(token);

    while (token != NULL)
    {
        debug("the token is %s\n", token);

        if ((token = strtok(NULL, BIN_EXEC_DELIM)) == NULL)
        {
            break;
        }

        string_list_push(bin_list, token);
    }

    return bin_list;
}
