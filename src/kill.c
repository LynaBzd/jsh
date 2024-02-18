#include "jobs.h"

int kill_command(char** argv) {
    if (argv[1] == NULL) {
        perror("Aucun argument");
        return -1;
    }

    int argi = 1;
    int signal = -1;
    char *sig = argv[argi];

    if (sig[0] == '-') {
        ++sig;
        ++argi;
        signal = atoi(sig);
    }
    if (argv[argi][0] == '%') {
        char *jobstr = argv[argi];
        ++jobstr;

        if (*jobstr == '\0') {
            perror("Mauvais argument");
            return -1;
        }
        
        int jobid = atoi(jobstr);
        jobs* tmp = first_job;
        while (tmp != NULL) {
            if (tmp->id == jobid) {
                // dprintf(STDERR_FILENO, "kill %s %d\n", tmp->commande ,jobId);
                if (signal == -1) {
                    kill(-tmp->pgid, SIGTERM);
                } else {
                    kill(-tmp->pgid, signal);
                }
                return 0;
            }
            tmp = tmp->suiv;
        }
        perror("Job non trouvé");
        return -1;
    } else {
        int pid = atoi(argv[argi]);
        if (pid <= 0) {
            perror("PID invalide");
            return -1;
        }

        if (signal == -1) {
            kill(pid, SIGTERM);
        } else {
            kill(pid, signal);
        }
        return 0;
    }
}