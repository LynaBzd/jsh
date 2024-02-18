#include "pwd.h"
#include "colors.h"


int cmd_pwd() {
    // allocation d'une zone mémoire pour le path
    char *path = malloc(PATH_MAX);
    if (path == NULL) {
        perror("Malloc path"); 
        goto error;
    }

    char *cwd = getcwd(path, PATH_MAX);
    if (cwd != NULL) {
        path = cwd;
        
        // écriture sur la sortie standard
        write(STDOUT_FILENO, path, strlen(path));
        write(STDOUT_FILENO, "\n", 1);

        // libère les ressources allouées 
        free(path);
        return 0;
    } else {
        goto error;
    }
    
    // gestion des erreurs
    error:
        free(path);
        return 1;
}


    





