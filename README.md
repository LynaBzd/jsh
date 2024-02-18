JSH - A SHELL THAT SUPPORTS JOBS HANDLING
-----------------------------------------

Implémentation d'un shell centré sur la gestion des jobs.

## COMPILATION

```sh
make
```

## EXÉCUTION

```sh
./jsh
```

## NETTOYAGE 

```sh
make clean
```

## UTILISATION
Fonctionne comme un shell en affichant au début du prompt le nombre de jobs
surveillés. Permet d'exécuter des commandes internes (`cd`, `pwd`, `?`, `exit`) 
ainsi que des commandes externes (`ls`, `cat`, `grep`, `find`, `wc`, `head`, `tail`, ...).
avec la possibilité de rediriger les sortie d'une commande vers un fichier,
d'utiliser des pipelines et de mettre en arrière / avant-plan.
