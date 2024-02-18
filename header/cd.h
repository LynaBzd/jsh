#ifndef CD_H
#define CD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>


/**
 * Permute deux chaines de caracteres
*/
void strswap(char **str1, char **str2);

/** Affiche le chemin courant
 * @return 0 si succès, 1 si erreur */
int cmd_cd(char* ref);


#endif