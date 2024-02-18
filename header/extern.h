#ifndef EXTERN_H
#define EXTERN_H

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


#define EXTERN_UNKOWN 127


/** exécute une commande externe si elle existe.
 * en utilisant cmd pour déterminer le nom de 
 * la commande et argv pour ses arguments. */
int cmd_extern(char *cmd, char **argv, char* line, int background, int isbg);


#endif