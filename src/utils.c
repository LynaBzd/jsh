#include "utils.h"
#include "colors.h"


void itos(char *buf, int i) {
	if (buf == NULL) {
		return;
	}
	sprintf(buf, "%d", i);
}


char** sans_le_et(char** cmd) {
    puts("OEE");
    if (cmd == NULL) {
        return NULL;
    }
    // Compter le nombre d'éléments
    int cpt = 0;
    while (cmd[cpt] != NULL) {
        cpt++;
    }
    if (cpt == 0) {
        return NULL;
    }
    // Allouer de la mémoire pour le nouveau tableau
    char** res = malloc((cpt + 1) * sizeof(char*));
    if (res == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    // Copier les chaînes
    for (int i = 0; i < cpt; i++) {
        if (cmd[i + 1] != NULL) {  // Si ce n'est pas le dernier élément
            res[i] = strdup(cmd[i]);
        } else {  // Pour le dernier élément, exclure le dernier caractère
            int len = strlen(cmd[i]);
            res[i] = malloc(len * sizeof(char));  // len inclut déjà le caractère nul
            if (res[i] == NULL) {
                perror("malloc failed");
                exit(EXIT_FAILURE);
            }
            strncpy(res[i], cmd[i], len - 1);  // Copier sans le dernier caractère
            res[i][len - 1] = '\0';  // Ajouter le caractère nul
        }
    }
    res[cpt] = NULL;  // Marquer la fin du tableau
    return res;
}


int get_length(char **lst) {
    int length = 0;
    if (lst == NULL) {
        return length;
    }
    while (*lst != NULL) {
        length++;
        lst++;
    }
    return length;
}

char **explode(char *str, char *separator) {
	if (str == NULL || separator == NULL) {
		return NULL;
	}

	// 1er mot
    int c = 0;
    char **res = malloc(sizeof(char *));
    if(res == NULL){
		perror("malloc res échoué : explode()");
        return NULL;
    }

    *(res) = strtok(str, separator);
	

    // mots suivants
    for (++c; res[c-1] != NULL; ++c) {
        res = realloc(res, (c+1) * sizeof(char *));
        res[c] = strtok(NULL, separator);
    };

    // valeur sentinelle
    res = realloc(res, (c+1) * sizeof(char *));
    res[c] = strtok(NULL, separator);

    return res;
}

char **split_sep(char ***darr, char *sep) {
    char **res = malloc(sizeof(char *));
    if(res == NULL){
		print_err("malloc res échoué : split_sep()");
        return NULL;
    }

    int i = 0;

    // copie des éléments de *darr dans res
    while ((*darr)[i] != NULL) {
        if (strcmp((*darr)[i], sep) == 0) {
            break;
        }
        
        char **tmp = realloc(res, (i+2) * sizeof(char *));
        if (tmp == NULL) {
            print_err("realloc res échoué : split_sep()");
            // Libération de la mémoire allouée précédemment
            for (int j = 0; j < i; j++) {
                free(res[j]);
            }
            free(res);
            return NULL;
        }
        res = tmp;
        res[i] = strdup((*darr)[i]);
        if (res[i] == NULL) {
            print_err("strdup échoué : split_sep()");
            // Libération de la mémoire allouée précédemment
            for (int j = 0; j <= i; j++) {
                free(res[j]);
            }
            free(res);
            return NULL;
        }
        i++;
    }

    // ajoute NULL à la fin de res
    res[i] = NULL;

    if ((*darr)[i] != NULL) {
        *darr += i + 1; // Avance le pointeur pour sauter le séparateur
    } else {
        *darr += i; // Si on est à la fin, déplace juste après le dernier élément
    }

    return res;
}

void dbl_free(char **darr) {
    for (int i = 0; darr[i] != NULL; i++) {
        free(darr[i]);
    }
    free(darr);
}
