#ifndef COLORS_H
#define COLORS_H

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


#define COLOR_SIZE 10
#define COLOR_NONE "\001\033[0;00m\002"

typedef enum {
    BLACK  = (int)'0',
    RED    = (int)'1',
    GREEN  = (int)'2',
    YELLOW = (int)'3',
    BLUE   = (int)'4',
    PURPLE = (int)'5',
    CYAN   = (int)'6',
    WHITE  = (int)'7',
} color;

typedef enum {
    FG = (int)'3',
    BG = (int)'4',
} colorplan;


/** change la couleur du shell.
 * Utiliser COLOR_NONE pour revenir aux couleurs par défaut.
 * 
 * @param c la couleur à utiliser.
 * @param p soit sur le texte soit à l'arrière-plan du texte.
 * @param bold true pour mettre le texte en gras
*/
char *shcolorpb(color c, colorplan p, bool bold);

/** change la couleur du shell, sans mise en gras.
 * Utiliser COLOR_NONE pour revenir aux couleurs par défaut.
 * 
 * @param c la couleur à utiliser.
 * @param p soit sur le texte soit à l'arrière-plan du texte.
*/
char *shcolorp(color c, colorplan p);

/** change la couleur du texte du shell.
 * Utiliser COLOR_NONE pour revenir aux couleurs par défaut.
 * 
 * @param c la couleur à utiliser.
 * @param bold true pour mettre le texte en gras
*/
char *shcolorb(color c, bool bold);

/** change la couleur du texte du shell, sans mise en gras.
 * Utiliser COLOR_NONE pour revenir aux couleurs par défaut.
 * 
 * @param c la couleur à utiliser.
*/
char *shcolor(color c);

/** affiche un message d'erreur. */
void print_err(char *str);

/** affiche un message d'information. */
void print_info(char *str);


#endif
