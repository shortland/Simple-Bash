#ifndef READLINE_H
#define READLINE_H

typedef int readline_sig_hook(void);

void readline_set_sig_hook(readline_sig_hook hook);

char *readline(char *prompt, char *filename);

#endif
