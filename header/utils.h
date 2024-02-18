#ifndef UTILS_H
#define UTILS_H

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

#define MAX_INT_STRLEN 13


/**
 * Convertit un entier en une str, stockée dans buf.
*/
void itos(char *buf, int i);

int get_length(char **lst);

char** sans_le_et(char** cmd);

/** Découpe une str donnée en entrée en un
 * tableau de str, où chaque case représente une
 * portion de la str délimitée par le sep. 
 * 
 * Le tableau renvoyé doit être free, mais pas les
 * éléments dedans car ce sont des pointeurs vers 
 * des positions de str d'origine.
 */
char **explode(char *str, char *sep);

/** Fonctionne un peu comme strtok pour un tableau
 * de type char** :
 *  - modifie *darr pour pointer vers le premier élément
 *      après le 1er sep trouvé.
 *  - renvoie un char** contenant tous les éléments de *darr
 *      jusqu'au 1er sep. Ce char** doit être free et ses éléments
 *      aussi avec la fonction dbl_free.
 */
char **split_sep(char ***darr, char *sep);

/**
 * Fonction qui permet de free un tableau de char**.
 */
void dbl_free(char **darr);

#endif
