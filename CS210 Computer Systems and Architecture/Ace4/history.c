#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define IN 513
#define HISTORYLENGTH 20
#define HISTORYFILE ".hist_list"
#define HISTORY_ERROR -1
#define HISTORY_SUBSTITUTED 0
#define HISTORY_SAVED 1
#define HISTORY_NOOP 2

/* Used in shell implementation */
int loadHistory();
int saveHistory();
int printHistory(char* tokens[]);
int clearHistory();
int handleHistory(char *cmd, int saveHistory);

/* Internal functions */
int substituteCmd(char *cmd);
int isHistoryInvocation(char* cmd);
int addHistoryCommand(char *cmd);
char* getHistoryCommand(int id);

int getIdx(int id);
int getStartIdx();

typedef struct {
    int id;
    char* command;
} h_command;

h_command commands[HISTORYLENGTH];
int nextId = 1;
char lastHistoryCmd[IN];

int loadHistory() {	
	char fPath[256];
    strcat(strcat(strcpy(fPath, getenv("HOME")), "/"), HISTORYFILE);
    FILE *f = fopen(fPath, "a+b");

    if (f == NULL) {
        fprintf(stderr, "Error opening history file: ");
        perror("");
        return -1;
    }

    int id;
    char cmd[IN];
    char input[IN];
    
    while (fgets(input, IN, f)) {

        int matched = sscanf(input, "%d %[^\n]", &id, cmd);
        if (matched != 2) {
            fclose(f);
            clearHistory();
            return;
        }
        
        int idx = getIdx(id);

        commands[idx].id = id;
        commands[idx].command = (char *) malloc(strlen(cmd) + 1);
        strcpy(commands[idx].command, cmd);
        strcat(commands[idx].command, "\n");

        nextId = id + 1;
    }

    fclose(f);
}

int saveHistory() {
	char fPath[256];
    strcat(strcat(strcpy(fPath, getenv("HOME")), "/"), HISTORYFILE);

    /* Make sure it works after we cd in shell */
    FILE *f = fopen(fPath, "wb");
    
    if (f == NULL) {
        fprintf(stderr, "Error saving history: ");
        perror("");
        return -1;
    }

    int i;
    int stIdx = getStartIdx();
    int startId = commands[stIdx].id;
    for (i = 0; i < HISTORYLENGTH; i++) {
        int id = startId + i;
        
        if (id == 0) {
            /* Empty history, keep it that way */
            break;
        }

        int idx = getIdx(id);
        if (commands[idx].id != 0) {
            fprintf(f, "%d %s", id, commands[idx].command);
        }
    }

    fclose(f);
}

int printHistory(char* tokens[]) {
    if (tokens[1] != NULL) {
        printf("Error: Command has no parameters\n");
        return -1;
    }

    if (nextId == 1) {
        printf("History is empty\n");
        return 0;
    }

    int id = commands[getStartIdx()].id;
    for (; id < nextId; id++) {
        h_command cmd = commands[getIdx(id)];
        printf("%3d  %s", cmd.id, cmd.command);
    }

    return 0;
}

/**
 * Save command to history or invoke history command
 * returns HISTORY_ERROR if error occured anywhere
 *         HISTORY_SUBSTITUTED if command was substituted to one from history
 *         HISTORY_SAVED if command was saved to history
 *         HISTORY_NOOP if command is not history invocation and was not saved in history
 */
int handleHistory(char *cmd, int saveHistory) {
    int isHistCmd = isHistoryInvocation(cmd);
    if (isHistCmd == 0) {
        if (saveHistory == 1) {
            addHistoryCommand(cmd);
            return HISTORY_SAVED;
        }
        return HISTORY_NOOP;
    } else if (isHistCmd == 1) {
        return substituteCmd(cmd);
    }
}

int substituteCmd(char *cmd) {
    int id = -1;

    if (strcmp(cmd, "!!\n") == 0 || strcmp(cmd, "!!") == 0) {
        id = nextId - 1;
    } else {
        int cmdNumber;
        char *endptr;
        cmdNumber = strtol(cmd + 1, &endptr, 10);
        
        if (errno != 0 && cmdNumber == 0) {
            perror("strtol");
            return HISTORY_ERROR;
        } else if (cmdNumber == 0) {
            printf("Invalid command. History invocation command should specify number \"![-]<no>\"\n");
            return HISTORY_ERROR;
        } else if (strlen(endptr) > 1) {
            printf("Invalid command. History invocation command format is \"![-]<no>\"\n");
            return HISTORY_ERROR;
        }

        if (cmdNumber < 0) {
            id = nextId + cmdNumber;
        } else {
            id = cmdNumber;
        }
    }

    char *historyCmd = getHistoryCommand(id);

    if (historyCmd != NULL) {
        strcpy(cmd, historyCmd);
        printf("%s", cmd);
        return HISTORY_SUBSTITUTED;
    } else {
        return HISTORY_ERROR;
    }
}

int isHistoryInvocation(char* cmd) {
    if (cmd == NULL || strcmp(cmd, "\n") == 0)
        return -1;

    if (strspn(cmd, "!") > 0 ) {
        return 1;
    }

    return 0;
}

int addHistoryCommand(char *cmd) {
    int idx = getIdx(nextId);
    commands[idx].id = nextId;
    commands[idx].command = (char *) malloc(strlen(cmd) + 1);
    strcpy(commands[idx].command, cmd);
    nextId++;
}

char* getHistoryCommand(int id) {
    if (nextId == 1) {
        fprintf(stderr, "Error: History is empty\n");
        return NULL;
    }

    if (id < 1 || nextId - id > HISTORYLENGTH || id >= nextId) {
        int stIdx = getStartIdx();
        fprintf(stderr, "Error: History contains commands %d - %d\n", commands[stIdx].id, nextId - 1);
        return NULL;
    }

    int idx = getIdx(id);
    return commands[idx].command;
}

int getIdx(int id) {
    return (id - 1) % HISTORYLENGTH;
}

int getStartIdx() {
    return (nextId - HISTORYLENGTH - 1 < 0) ? 0 : (nextId - HISTORYLENGTH - 1) % HISTORYLENGTH;
}


int clearHistory(){
      
	int id = commands[getStartIdx()].id;
    	for (; id < nextId; id++) {
        	commands[getIdx(id)].command = NULL/*?*/;
  		commands[getIdx(id)].id = 0;
		/*loop through the array and set all items to 0/ maybe null??*/

		

	}
	
	nextId=1;
		
                    

}

