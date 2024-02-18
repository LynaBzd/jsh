#include "jobs.h"
#include "utils.h"


int bg(int job_id) {
    jobs *job = search_jobs(job_id);
    if (job == NULL) {
        fprintf(stderr, "bg: job %d not found\n", job_id);
        return 1;
    }
    update_all_jobs_status_without_print();
    if (job->etat == STOPPED) {
        kill(-job->pgid, SIGCONT);
        job->etat = RUNNING;
    } else {
        fprintf(stderr, "bg: job %d is not stopped\n", job_id);
        return 1;
    }

    return 0;
}
