#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_PATH 128

static char *SMASH_VERSION = "1.0.3";
static char *ROOT_PATH = "/";
static char *ENV_PATH_KEY = "PATH";
static char *ENV_HOME_KEY = "HOME";
static const char *ENV_DELIM = "=";
static const char *BIN_EXEC_DELIM = ":";
static const char *BACKGROUND_KEY = "&";
static const char *OUTPUT_REDIRECT_KEY = ">";
static const char *OUTPUT_ERROR_REDIRECT_KEY = "2>";
static const char *INPUT_REDIRECT_KEY = "<";
static const char VARIABLE_START_KEY = '$';
static const char NULL_CHAR = '\0';
static char *LAST_RETURN_KEY = "$?";
static char *PROMPT = "smash> ";
static char *DEBUG_FLAG = "-d";

#endif
