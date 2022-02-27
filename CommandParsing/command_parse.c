#include "exec.h"

int main(){
	return 0;
}

/*
	This function reads the line from the user, and executes the right function if it found a known
	command name. Else, it returns an error.
	parameters:
	params - the complex parameters.
*/
void executeCommand(){
	char input[MAX_LEN] = {0};
	char command[MAX_LEN] = {0};
	int i, j;
	i = j = 0;
	
	fflush(stdin);
	
	if(fgets(input, MAX_LEN, stdin) == NULL){
		fprintf(stderr, "EOF Entered.\n");
		exit(0);
	}
	
	input[strcspn(input, "\n")] = 0; /* replace the \n with a null terminator */
	
	printf("%s\n", input);
	
	if(isWhiteSpaceOnly(input)) return;
	
	for(; isalpha(input[i]) || input[i] == '_'; i++){
		command[j++] = input[i]; /* get each character of the command */ 
	}
	command[j] = '\0';
	
	/* Loop over each command, and check if it's name is equal to a known command */
	for(i = 0; cmd[i].func != NULL; i++){
		if(strcmp(command, cmd[i].name)==0)
			break;
	}
	
	/* if it isn't, we print an error */
	if(cmd[i].func == NULL){
		fprintf(stderr, "Command does not exist: %s\n", command);
		return;
	}
	
	/* if it is, we call the function that executes the command */
	else (*(cmd[i].func))(input);
}

/*
	This function checks if a line is only whitespaces.
	parameters:
	input - the input line.
*/
int isWhiteSpaceOnly(char input[]){
	int i = 0;
	while(input[i++] != 0){
		if(!isspace(input[i - 1]))
			return 0;
	}
	return 1;
}
