#include "cd.h"

void strswap(char **str1, char **str2) {
    char *tmp = *str1;
    *str1 = *str2;
    *str2 = tmp;
}


int cmd_cd(char* ref) {

    char *pwd = malloc(PATH_MAX);
    if (pwd == NULL) goto error;
    memset(pwd, '\0', PATH_MAX);
    strcpy(pwd, getenv("PWD"));

    char *oldpwd = malloc(PATH_MAX);
    if (oldpwd == NULL) goto error;
    memset(oldpwd, '\0', PATH_MAX);
    strcpy(oldpwd, getenv("PWD"));

    int value;
    

    // cd       -> le repertoire courant devient $HOME.
    
    if (ref == NULL || strcmp(ref, "") == 0) {
        // Si HOME n'existe pas alors on renvoie une erreur.
        if (chdir(getenv("HOME")) < 0) {
            value = 1;
            goto error;
        } 
        strcpy(oldpwd, pwd);
        strcpy(pwd, getenv("HOME"));
    }


    // cd [-]   -> repertoire courant devient le repertoire precedent. 
    
    else if (strcmp(ref, "-") == 0) {
        // OLDPWD peut être NULL si aucun cd n'a été fait avant
        if (getenv("OLDPWD") != NULL) {
            strcpy(oldpwd, getenv("OLDPWD"));
            if (chdir(oldpwd) < 0){
                value = 2;
                goto error;
            }
            strswap(&oldpwd, &pwd);
        }
    }


    // cd [ref] -> verifier que ref est valide et changer de repertoire.
    
    else {
        if (chdir(ref) == -1) {
            value = 3;
            goto error;
        }
        strcpy(oldpwd, pwd);
        strcat(pwd, "/");
	    strcat(pwd, ref);
	}


    if (setenv("OLDPWD", oldpwd, 1) < 0) goto error;
    if (setenv("PWD", pwd, 1) < 0) goto error;

    ////////////////////// pour tester //////
    // printf("Ancien répertoire de travail : %s\n", oldpwd);
    // printf("Nouveau répertoire de travail : %s\n", pwd);
    ///////////////////////////////////////////

    
    free(pwd);
    pwd = NULL;
    free(oldpwd);
    oldpwd = NULL;

    return 0;

    // Gestion des erreurs
    error:
        switch (value){
            case 1: perror("HOME"); break;
            case 2: perror("Oldpwd doesnt exist"); break;
            case 3: perror("Invalid reference"); break;
        }
        free(pwd);
        free(oldpwd);
        return 1;
}



 
   