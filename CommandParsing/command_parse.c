#include "exec.h"

int isWhiteSpaceOnly(char *input){
	
}

void isImmediate(char* line, int* number){
	char* tok;
	tok = strtok(line, " ");
	/* check if it starts with a # */
	if(tok[0] != '#') return;
	strcpy(tok, tok + 1); /* skip over it */
	
	*number = getNumber(tok); /* get the argument */
}

int getNumber(int* num){
	int i = 0;
	int j = 0;
	char number[MAX_LEN];
	while(isdigit(num[i]) || num[i] == '.' || num[i] == '-') number[j++] = num[i++];

	if(j == 0){
		/* alert error and shit */
		return -1;
	}
	return atoi(number);
}

int isDirect(char* line){
	char* tok;
	tok = strtok(line, " ");
	if(tok != NULL){
		/* check if tok is in the symbol table */
		/* if it is: return address somehow */
		/* else alert error */
	}
	else{
		return -1; /* alert error */
	}
	
}

void isIndex(char* line, char* label, int *index){
	char* tok;
	tok = strtok(line, "[");
	if(tok != NULL){
		/* check if tok is in the symbol table */
		/* if it is: */
		*label = tok;
		strcpy(line, line + strlen(tok) + 1); /* skip the label and the [ */
		tok = strtok(line, "]");
		if((*index = getNumber(tok)) == -1){
			/* index is illegal */
			/* throw an error and flag */
		}
	}
	else{
		/* throw an error and raise flag */
	}
}

int isRegisterDirect(char* line){
	char* tok;
	int num;
	tok = strtok(line, " ");
	if(tok[0] == 'r'){
		strcpy(tok, tok + 1);
		if((num = getNumber(tok)) != -1){
			return num;
		}
	}
	/* alert error */
	return -1;
}