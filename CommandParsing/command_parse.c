#include "exec.h"

int getNumber(char *num, PSW* flagRegister)
{
	int i = 0;
	int j = 0;
	char number[MAX_LEN] = {0};
	while ((isdigit(num[i]) || num[i] == '-' || num[i] == '+') && num[i] != ',')
		number[j++] = num[i++];

	if (j == 0 || num[i] == '.')
	{
		flagRegister->ERR = 1;
		return -1;
	}

	return atoi(number);
}

bool isImmediate(char *line, int *number, PSW *flagRegister)
{
	char *tok;
	char* binary_line; 

	/* check if it starts with a # */
	if (line[0] != '#')
		return false;
	memmove(line, line + 1, strlen(line)); /* skip over it */

	*number = getNumber(line, flagRegister); /* get the argument */
	if (flagRegister->ERR)
	{
		return false;
	}
	
	binary_line = encode_immediate(*number);
	free(binary_line);
	return true;
}

bool isDirect(char *line, int *address, SymbolList *head)
{
	if (line)
	{
		int i;
    	bool valid = true;
		/* first we check that the first character is a letter */
    	valid = isalpha(line[0]) ? valid : false; 

    	for(i = 1; i < strlen(line) - 1; i++){
        	if(!isalpha(line[i]) && !isdigit(line[i])) valid = false;
    	}
		
		if(valid)
			return true;
	}

	return false;
}

bool isIndex(char *line, char *label, int *index, PSW *flagRegister, int line_number)
{
	char *tok;
	tok = strtok(line, "[");
	if (tok != NULL)
	{
		/* check if tok is in the symbol table */
		strcpy(label, tok);
		/* if it is: */
		tok = strtok(NULL, "]");
		if (tok && tok[0] == 'r')
		{
			memmove(tok, tok + 1, strlen(tok));
			*index = getNumber(tok, flagRegister);
			if(flagRegister->ERR == 1)
			{
				/* index is illegal */
				/* throw an error and flag */
				throw_error("Invalid Index!", line_number);
				return false;
			}
			return true;
		} else if(tok){
			flagRegister->ERR = 1;
			throw_error("Index must be a register!", line_number);
			return false;
		}
	}
	return false;
}

bool isRegisterDirect(char *line, int *number, PSW *flagRegister)
{
	char *tok;
	int num;
	if (line[0] == 'r')
	{
		memmove(line, line + 1, strlen(line));
		*number = getNumber(line, flagRegister);
		if(flagRegister->ERR)
		{
			return false;
		}
		return true;
	}
	return false;
}

void throw_error(char *message, int line_number)
{
	printf("Line %d: %s\n", line_number, message);
}

char* encode_immediate(int num){
	const size_t BITS = 16;
	const size_t ARE_SIZE = 3;
	const size_t A = 1;
	char* bin_str = (char*)malloc(WORD_SIZE);
	unsigned int i, mask;

	/* insert ARE */
	for(i = 0; i < ARE_SIZE + 1; i++){
		bin_str[i] = i == A ? '1' : '0'; 
	}

	/* insert number */
	for(i = 4; i < WORD_SIZE; i++){
		mask = 1u << (BITS - 1 - (i - (ARE_SIZE + 1)));
		bin_str[i] = (num & mask) ? '1' : '0';
	}

	bin_str[WORD_SIZE] = 0;
	return bin_str;
}

char* encode_command_opcode(int action_index){
	char* bin_str = (char*)malloc(WORD_SIZE + 1);
	unsigned int i;

	/* insert opcode */
	for(i = 0; i < WORD_SIZE; i++){
		bin_str[i] = (i == action_table[action_index].op_code) ? '1' : '0';
	}

	/* insert ARE */
	bin_str[WORD_SIZE - 2] = '1';

	/* insert terminator */ 
	bin_str[WORD_SIZE] = 0;

	return bin_str;
}

bool is_empty_line(char *line){
	int i;
	for(i = 0; i < strlen(line); i++){
		if(!isspace(line[i])) return false;
	}
	return true;
}
