#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "history.c"
#include "alias.c"

#define IN 513
#define MAXTOKENS 50
#define TOKENLEN 15

char input[IN], *inTokens[MAXTOKENS], *token;
const char delim[8] = {' ','>','<',';','\t', '|','&','\n'};

typedef struct command_node {
	char command[IN];
	struct command_node* next;
} command_node_t;

char *originalPath;
char *originalHome;

void prepareshell();
void prepareEnvironment();
void shellexit();

int builtInCommands(char* tokens[MAXTOKENS]);
int exitcmd(char* tokens[MAXTOKENS]);
int getpath(char* tokens[MAXTOKENS]);
int setpath(char* tokens[MAXTOKENS]);
int cd(char* tokens[MAXTOKENS]);

char* trim(char* str);

int main() {
	prepareshell();

	while (1) {
		printf("> ");

		/* CTRL + D handling */
		if (fgets(input, IN, stdin) == NULL) {
			printf("\n");
			shellexit();
		}

		trim(input);

		int validCmd = 1;

		int firstTime = 1; /* Flag for history handling whether to save the command in history */
		int changed = 1;   /* Flag whether alias was substituted by command */
		
		command_node_t commandList;
		strcpy(commandList.command, input);
		commandList.next = NULL;

		while (changed) {
			changed = 0;

			/* History handling */
			int status = handleHistory(input, firstTime);
			if (status == HISTORY_ERROR) {
				/* Invalid arguments or enter pressed */
				validCmd = 0;
				break;
			} else if (status == HISTORY_SUBSTITUTED) {
				if (addCommandChain(&commandList, input) == -1) {
					/* Break out of while loop and dont continue on execution */
					validCmd = 0;
					break;
				}
			}

			/* Alias handling */
			status = handleAlias(input);
			if (status == ALIAS_ERROR) {
				/* Invalid arguments or enter pressed */
				validCmd = 0;
				break;
			} else if (status == ALIAS_SUBSTITUTED) {
				if (addCommandChain(&commandList, input) == -1) {
					/* Break out of while loop and dont continue on execution */
					validCmd = 0;
					break;
				}
				changed = status;
			}

			firstTime = 0;
		}
		
		if (validCmd == 0) {
			/* Dont try to parse and execute already invalid command */
			continue;
		}
		
		/* Input parsing */
		token = strtok(input, delim);
		int i = 0;
		while (token != NULL) {
			inTokens[i] = (char *) malloc(strlen(token) + 1);
			strcpy(inTokens[i], token);
			token = strtok(NULL, delim);
   			i++;
		}
		inTokens[i] = NULL;

		/* Command handling */
		if (builtInCommands(inTokens) == -1) {
			pid_t pid;
			pid = fork();
			if (pid < 0) {
				fprintf(stderr, "Fork failed!\n");
				exit(-1);
			} else if (pid == 0) {
				if (execvp(inTokens[0], inTokens) == -1) {
					fprintf(stderr, "Command \"%s\" not found: ", inTokens[0]);
					perror("");				
					exit(-1);
				}
			} else {
				int status;
				wait(&status);
			}
		}
	}

	return 0;
}

int addCommandChain(command_node_t* userCommand, char* input) {
	/* Loop and check if there are same occurrences */
	command_node_t* current;
	current = userCommand;

	int maxChainLength = 20;
	int i = 0;
	while (current != NULL) {
		i++;
		if (i - 1 == maxChainLength) {
			printf("Error: command chain to long\n");
			return -1;
		}
		if (strcmp(current->command, input) == 0) {
			printf("Error: loop in command chain\n");
			return -1;
		}
		/* We want last node after loop, so break now */
		if (current->next == NULL) 
			break;
		current = current->next;
	}
	
	current->next = malloc(sizeof(command_node_t));
	strcpy(current->next->command, input);
	current->next->next = NULL;
	
	return 0;
}

void prepareshell() {
	prepareEnvironment();
	loadHistory();
	loadAlias();
}

