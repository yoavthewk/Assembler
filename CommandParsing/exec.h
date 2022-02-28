#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct{
	char *name;
	void (*func)(char *input);
	}cmd[] = {
		{"mov", mov_exec},
		{"cmp", cmp_exec},
		{"add", add_exec},
		{"sub", sub_exec}, 
		{"lea", lea_exec}, 
		{"clr", clr_exec}, 
		{"not", not_exec}, 
		{"inc", inc_exec}, 
		{"dec", dec_exec},
		{"jmp", jmp_exec},
		{"bne", bne_exec},
		{"jsr", jsr_exec},
		{"red", red_exec},
		{"prn", prn_exec},
		{"rts", rts_exec},
		{"stop", stop_exec},
		{"not_valid", NULL}
	};
	
/* Helper functions */
void executeCommand();
int isWhiteSpaceOnly(char *input);
void isImmediate(char* line, int* number);
int isDirect(char* line);
void isIndex(char* line, char* label, int *index);
int isRegisterDirect(char* line);

/* Execution functions */
void mov_exec(char *input);
void cmp_exec(char *input);
void add_exec(char *input);
void sub_exec(char *input);
void lea_exec(char *input);
void clr_exec(char *input);
void not_exec(char *input);
void inc_exec(char *input);
void dec_exec(char *input);
void jmp_exec(char *input);
void bne_exec(char *input);
void jsr_exec(char *input);
void red_exec(char *input);
void prn_exec(char *input);
void rts_exec(char *input);
void stop_exec(char *input);
