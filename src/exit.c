#include "jobs.h"

int cmd_exit(char** argv, int prevalr) {
    jobs* tmp = first_job;
    while (tmp != NULL) {
        if (tmp->etat == RUNNING || tmp->etat == STOPPED) {
            perror("Il y a des jobs en cours d'exécution ou suspendus.\n");
            return 1;
        }
        tmp = tmp->suiv;
    }
    if (argv == NULL) {
        perror("problème de pointeur avec exit");
        exit(1);
    } else if (argv[0] == NULL) {
        exit(prevalr);
    } else {
        exit(atoi(argv[0]));
    }
}