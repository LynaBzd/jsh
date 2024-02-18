#include "extern.h"
#include "jobs.h"
#include "utils.h"

int cmd_extern(char *cmd, char **argv, char *line, int background, int isbg) {
    int valr = 0;
    pid_t pid;
    pid_t wpid;
    int status;
    
    pid = fork();
    switch (pid) {
        case -1 :
            perror("fork() échoué : cmd_extern()");
            exit(1);
        case 0 : {
            update_all_jobs_status();
            setpgid(0,0); // tester si tt ok
            signal(SIGTERM,SIG_DFL);

            // exe de la commande externe, pas de contrôle sur la suite
            execvp(cmd, argv);
            // si la bascule sur la commande externe ne s'est pas faite
            exit(EXTERN_UNKOWN);
        }
        default : {
            if (isbg) {
                jobs *job = nouv_jobs(RUNNING, line, pid, pid);
                affiche_jobs(job, 2);
            } else {
                    signal(SIGTTOU, SIG_IGN); // ignore les signaux avant de faire tcsetpgrp
                    tcsetpgrp(STDIN_FILENO,pid);

                    wpid=waitpid(pid, &status, WUNTRACED | WCONTINUED);
                    if(wpid==-1){
                        perror("waitpid 34");
                        exit(EXIT_FAILURE);
                    }
                    if (WIFEXITED(status)) {
                        valr = WEXITSTATUS(status);
                    }
                    if (WIFSTOPPED(status)) {
                        jobs *job = nouv_jobs(STOPPED, line, pid, pid);
                        update_all_jobs_status();
                        affiche_jobs(job, 2);
                    }

                    tcsetpgrp(STDIN_FILENO,getpgrp());
                    return valr;
            }
        }
    }

    return -1;
}