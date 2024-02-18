#ifndef FG_H
#define FG_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <limits.h>
#include <string.h> 
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>


int fg(int job_id);


#endif