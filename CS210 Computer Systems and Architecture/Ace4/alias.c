#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define IN 513
#define ALIASLENGTH 10
#define ALIASFILE ".aliases"
#define ALIAS_ERROR -1
#define ALIAS_NOOP 0
#define ALIAS_SUBSTITUTED 1
/*structure of aliases*/
typedef struct {
    char* command;
    char* alias;
} aliasMap;

aliasMap aliases[ALIASLENGTH];

/*counter to keep track of number of aliases*/
int count = 0;

/* Used in shell implementation */
int saveAlias();
int loadAlias();
int alias();
int removeAlias();
int hadleAlias(char *cmd);

/* if no parameters then print aliases, if 1 parameter error, else add alias */
int alias(char* tokens[]){
	if (tokens[1] != NULL && tokens[2] != NULL) {
		/*if there are 2 parameters*/
		int i = 0;
		int param = 3;
		for (i;i<count;i = i+1){
			/*if the new alias has the same name as another aliases then the current alias is overwritten*/
			if (strcmp(aliases[i].alias, tokens[1]) == 0) {
				aliases[i].command = tokens[2];
				/*loop through all of the parameters and concatenate them with command*/
				while(tokens[param] != NULL){
					strcat(aliases[i].command," ");
					strcat(aliases[i].command, tokens[param]);
					param++;		
				}
				printf("The alias %s has been overwritten and now represents %s \n",tokens[1],tokens[2]);
				return 0;
			}
		}
		if (count<10){
			/*the alias is the first parameter, the first part of the command is the 2nd parameter*/
			aliases[count].alias = tokens[1];
			aliases[count].command = tokens[2];
			/*loop through all of the parameters that make up the command and concatenate them*/
			while(tokens[param] != NULL){
				strcat(aliases[count].command," ");
				strcat(aliases[count].command, tokens[param]);
				param++;	
			}							
			count = count+1;
			return 0;
		}
		if (count ==10){
			/*no more aliases can be added*/
			printf("Error: There are already 10 aliases, you cannot use any more\n");
			return -1;
		}	
   	}
	else if (tokens[1] == NULL) {
		/*if there are no parameters then print aliases*/
		/*Print Aliases*/
		if (count ==0){
			/*if there are no aliases*/
			printf("No aliases have been assigned \n");
			return 0;		
		}
		else{
			int i=0;
			/*loop through all aliases and print them*/
			for (i;i<count;i++){
				printf("%d alias: %s = command: %s \n",i+1,aliases[i].alias,aliases[i].command);	
			}
			return 0;
		}
	}
	else{
		printf("command should either have 0 or more than 1 parameters\n");
		return -1;	
	}
}

int removeAlias(char* tokens[]){
	if (tokens[1] == NULL) {
		/*no parameters passed in*/
		fprintf(stderr, "Error: Command has 1 parameter\n");
		return -1;
	}
	else if (tokens[1] != NULL && tokens[2] == NULL) {
		/*only 1 parameter passed in*/	
		/*remove alias*/
		if(count == 0 ){
			printf("Error: There are no aliases to remove\n");
		}
		else{
			int i = 0;
			for (i;i<=count -1;i = i+1){

				/*if alias is found*/
				if (strcmp(aliases[i].alias, tokens[1]) == 0) {
					/*Remove Alias*/
					for (i;i<=count -1;i = i+1){					
						aliases[i] = aliases[i+1];	
					}
					count = count -1;
					printf("Alias %s has been removed \n", tokens[1]);
					return 0;				
				}
			}
			/*if that alias doesnt exist -- this line will only be reached if alias isnt there*/
			printf("Error: The string %s is not an alias \n", tokens[1]);
			return -1;
		}
	}
	else{
		/*user entered 0 or more than 1 parameter*/
		fprintf(stderr, "Error: Command has 1 parameter\n");
		return -1;
    	}

}

/**
 * Changing the input
 * returns ALIAS_NOOP if alias not found
 * 		   ALIAS_SUBSTITUTED if alias was found
 */
int handleAlias(char *cmd) { 
	const char delim[8] = {' ','>','<',';','\t', '|','&','\n'};
	char* command , *inCommands[50];
	command = strtok(cmd, delim);
	int number = 0;

	/* Tokenization */
	while (command != NULL) {
		inCommands[number] = (char *) malloc(strlen(command) + 1);
		strcpy(inCommands[number], command);
		command = strtok(NULL, delim);
		number++;
	}
	
	/* Checking tokens */
	int i = 0;
	int aliasOperation = ALIAS_NOOP;
	for (i;i<count;i = i+1){
		if (strcmp(inCommands[0], aliases[i].alias) == 0) {
			strcpy(inCommands[0],aliases[i].command);
			aliasOperation = ALIAS_SUBSTITUTED;
		}
	}
	
	strcpy(cmd,inCommands[0]);
	i = 1;
	for(i;i<number;i++){
		strcat(cmd, " ");
		strcat(cmd, inCommands[i]);
	}
	
	return aliasOperation;
}

/*runs when exiting shell*/
int saveAlias() {
	char fPath[256];
    strcat(strcat(strcpy(fPath, getenv("HOME")), "/"), ALIASFILE);

    FILE *f = fopen(fPath, "wb");

    if (f == NULL) {
        fprintf(stderr, "Error saving aliases: ");
        perror("");
        return -1;
    }

    int i;
    int stCounter = 0;
    for (i = 0; i < ALIASLENGTH; i++) {
        
        if (count == 0) {
            /* no aliases*/
            break;
        }

      
        if (aliases[i].alias != NULL) {
            fprintf(f, "%s %s\n", aliases[i].alias, aliases[i].command);
        }
    }

    fclose(f);
}
/*runs when shell starts for persistent aliases*/
int loadAlias() {	
	char fPath[256];
    strcat(strcat(strcpy(fPath, getenv("HOME")), "/"), ALIASFILE);
    FILE *f = fopen(fPath, "a+b");

    if (f == NULL) {
        fprintf(stderr, "Error opening aliases file: ");
        perror("");
        return -1;
    }

    
    char cmd[IN];
    char alias[IN];
    char input[IN];
    while (fgets(input, IN, f)) {

        int matched = sscanf(input, "%s %[^\n]s", alias, cmd);
        if (matched != 2) {
            fclose(f);
            count = 0;
            return;
        }
        
		aliases[count].alias = (char *) malloc(strlen(alias) + 1);
        aliases[count].command = (char *) malloc(strlen(cmd) + 1);
        strcpy(aliases[count].command, cmd);
		strcpy(aliases[count].alias, alias);
		
        count = count + 1;
    }

    fclose(f);
}






