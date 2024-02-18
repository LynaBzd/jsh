#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <errno.h>
#include <stdbool.h>

#include "prompt.h"
#include "parse.h"
#include "utils.h"
#include "colors.h"


int valr = 0;


int main(int argc, char** argv){
    rl_outstream = stderr;

    char *prompt;
    char *line;

    printf(":::::::::::: JSH ::::::::::::\n");
    while (1) {
        prompt = affiche_prompt(valr);
        /* TODO : allouer et remplire line avec une version "propre" de la ligne saisie,
        * ie nettoyée des éventuelles erreurs et corrections (et du '\n' final) */
        line = readline(prompt);
        free(prompt);
        prompt = NULL;

        // si ctrl-D
        if (line == NULL) {
            printf(":::::::::: bye JSH ::::::::::\n");
            // en quittant on renvoie la dernière valeur de retour comme questionmark
            exit(valr);
        }

        /* évite d'aller plus loin dans la boucle si on a juste appuyé sur ENTER */
        if (strlen(line) == 0) {
            continue;
        }

        /* ajoute la ligne à l'historique, permettant 
        sa réutilisation avec les flèches du clavier */
        add_history(line);

        int linelen = strlen(line);
        char *linecpy = malloc((linelen+1)*sizeof(char));
        strncpy(linecpy, line, linelen+1);
        valr = exeline(linecpy, valr);

        // libération de la mémoire
        free(linecpy);
        linecpy = NULL;
        free(line);
        line = NULL;
    }

    /* efface l'historique si on quitte. C'est un choix d'effacer ou non */
    // clear_history();
    printf(":::::::::: bye JSH ::::::::::\n");

    return EXIT_SUCCESS;
}
