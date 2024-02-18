#include "redirections.h"

// Tous les types de redirection.
char *specials[] = { "<", ">", ">|", ">>", "2>", "2>|", "2>>", "&" };

int std_fd[3] = { STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO };
int red_fd[3] = { STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO };


int get_specials(char *str) {
    for (int i = 0; i < 8; i++) {
		if (strcmp(str, specials[i]) == 0) {
			return i;
		}
    }
    return -1;
}

int check_special(char** argv){
	int flag = 0; 

	int i;
    for (i = 1; argv[i] != NULL; i++) {
		int red = get_specials(argv[i]);
		
		if (red == 7) {
			argv[i] = NULL;
			return 7;
		}
		if (red != -1) {
			if (argv[i+1] != NULL) {
				int rd = apply_redirection(red, argv[i+1]);

				if (rd != 0) {
					return -1;
				}

				if (flag == 1) {
					argv[i-1] = NULL;
					argv[i-2] = NULL;
				} else {
					flag = 1;
				}

				i++;
			}
			else return -2; // err : redirection sans cible
		}

    }
	if (!flag) return 2; // pas de redirections

	argv[i-1] = NULL;
	argv[i-2] = NULL;
    return 0;
}

int apply_redirection(int red, char* fic) {
	int fd;
    switch (red) {
	//cas: "<"
	case 0: fd = open(fic, O_RDONLY);
		if (fd < 0) return 1;
		red_fd[0] = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
		break;

	//cas: ">"        
	case 1: fd = open(fic, O_WRONLY | O_CREAT | O_EXCL, 0664);
		if (fd < 0) return 1;
		if (errno == EEXIST) {
            fprintf(stderr, "bash: %s: cannot overwrite existing file\n", fic);
			return 1;
		}
		red_fd[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		break;

	//cas: ">|"          
	case 2: fd = open(fic, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		if (fd < 0) return 1;
		red_fd[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		break;

	//cas: ">>"     
	case 3: fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0664);
		if (fd < 0) return 1;
		red_fd[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		break;

	//cas: "2>"       
	case 4: fd = open(fic, O_WRONLY | O_CREAT | O_EXCL, 0664);
		if (errno == EEXIST) {
            fprintf(stderr, "bash: %s: cannot overwrite existing file\n", fic);
			return errno;
		}
		if (fd < 0) return 1;
		red_fd[2] = dup(STDERR_FILENO);
		dup2(fd, STDERR_FILENO);
		close(fd);
		break;

	//cas: "2>|"
	case 5: fd = open(fic, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		if (fd < 0) return 1;
		red_fd[2] = dup(STDERR_FILENO);
		dup2(fd, STDERR_FILENO);
		close(fd);
		break;

	//cas: "2>>"
	case 6: fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0664);
		if (fd < 0) return 1;
		red_fd[2] = dup(STDERR_FILENO);
		dup2(fd, STDERR_FILENO);
		close(fd);
		break;

	default: perror ("No redirection, please try again !");
		return 1;
    }
    return 0;
}

void resetstd() {
    dup2(red_fd[0], std_fd[0]);
    dup2(red_fd[1], std_fd[1]);
    dup2(red_fd[2], std_fd[2]);

	if (red_fd[0] != STDIN_FILENO) {
		close(red_fd[0]);
	}
	if (red_fd[1] != STDOUT_FILENO) {
		close(red_fd[1]);
	}
	if (red_fd[2] != STDERR_FILENO) {
		close(red_fd[2]);
	}
}
