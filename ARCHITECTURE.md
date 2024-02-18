# ARCHITECTURE JSH


## Aperçu
Ce projet est une implémentation d'un shell en C, 
centré sur la gestion des jobs. Il permet l'exécution de commandes internes et externes,
les recirections, les pipelines, et la mise en avant / arrière-plan. 


## Architecture logicielle 
Notre projet est structuré de manière à séparer les fichiers d'en-tête (headers) 
et les fichiers source (sources), favorisant ainsi la modularité. 
Voici comment il est organisé :

```
header/
src/
Makefile
```

`Makefile` génère des `.o` dans un dossier `out/` et l'exécutable `jsh` dans le répertoire `.`.

Cette organisation aide à maintenir notre code clair et facile à gérer.


## Structure de données
Voici la structure des fichiers du code source.

```
├── header
│   ├── bg.h
│   ├── cd.h
│   ├── color.h
│   ├── exit.h
│   ├── extern.h
│   ├── fg.h
│   ├── jobs.h
│   ├── kill.h
│   ├── parse.h
│   ├── prompt.h
│   ├── pwd.h
│   ├── questionmark.h
│   ├── redirections.h
│   └── utils.h
├── src
│   ├── bg.c
│   ├── cd.c
│   ├── color.c
│   ├── exit.c
│   ├── extern.c
│   ├── fg.c
│   ├── jobs.c
│   ├── kill.c
│   ├── main.c
│   ├── parse.c
│   ├── prompt.c
│   ├── pwd.c
│   ├── questionmark.c
│   ├── redirections.c
│   └── utils.c
└── Makefile
```


`main.c` : point d'entrée du programme.

`cd.c`, `pwd.c`, `exit.c`, `questionmark.c` : commandes internes.

`extern.c` : commandes externes.

`utils.c` : fonctions utilitaires, nécéssaires pour la manipulation des structures de données.

`jobs.c` : la gestion des tâches.

`parse.c` : analyse et transforme la donnnée dans la ligne de commande donnée par
l'utilisateur dans le but de l'exécuter.

`prompt.c` , `color.c`: gestion de l'affichage du prompt.

`redirection.c` : gestion des redirections.

`kill.c` : gestion des signaux liés à kill.

`fg.c` et `bg.c` : gestion de la mise en arièrre-plan et au premier plan des jobs.


## Data Flow
**Entrée** : 
- la commande saisie par l'utilisateur est lue et stockée dans un `char*`.

**Parsing** : 
- la ligne est divisée avec le séparateur `" "` et stockée sous forme d'un tableau (`char**`).
- elle est analysée par `exeline` pour vérifier si elle contient des symboles de substitution (`<(` et ` )`), puis de pipeline (`|`) puis elle est redirigée vers la fonction qui gère l'un de ces cas. 
Si aucun de ces deux cas n'est détecté, la commande est envoyée dans la fonction `execmd`.
- `execmd` s'occupe de lancer la fonction correspondant à la commande après avoir effectué 
les possibles redirections.

**Exécution** : 
- s'il s'agit d'une commande interne, `jsh` appelle la fonction correspondante.
- s'il s'agit d'une commande externe, `jsh` appelle `execvp`.

**Boucle** :
- après l'exécution de la ligne entière (toutes les commandes qui la composent),
`jsh` attend une nouvelle entrée utilisateur.
