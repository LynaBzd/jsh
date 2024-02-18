#include "prompt.h"
#include "colors.h"
#include "utils.h"
#include "jobs.h"


char* shrink_path(char* str, int len) {
    if (str[0] == '\0') {
        perror("Erreur mot");
        return NULL;
    }
    if (len < 3) {
        len = 3;
    }
    char* ret = calloc(len+1,sizeof(char));
    if (ret == NULL) {
        perror("Erreur : allocation de mémoire échouée : shrink_path().\n");
        exit(EXIT_FAILURE);
    }
    ret[0] = '.';
    ret[1] = '.';
    ret[2] = '.';
    int j = 3;
    for (int i = strlen(str)-len+j; i < strlen(str); ++i) {
        ret[j] = str[i];
        j++;
    }
    return ret;
}

char* affiche_prompt(int valr) {
    update_all_jobs_status();
    char *courant = calloc(PATH_MAX, 1);
    if (courant == NULL) {
        perror("Allocation mémoire de courant échouée : affiche_prompt()");
        exit(EXIT_FAILURE);
    }
    int prompt_len = PROMPT_SIZE + COLOR_SIZE*4 +1;
    char *prompt = malloc(prompt_len*sizeof(char));
    if (prompt == NULL) {
        perror("Allocation mémoire de prompt échouée : affiche_prompt()");
        goto end;
    }
    /* obtention du chemin absolu du répertoire courant */
    if (getcwd(courant, PATH_MAX) == NULL) {
        perror("getcwd() échoué : affiche_prompt()");
        goto end;
    }

    /* récup du nombre de jobs en str */
    char jobstr[MAX_INT_STRLEN];
    itos(jobstr, watched_jobs);
    
    // -4 pour les "[]" et "$ "
    size_t jobstr_len = strlen(jobstr);
    size_t taille_max_path = PROMPT_SIZE -4 -jobstr_len;

    char *clr;
    
    /* remplissage du prompt */
    strcpy(prompt, "[");
    clr = shcolorb(GREEN, true);
    if (clr == NULL) {
        perror("Allocation mémoire de clr échouée : affiche_prompt()");
        goto end;
    }
    strcat(prompt, clr);
    free(clr);
    clr = NULL;
    strcat(prompt, jobstr);
    strcat(prompt, COLOR_NONE);
    strcat(prompt, "]");
    clr = shcolorb(BLUE, true);
    strcat(prompt, clr);
    free(clr);
    clr = NULL;
    size_t courant_len = strlen(courant);
    if (courant_len <= taille_max_path) {
        strcat(prompt, courant);
    } else {
        // -3 pour les "...""
        char *skpath = shrink_path(courant, taille_max_path);
        if (skpath == NULL) {
            perror("Allocation mémoire de skpath échouée : affiche_prompt()");
            goto end;
        }
        strcat(prompt,skpath);
        free(skpath);
        skpath = NULL;
    }
    strcat(prompt, COLOR_NONE);
    strcat(prompt, "$ ");

    /* libération de mémoire */
    free(courant);
    courant = NULL;
    return prompt;
    
    end:
        free(courant);
        courant = NULL;
        exit(EXIT_FAILURE);
}