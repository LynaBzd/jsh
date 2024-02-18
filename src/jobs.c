#include "jobs.h"
#include "utils.h"

jobs *first_job=NULL;
int next_job_id = 0;
int watched_jobs = 0;

char* etat_char(etat e) {
    char* res = malloc(9); // Allouer de l'espace pour le plus long état + le caractère nul
    if (res == NULL) {
        perror("Erreur : allocation de mémoire échouée, jobs.\n");
        return NULL;
    }
    switch (e) {
        case DONE:
            strncpy(res, "Done", 9);
            break;
        case RUNNING:
            strncpy(res, "Running", 9);
            break;
        case STOPPED:
            strncpy(res, "Stopped", 9);
            break;
        case KILLED:
            strncpy(res, "Killed", 9);
            break;
        case DETACHED:
            strncpy(res, "Detached", 9);
            break;
        default:
            strncpy(res, "Failed", 9);
            break;
    }
    return res;
}

int is_first(jobs* first){
    if (first == NULL) {
        printf("Pas de jobs en paramètre de is_first.\n");
        return -1;
    }
    first_job=first;
    return (first->suiv == NULL) ? 1 : 0; 
}

void affiche_jobs(jobs* jobs, int std) {
    if (jobs == NULL) {
        printf("Pas de jobs a afficher.\n");
        return;
    }

    char number_jobs[12]; 
    sprintf(number_jobs, "[%d]", jobs->id);

    char group[12]; 
    sprintf(group, "%d", jobs->pgid);

    char* etat = etat_char(jobs->etat);
    if (etat == NULL) {
        return;
    }
    dprintf(std, "%s\t%s\t%s\t%s\n", number_jobs, group, etat, jobs->commande ? jobs->commande : "Commande inconnue");
    free(etat); 
}

int affiche_grp(jobs* first){
    if (first == NULL) {
        // printf("Pas de jobs a afficher.\n");
        return 0;
    }
    jobs* tmp=first;
    while(tmp->suiv!=NULL){
        affiche_jobs(tmp, STDOUT_FILENO);
        tmp=tmp->suiv;
    }
    affiche_jobs(tmp,1);
    return 0;
}

jobs* nouv_jobs(etat etat, char* cmd, pid_t pid, pid_t pgid) {
    jobs* nouveauJob = (jobs*)malloc(sizeof(jobs));
    if (nouveauJob == NULL) {
        perror("Erreur : allocation de mémoire échouée, nouveauJob | jobs.c.\n");
        exit(EXIT_FAILURE);
    }
    nouveauJob->id = ++next_job_id;
    watched_jobs++;

    nouveauJob->pgid = pid;
    nouveauJob->etat = etat;
    nouveauJob->commande = strdup(cmd); 
    nouveauJob->courant = nouveauJob;
    nouveauJob->suiv = NULL;
    
    if (first_job == NULL) {
        first_job = nouveauJob;
    } else {
        nouveauJob->suiv = first_job;
        first_job = nouveauJob;
    }
    return nouveauJob;
}

jobs* search_jobs(int id) {
    jobs *courant = first_job;
    while (courant != NULL) {
        if (courant->id == id) {
            return courant; // Retourne le job trouvé
        }
        courant = courant->suiv; // Passe au job suivant
    }
    return NULL; 
}


void remove_jobs() {
    jobs *courant = first_job;
    jobs *precedent = NULL;

    while (courant != NULL) {
        jobs *tmp = courant->suiv;

        if (courant->etat == DONE || courant->etat == KILLED) {
            if (precedent == NULL) {
                first_job = tmp;
            } else {
                precedent->suiv = tmp;
            }

            free(courant->commande);
            courant->commande = NULL;
            // free(courant);
            // courant = NULL;

            free(courant);
            courant = (precedent == NULL) ? first_job : precedent->suiv;

        } else {
            precedent = courant;
        }
        courant = tmp;
    }
}


void update_all_jobs_status() {
    jobs* courant = first_job;
    while (courant != NULL) {
        int status;
        pid_t res = waitpid(courant->pgid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if (res > 0) {
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                courant->etat = (WIFEXITED(status)) ? DONE : KILLED;
                watched_jobs--;
                affiche_jobs(courant, 2);
                remove_jobs();
                courant = first_job;
            } else {
                if (WIFSTOPPED(status)) {
                    courant->etat = STOPPED;
                } else if (WIFCONTINUED(status)) {
                    courant->etat = RUNNING;
                }
                affiche_jobs(courant, 2);
                courant = courant->suiv;
            }
        } else {
            courant = courant->suiv;
        }
    }
}

void update_all_jobs_status_without_print() {
    jobs* courant = first_job;
    while (courant != NULL) {
        int status;
        pid_t res = waitpid(courant->pgid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if (res > 0) {
            if (WIFEXITED(status)) {
                courant->etat = DONE;
                affiche_jobs(courant, 1);
                watched_jobs--;
                remove_jobs(courant->pgid);
            } else if (WIFSTOPPED(status)) {
                courant->etat = STOPPED;
            } else if (WIFCONTINUED(status)) {
                courant->etat = RUNNING;
            }else if (WIFSIGNALED(status)) {
                courant->etat = KILLED;
                watched_jobs--;
                remove_jobs(courant->pgid);
            }
        }

        courant = courant->suiv;
    }

}




