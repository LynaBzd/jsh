#ifndef PARSE_H
#define PARSE_H

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


/** Traite la ligne de commande pour la préparer à l'interprétation */
int exeline(char *line, int prevalr);

/** Interprète les arguments : 
 * cmd pour choisir la commande à exécuter,
 * argv pour y passer les éventuels arguments de cette commande.
 * 
 * Si la commande à exécuter n'a pas d'argument, 
 * argv devrait contenir un seul élément NULL. */
int execmd(char **argv, char *line, int prevalr);

/** Vérifie si argv a un nombre d'arguments attendu
 * @returns 20 si trop d'arguments, 21 si pas assez, 0 si c'est ok. */
int checknbargs(char **argv, int min, int max);

/** Récupère le nombre de pipes dans argv. */
int nb_pipes(char** argv);

/** Applique les pipes aux arguments. */
int apply_pipes(char *line, char **argv, int pipes, int prevalr);

/** Récupère le nombre de substitutions dans argv. */
int nb_sub(char** argv);

/** Applique les substitutions aux arguments. (une seule sub fonctionne) */
int apply_sub(char *line, char **argv, int prevalr);


#endif
