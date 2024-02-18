#include "jobs.h"
#include "utils.h"

int fg(int job_id) {
    jobs *job = search_jobs(job_id);
    if (job == NULL) {
        fprintf(stderr, "fg: job %d not found\n", job_id);
        return 1;
    }

    tcsetpgrp(STDIN_FILENO, job->pgid);

    if (job->etat == STOPPED) {
        if (kill(-job->pgid, SIGCONT) < 0) {
            perror("fg: error sending SIGCONT");
            return 1;
        }
    }

    int status;
    if (waitpid(job->pgid, &status, WUNTRACED) < 0) {
        perror("fg: waitpid error");
        return 1;
    }

    if (WIFEXITED(status) || WIFSIGNALED(status)) {
        job->etat = DONE;
        watched_jobs--;
    } else if (WIFSTOPPED(status)) {
        job->etat = STOPPED;
        affiche_jobs(job,2);
    }

    update_all_jobs_status_without_print();
    tcsetpgrp(STDIN_FILENO, getpgrp());

    return 0;
}

