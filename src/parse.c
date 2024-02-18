#include "parse.h"
#include "utils.h"
#include "colors.h"
#include "redirections.h"
#include "extern.h"
#include "questionmark.h"
#include "cd.h"
#include "exit.h"
#include "jobs.h"
#include "kill.h"
#include "pwd.h"
#include "bg.h"
#include "fg.h"


int exeline(char *line, int prevalr) {
    int valr = 0;

    char **argv = explode(line, " ");
    if (argv == NULL) {
        print_err("malloc argv échoué : execline()");
        valr = 1;
        return valr;
    }
    if (argv[0] == NULL) {
        print_err("malloc dans argv échoué : execline()");
        free(argv);
        valr = 2;
        return valr;
    }

    int nb = nb_sub(argv);
    if (nb>0) {
        valr = apply_sub(line, argv, prevalr);
        free(argv);
        return valr;
    }

    int pipes = nb_pipes(argv);
    if (pipes > 0) {
        valr = apply_pipes(line, argv, pipes, prevalr);
        free(argv);
        return valr;
    }
    if (pipes == -1) {
        valr = -1;
        return valr;
    }

    valr = execmd(argv, line, prevalr);

    if (valr == EXTERN_UNKOWN) {
        char *clrerr = shcolor(RED);
        dprintf(STDERR_FILENO, 
            "%scommande inconnue : %s%s\n", 
            clrerr, argv[0], COLOR_NONE
        );
        free(clrerr);
    }

    free(argv);
    return valr;
}


int execmd(char **argv, char *line, int prevalr) {
    int valr = 0;
    int isbg = 0;
    int rd = check_special(argv);
    if (rd == -1) {
        resetstd();
        return 1;
    }
    if (rd == -2) {
        resetstd();
        perror("redirection sans cible : execmd()");
        return 1;
    }
    if (rd == 7) {
        isbg = 1;
    }

    if (argv == NULL) {
        perror(
            "pas de tableau des arguments (si aucun argument argv"
            "contient une unique case NULL) : execmd()"
        );
        valr = 11;
        return valr;
    }

    char *cmd = argv[0];
    if (cmd == NULL) {
        perror("pas de commande reçue : execmd()");
        valr = 10;
        return valr;
    }

    if (strcmp(cmd, "?") == 0) {
        if ((valr = checknbargs(argv, 0, 0)) != 0) {
            return valr;
        }
        valr = cmd_questionmark(prevalr);
    }
    else if (strcmp(cmd, "cd")   == 0) {
        if ((valr = checknbargs(argv, 0, 1)) != 0) {
            return valr;
        }
        else valr = cmd_cd(argv[1]);
    }
    else if (strcmp(cmd, "exit") == 0) {
        if ((valr = checknbargs(argv, 0, 1)) != 0) {
            return valr;
        }
        // argv +1 pour prendre les arguments sans la commande
        valr = cmd_exit(argv + 1, prevalr);
    } else if (strcmp(cmd, "jobs") == 0) {
        if ((valr = checknbargs(argv, 0, 0)) != 0) {
            return valr;
        }
        update_all_jobs_status_without_print();
        valr = affiche_grp(first_job);
    } else if (strcmp(cmd, "kill") == 0) {
        if ((valr = checknbargs(argv, 0, 2)) != 0) {
            return valr;
        }
        valr = kill_command(argv);
    } else if (strcmp(cmd, "bg") == 0 || strcmp(cmd, "fg") == 0) {
        // Vérifier qu'il y a exactement un argument supplémentaire
        if ((valr = checknbargs(argv, 1, 1)) != 0) {
            return valr;
        }

        // Extraire l'ID du job (en supposant que l'argument est au format %<job_id>)
        int job_id = 0;
        if (argv[1][0] == '%') {
            job_id = atoi(&argv[1][1]); // Convertit la partie numérique en entier
        } else {
            fprintf(stderr, "Usage: %s %%job_id\n", cmd);
            return 1;
        }

        // Appeler la fonction fg ou bg avec l'ID du job
        if (strcmp(cmd, "bg") == 0) {
            valr = bg(job_id);
        } else if (strcmp(cmd, "fg") == 0) {
            valr = fg(job_id);
        }
    }
    else if (strcmp(cmd, "pwd")  == 0) {
        if ((valr = checknbargs(argv, 0, 0)) != 0) {
            return valr;
        }
        valr = cmd_pwd();
    } else {
        char line[1024];
        line[0] = '\0';  
        strcat(line, cmd);
        for (int i = 1; argv[i] != NULL; i++) {
            if(*argv[i]!='&'){
                strcat(line, " ");
                strcat(line, argv[i]);
            }
        }
        valr = cmd_extern(cmd, argv, line, 0, isbg);
        
    }

    resetstd();

    return valr;
}


