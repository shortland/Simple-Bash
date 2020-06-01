#include "signals.h"

/**
 * Handler for when a job finishes.
 */
void signals_job_done(int sig) {
    has_completed = 1;

    debug("a job appears to have finished\n");

    return;
}

/**
 * Check the jobs list for any jobs that need to be reaped.
 */
int signals_readline_operator() {
    executor_jobs *current = NULL;

    /** 
     * Reloop through process of looking for more children to reap if we've encountered 
     * a status saying no more children.
     */
    if (has_completed == 1) {
        /** Set back to 0 so we stop looking for children unless more have spawned. */
        has_completed = 0;

        /** Get the head of the jobs list */
        current = executor_execd_head();

        debug2("the current is: '%p'\n", current);

        while (current->cmd != NULL) {
            int status;
            pid_t wpid;

            debug("look for any pids that can be reaped.\n");

            if ((wpid = waitpid(current->cmd->pid, &status, WNOHANG)) <= 0) {
                debug("no more pids to wait for.\n");
            }

            if (WIFEXITED(status)) {
                debug("ENDED: '%s'(ret=%d)\n", current->cmd->bin, WEXITSTATUS(status));

                /** set the exit code of the prog. */
                set_last_return_value(WEXITSTATUS(status));

                current->cmd->exit_code = WEXITSTATUS(status);
                current->cmd->finished = 1;

                if (status == 0) {
                    debug2("child exited with status 0\n");
                    executor_pop_execd(current->cmd->job_id);
                } else {
                    debug2("child exit not-ok: '%d'\n", WEXITSTATUS(status));
                    executor_pop_execd(current->cmd->job_id);
                }
            } else {
                // TODO: remove child from list... (use executor_pop_execd())
                debug2("error; child didn't have an exit status code; might be result of SIGFAULT (or some run-time crash)\n");
            }

            if (current->next == NULL) {
                break;
            }

            /** Iterate to next job */
            current = current->next;
        }
    }

    return 0;
}
