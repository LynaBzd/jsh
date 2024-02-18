#ifndef QUESITONMARK_H
#define QUESITONMARK_H

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


/** Revoie la valeur de retour
 * de la dernière commande utilisée. */
int cmd_questionmark(int prevalr);


#endif