void prepareEnvironment() {
	size_t size;
	size = pathconf(".", _PC_PATH_MAX);;
	char *curHome = (char *)malloc((size_t)size);

	originalPath = getenv("PATH");
	printf("PATH: %s\n", originalPath);

	originalHome = getenv("HOME");
	printf("HOME: %s\n", originalHome);

	getcwd(curHome, size);
	printf("Old directory: %s\n", curHome);

	if (chdir(originalHome) == -1) {
		perror("Error occurred while changing working directory\n");
		shellexit();
	}

	getcwd(curHome, size);
	printf("New directory: %s\n", curHome);
}

/** 
 * Handle custom functions
 * return 0 if command was recognized 
 *       -1 otherwise
 */
int builtInCommands(char* tokens[MAXTOKENS]) {
	char *cmd = tokens[0];

	if (cmd == NULL) {
		/* Enter pressed - do nothing */
		return 0;
	}

	if (strcmp(cmd, "exit") == 0) {
		exitcmd(tokens);
		return 0;
	}

	if (strcmp(cmd, "getpath") == 0) {
		getpath(tokens);
		return 0;
	} 

	if (strcmp(cmd, "setpath") == 0) {
		setpath(tokens);
		return 0;
	}

	if (strcmp(cmd, "cd") == 0) {
		cd(tokens);
		return 0;
	}

	if (strcmp(cmd, "history") == 0) {
		printHistory(tokens);
		return 0;
	}
	if (strcmp(cmd, "clearhistory") == 0) {
		clearHistory();
		return 0;
	}
	
	if (strcmp(cmd, "alias") == 0) {
		alias(tokens);
		return 0;
	}
	if (strcmp(cmd, "unalias") == 0) {
		removeAlias(tokens);
		return 0;
	}

	return -1;
}

void shellexit() {
	if (setenv("PATH", originalPath, 1) == -1) {
		perror("Error: PATH was not restored\n");	
	} else {
		printf("Restored PATH: %s\n", getenv("PATH"));
	}
	
	saveHistory();
	saveAlias();
	exit(0);
};

int exitcmd(char* tokens[MAXTOKENS]) {
	if (tokens[1] != NULL) {
		fprintf(stderr, "Error: Command has no parameters\n");
		return -1;
	}

	shellexit();
}

int getpath(char* tokens[MAXTOKENS]) {
	if (tokens[1] != NULL) {
		fprintf(stderr, "Error: Command has no parameters\n");
		return -1;
	}

	printf("%s\n", getenv("PATH"));
	return 0;
}

int setpath(char* tokens[MAXTOKENS]) {
	if (tokens[1] == NULL || tokens[2] != NULL) {
		fprintf(stderr, "Error: Command should have exactly 1 parameter\n");
		return -1;
	}

	if (setenv("PATH", tokens[1], 1) != 0) {
		perror("Error: PATH was not set\n");
		return -1;
	}

	return 0;
}

int cd(char* tokens[MAXTOKENS]) {
	char* path;

	if (tokens[1] == NULL) {
		path = (char *) malloc(strlen(originalHome) + 1);
		strcpy(path, originalHome);
	} else if (tokens[2] == NULL) {
		path = (char *) malloc(strlen(tokens[1]) + 1);
		strcpy(path, tokens[1]);
	} else {
		fprintf(stderr, "Error: Command \"cd\" should have at most 1 argument\n");
		return -1;
	}

	if (chdir(path) == -1) {
		perror("Working directory wasn't changed");
		return -1;
	}

	return 0;
}

char* trim(char* str) {
	if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }
	const char delim[7] = {' ','>','<',';','\t', '|','&'};

	char* temp = (char *) malloc(strlen(str) + 1);
	strcpy(temp, str);
	while (strchr(delim, *temp) != NULL)
		temp++;
	char *end;
	end = temp + strlen(temp) - 2;	/* last two is \n\0 */
	while (end > temp && strchr(delim, *end) != NULL) {
		end--;
	}
	*(end + 1) = '\n';
	*(end + 2) = '\0';
	strcpy(str, temp);
	
	return str;
}