#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_return_list.h"
#include "debug.h"
#include "executor.h"
#include "globals.h"
#include "readline.h"
#include "signals.h"
#include "string_list.h"

int interactive_mode_run(int argc, char *argv[], string_list *bin_list);

#endif
