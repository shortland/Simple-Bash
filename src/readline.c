#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "readline.h"
#include "debug.h"

char *readline(char *prompt)
{
    int size = 32;
    char *buf = malloc(size);

    if (buf == NULL)
    {
        fprintf(stderr, "error: unable to malloc space for readline buffer\n");
        return NULL;
    }

    char c;
    char *bp = buf;

    fprintf(stdout, "%s", prompt);
    fflush(stdout);

    while (1)
    {
        /**
         * Read in 1 byte at a time.
         */
        if (read(0, &c, 1) != 1)
        {
            if (bp - buf > 0)
            {
                // new line
                break;
            }
            else
            {
                // possible EOF
                free(buf);
                return NULL;
            }
        }

        /**
         * User pressed enter.
         */
        if (c == '\n')
        {
            break;
        }

        if (bp - buf >= size - 1)
        {
            size <<= 1;
            char *new_buf = realloc(buf, size);

            if (new_buf == NULL)
            {
                fprintf(stderr, "realloc failed");
                break;
            }
            else
            {
                bp = new_buf + (bp - buf);
                buf = new_buf;
            }
        }

        *bp++ = c;
    }

    *bp = '\0';
    return buf;
}
