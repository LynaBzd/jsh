#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

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


/** Selectionne la redirection selon la chaine de caractere fourni en argument. */
int get_specials(char *str);

/** Application des redirections selon le type. */
int apply_redirection(int red, char* fic);

/** Parcours des arguments a la recherche d'une redirection ou &. Si oui apply_redirection est appelee. */
int check_special(char** argv);

/** reinitialise les stdin, stdout et stderr */
void resetstd();

#endif
