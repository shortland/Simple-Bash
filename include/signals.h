#ifndef SIGNALS_H
#define SIGNALS_H

volatile sig_atomic_t has_completed;

void signals_job_done(int sig);

int signals_readline_operator();

#endif