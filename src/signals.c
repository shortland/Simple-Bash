#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "signals.h"
#include "debug.h"
#include "executor.h"
#include "parse_path.h"

/**
 * Handler for when a job finishes.
 */
void signals_job_done(int sig)
{
    has_completed = 1;

    debug("a job appears to have finished\n");
    return;
}

/**
 * Check the jobs list for any jobs that need to be reaped.
 */
int signals_readline_operator()
{
    debug("waiting for signal readline operator; pid: '%d'\n", getpid());

    executor_jobs *current;

    /** 
     * Reloop through process of looking for more children to reap if we've encountered 
     * a status saying no more children.
     */
    // recheck_pids:
    if (has_completed == 1)
    {
        debug("FOUND COMPLETED\n");
        /** Set back to 0 so we stop looking for children unless more have spawned. */
        has_completed = 0;

        /** Get the head of the jobs list */
        current = executor_execd_head();

        debug("the current is: '%p'\n", current);
        while (current->cmd != NULL)
        {
            debug("look for any pids that can be reaped.\n");

            int status;
            pid_t wpid = waitpid(current->cmd->pid, &status, WNOHANG);

            if (wpid <= 0)
            {
                debug("no more pids to wait for.\n");
                // goto recheck_pids;
            }

            if (WIFEXITED(status))
            {
                debug("Child %d terminated with exit status %d\n", wpid, WEXITSTATUS(status));

                // set the exit code of the prog.
                set_last_return_value(WEXITSTATUS(status));
                current->cmd->exit_code = WEXITSTATUS(status);
                current->cmd->finished = 1;

                if (status == 0)
                {
                    debug("child exited with status 0\n");
                    executor_pop_execd(current->cmd->job_id);
                }
                else
                {
                    debug("child exit not-ok: '%d'\n", WEXITSTATUS(status));
                    executor_pop_execd(current->cmd->job_id);
                }
            }
            else
            {
                debug("error; child didn't have an exit status code.\n");
            }

            if (current->next == NULL)
            {
                break;
            }

            /** Iterate to next job */
            current = current->next;
        }
    }

    return 0;
}
