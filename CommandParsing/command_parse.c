#include "exec.h"

int isWhiteSpaceOnly(char *input){
	int i = 0;
	for (i = 0; i < strlen(input); i++)
	{
		if (!isspace(input[i])) {
			return true;
		}
	}
	return false;
}

int getNumber(char* num){
	int i = 0;
	int j = 0;
	char number[MAX_LEN] = {0};
	while((isdigit(num[i]) || num[i] == '-' || num[i] == '+') && num[i] != ',') number[j++] = num[i++];

	if(j == 0 || num[i] == '.'){
		return -1;
	}
	return atoi(number);
}

bool isImmediate(char* line, int *number){
	char* tok;
    
	/* check if it starts with a # */
	if(line[0] != '#') return false;
	memmove(line, line + 1, strlen(line));; /* skip over it */
    
	*number = getNumber(line); /* get the argument */
	if(*number == -1){
		return false;
	}

	return true;
}

bool isDirect(char* line, int *address, SymbolList* head){	
	if(line && contains(head, line)){
		/* if it is: return address somehow */
		memmove(line, line + strlen(line), strlen(line));;
		/* else alert error */
	}
	
	return false;
}

bool isIndex(char* line, char* label, int *index){
	char* tok;
	tok = strtok(line, "[");
	if(tok != NULL){
		/* check if tok is in the symbol table */
		strcpy(label, tok);
		/* if it is: */
		tok = strtok(NULL, "]");
        if(tok && tok[0] == 'r') {
            memmove(tok, tok + 1, strlen(tok));
            if ((*index = getNumber(tok)) == -1) {
                /* index is illegal */
                /* throw an error and flag */
                return false;
            }
            return true;
        }
	}
	return false;
}

bool isRegisterDirect(char* line, int *number){
	char* tok;
	int num;
	if(line[0] == 'r'){
		memmove(line, line + 1, strlen(line));
		if((*number = getNumber(line)) != -1){
			return true;
		}
	}
	return false;
}

void throw_error(char* message, int line_number){
	printf("ERROR: in line %d: %s\n", line_number, message);
}
