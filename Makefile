CC=gcc
CFLAGS=-Iheader -Wall -g
LIBS=-lreadline
COMPILE=$(CC) $(CFLAGS)

# renvoie tous les fichiers .c (séparés par des espaces) dans src
SRC=$(wildcard src/*.c)
# dans $(SRC) remplace tous les textes "src/[filename].c" par "out/[filename].o"
OUT=$(patsubst src/%.c,out/%.o,$(SRC))

all : jsh
	@echo "\tPour exécuter jsh : \001\033[1;35m\002./jsh\001\033[0m\002"

clean :
	@rm -f jsh
	@rm -rf out
	@echo "\t\001\033[34m\002projet nettoyé\001\033[0m\002"
	@echo "\tpour compiler le projet : \001\033[1;35m\002make\001\033[0m\002"


# prend tous les fichiers .o de out et les compile
jsh : $(OUT)
	@$(COMPILE) $(OUT) -o jsh $(LIBS)

# prend tous les fichiers .c de src et en fait des .o dans out
out/%.o : src/%.c
	@mkdir -p out
	@$(COMPILE) -c $< -o $@


