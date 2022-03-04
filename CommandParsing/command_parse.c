#include "exec.h"

int isWhiteSpaceOnly(char *input){
	return -1;
}

int getNumber(char* num){
	int i = 0;
	int j = 0;
	char number[MAX_LEN];
	while(isdigit(num[i]) || num[i] == '.' || num[i] == '-') number[j++] = num[i++];

	if(j == 0){
        /* alert error */
		return -1;
	}
	return atoi(number);
}

bool isImmediate(char* line, int *number, bool first){
	char* tok;
	tok = strtok(line, first ? &',' : &'\n');
	/* check if it starts with a # */
	if(tok[0] != '#') return false;
	strcpy(tok, tok + 1); /* skip over it */
	
	*number = getNumber(tok); /* get the argument */
	if(*number == -1){
		return false;
	}

	strcpy(line, line + strlen(tok));
	return true;
}

bool isDirect(char* line, int *address){
	char* tok;
	tok = strtok(line, first ? &',' : &'\n');
	if(tok != NULL && contains){
		/* if it is: return address somehow */
		strcpy(line, line + strlen(tok));
		/* else alert error */
	}
	
	return false;
}

bool isIndex(char* line, char* label, int *index){
	char* tok;
	tok = strtok(line, "[");
	if(tok != NULL){
		/* check if tok is in the symbol table */
		/* if it is: */
		label = tok;
		strcpy(line, line + strlen(tok) + 1); /* skip the label and the [ - risky */
		tok = strtok(line, "]");
		if((*index = getNumber(tok)) == -1){
			/* index is illegal */
			/* throw an error and flag */
			return false;
		}
		strcpy(line, line + strlen(tok));
		return true;
	}
	else{
		/* throw an error and raise flag */
		return false;
	}
}

bool isRegisterDirect(char* line, int *number){
	char* tok;
	int num;
	tok = strtok(line, first ? &',' : &'\n');
	if(tok[0] == 'r'){
		strcpy(tok, tok + 1);
		if((*number = getNumber(tok)) != -1){
			return true;
		}
	}
	/* alert error */
	return false;
}

void throw_error(char* message, int line_number){
	printf("%d: %s\n", line_number, message);
}
