#ifndef JOBS_H
#define JOBS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>
#include <string.h> 
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>


typedef struct jobs jobs;

typedef enum {
    DONE=(int)'0',
    RUNNING=(int)'1',
    STOPPED=(int)'2',
    KILLED=(int)'3',
    DETACHED=(int)'4',
    FAILED=(int)'5',
} etat;

typedef struct jobs{
    unsigned id;
    pid_t pgid;
    etat etat;
    char* commande;
    jobs* courant;
    jobs* suiv;
} jobs;

extern jobs* first_job;

extern int next_job_id; 
extern int watched_jobs;


/** Get the status of a job as a char */
char* etat_char(etat e) ;

/** Display one job */
void affiche_jobs(jobs* jobs, int std);

/** Search a job by id */
jobs* search_jobs(int id);


/** Display all jobs */
int affiche_grp(jobs* first);

/** Check if the job is the first job */
int is_first(jobs* first);

/** Create a new job 
 * background sera utilisé comme booléen qui me permettra de savoir si il fait parti du même jobs
*/
jobs* nouv_jobs(etat etat, char* cmd, pid_t pid, pid_t pgid);

/** Update all jobs status */
void update_all_jobs_status();

/** Update all jobs status without print */
void update_all_jobs_status_without_print();

/** Remove job by id */
void remove_jobs();


#endif