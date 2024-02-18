#include "colors.h"


char *shcolorpb(color c, colorplan p, bool bold) {
    char colo = (char) c;
    char plan = (char) p;
    char boldc = bold ? '1' : '0';

    char tab[COLOR_SIZE] = {'\001', '\033', '[', 
            boldc, ';', plan, colo, 
            'm', '\002', '\0'};

    char *s = malloc(COLOR_SIZE + 1);
    strncpy(s, tab, COLOR_SIZE + 1);
    return s;
}
char *shcolorp(color c, colorplan p) {
    return shcolorpb(c, p, false);
}
char *shcolorb(color c, bool bold) {
    return shcolorpb(c, FG, bold);
}
char *shcolor(color c) {
    return shcolorpb(c, FG, false);
}


void print_err(char *str) {
    char *clrerr = shcolor(RED);
    dprintf(STDERR_FILENO, "%s%s%s\n", clrerr, str, COLOR_NONE);
    free(clrerr);
}

void print_info(char *str) {
    char *clrinfo = shcolor(BLUE);
    dprintf(STDERR_FILENO, "%s%s%s\n", clrinfo, str, COLOR_NONE);
    free(clrinfo);
}
