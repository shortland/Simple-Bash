#include "parse_path.h"

static int LAST_RETURN = 0;

int get_last_return_value() {
    return LAST_RETURN;
}

void set_last_return_value(int code) {
    LAST_RETURN = code;
}

/**
 * Parse out all the environment variables, and set it in the static variable @see env_params
 */
char **parse_path_all_env_params(char *envp[]) {
    char **env_list = pointer_pointer_dup(envp);
    debug2("contents of new created envlist\n");
    pointer_pointer_debug(env_list, -1);
    debug2("done printing contents of envlist\n");

    int i = 0;
    char *token = NULL;
    char *path_value = NULL;

    while (envp[i] != NULL) {
        token = strtok(envp[i], ENV_DELIM);

        /**
         * Token is only null if no = is found.
         * No = is found when no valid key-value env variable set.
         */
        if (token == NULL) {
            break;
        }

        /**
         * Resize env_variables to hold 1 more k-v pair pointer
         */
        env_variables = realloc(env_variables, (i + 1) * sizeof(env_params **));
        env_variables[i] = malloc(1 * sizeof(env_params *));

        /**
         * Create space for the key and copy it over.
         */
        env_variables[i]->key = malloc(strlen(token) + 1);
        memcpy(env_variables[i]->key, token, strlen(token));
        env_variables[i]->key[strlen(token)] = NULL_CHAR;

        /**
         * Create space for the value and copy it over
         */
        path_value = strtok(NULL, ENV_DELIM);
        env_variables[i]->value = malloc(strlen(path_value) + 1);
        memcpy(env_variables[i]->value, path_value, strlen(path_value));
        env_variables[i]->value[strlen(path_value)] = NULL_CHAR;

        i++;
    }

    env_variables = realloc(env_variables, i * sizeof(env_params **));
    env_variables[i - 1] = malloc(sizeof(NULL));
    env_variables[i - 1] = NULL;

    return env_list;
}

/**
 * Get and environment variables value from it's key.
 * Or return NULL if key not found in env.
 */
char *parse_path_get_env(char *key) {
    int i = 0;

    while (env_variables[i] != NULL) {
        if (strcmp(env_variables[i]->key, key) == 0) {
            debug("found env variable for specified key.\n");

            return strdup(env_variables[i]->value);
        }

        i++;
    }

    return NULL;
}

/**
 * Split the path variable contents by ':' to retrieve each of its individual directory paths.
 * 
 * @return char **array - array of strings - each representing the path to a bin executable dir.
 */
string_list *parse_path_bin_dirs(char *path_str) {
    char *token = NULL;
    token = strtok(path_str, BIN_EXEC_DELIM);

    if (token == NULL) {
        fprintf(stderr, "error: unable to parse bin executable directory paths.\n");

        return NULL;
    }

    string_list *bin_list = string_list_from(token);

    while (token != NULL) {
        debug("the token is %s\n", token);

        if ((token = strtok(NULL, BIN_EXEC_DELIM)) == NULL) {
            break;
        }

        string_list_push(bin_list, token);
    }

    return bin_list;
}

void parse_path_debug_env_variables() {
    debug2("attempting to print out all env_variables.\n");

    int i = 0;

    while (env_variables[i] != NULL) {
        debug2("ENV_VARIABLE - key: '%s'\n", env_variables[i]->key);
        debug2("ENV_VARIABLE - value: '%s'\n", env_variables[i]->value);
        debug2("---- / END PARSE_PATH_DEBUG_ENV_VARIABLES() / ----\n");

        i++;
    }

    return;
}