int checknbargs(char **argv, int min, int max) {
    int valr = 0;
    char *clrerr = shcolor(RED);

    if (argv == NULL) {
        fprintf(stderr, 
            "%sargv non initialisé : checknbargs()%s\n",
            clrerr, COLOR_NONE
        );
    }
    if (argv[0] == NULL) {
        fprintf(stderr, 
            "%saucune commande détectée : checknbargs()%s\n",
            clrerr, COLOR_NONE
        );
    }
    if (argv[max+1] != NULL) {
        fprintf(stderr, 
            "%s%s : trop d'arguments (max : %d)%s\n", 
            clrerr, argv[0], max, COLOR_NONE
        );
        valr = 20;
    }

    for (int i = 1; i < min+1; i++) {
        if (argv[i] == NULL) {
            fprintf(
                stderr, "%s%s : pas assez d'arguments (min : %d)%s\n", 
                clrerr, argv[0], min, COLOR_NONE
            );
            valr = 21;
        }
    }

    free(clrerr);
    return valr;
}

int nb_pipes(char** argv) {
    int res = 0;
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) {
            res++;
        }
    }
    return res;
}

int apply_pipes(char *line, char **argv, int pipes, int prevalr) {
    int valr = 0;

    int in = STDIN_FILENO;

    for (int i = 0; i <= pipes; i++) {
        char **cmd = split_sep(&argv, "|");

        int pfd[2];
        if (i != pipes) {
            pipe(pfd);
        }

        pid_t pid;
        if ((pid = fork()) == 0) {
            if (i != 0) {
                dup2(in, STDIN_FILENO);
                close(in);
            }
            if (i != pipes) {
                close(pfd[0]);
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
            valr = execmd(cmd, line, prevalr);

            dbl_free(cmd);
            exit(0);
        }
        if (pid != 0) {
            if (i != 0) {
                close(in);
            }
            if (i != pipes) {
                close(pfd[1]);
                in = pfd[0];
            }
        }
        dbl_free(cmd);
        cmd = NULL;
    }

    for (int i = 0; i <= pipes; i++) {
        wait(NULL);
    }

    return valr;
}

int nb_sub(char** argv) {
    int res = 0;
    int found=0;
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "<(") == 0 && found == 0) {
            found = 1;
        } else if (strcmp(argv[i], ")") == 0 && found == 1){
            res++;
            found = 0;
        }
    }
    return res;
}

int apply_sub(char *line, char **argv,int prevalr) {
    int valr = 0;
    int nb = nb_sub(argv);
    if (nb <= 0) {
        return 1;
    }
    int fd[2];
    char **cmd = split_sep(&argv, "<(");

    for (int i = 1; i <= nb; i++) {
        char **sub_split = split_sep(&argv, ")");
        
        if (pipe(fd)) {
            print_err("tube");
            return 1;
        }

        // Rediriger la sortie vers le fichier temporaire
        pid_t pid = fork();
    
        if (pid == -1) {
            print_err("fork");
            return 1;
        } else if (pid == 0) {
            
            if (dup2(fd[1], STDOUT_FILENO) == -1) {
                print_err("dup2");
                close(fd[1]);
                return 1;
            }
            
            close(fd[1]); // Fermer le descripteur dupliqué

            valr = execmd(sub_split, line, prevalr); // pas un recouvrement
            exit(0);
        }
        
        close(fd[1]);       

        dbl_free(sub_split);
        int tailleCmd = 0;
        while (cmd[tailleCmd] != NULL) {
            tailleCmd++;
        }
        
        // Réallocation de 'cmd' pour ajouter de la place pour le nouvel élément plus un NULL 
        cmd = realloc(cmd, (tailleCmd + 2) * sizeof(char *));
        if (cmd == NULL) {
            print_err("realloc cmd échoué");
            
            return 1;
        }

        char fic[4096];
        snprintf(fic,sizeof(fic) ,"/dev/fd/%d", fd[0]);
        
        cmd[tailleCmd] = strdup(fic); 
        if (cmd[tailleCmd] == NULL) {
            print_err("strdup échoué");
            return 1;
        }
        cmd[tailleCmd + 1] = NULL; // Nouveau 
    }

    valr = execmd(cmd, line, 0);
    close(fd[0]);
    dbl_free(cmd);
    
    return valr;
}
