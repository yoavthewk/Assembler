#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../AssemblerFirstIteration/symbol_list.h"
#include "../Hardware/hardware.h"

#define MAX_LEN 83
#define NUM_OF_ADDRESSING 4
#define IMMEDIATE 0 
#define DIRECT 1
#define INDEX 2
#define REGISTER_DIRECT 3
/* Helper functions */
void executeCommand();
int isWhiteSpaceOnly(char *input);
bool isImmediate(char* line, int* number);
bool isDirect(char* line, int *address, SymbolList* head);
bool isIndex(char* line, char* label, int *index);
bool isRegisterDirect(char* line, int *number);
void throw_error(char* message, int line_number);

/* Execution functions */
void parse_command(char* line, SymbolList* head, int action_index, int line_number, hregister* IC, hregister* DC);

struct {
	char name[8];
	unsigned int op_code : 16;
	unsigned int funct : 4;
	unsigned int operands : 2;
	unsigned int first_operand_valid[4];
	unsigned int second_operand_valid[4];
} static action_table[] = {
		{"mov", 0, 0, 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
		{"cmp", 1, 0, 2, {1, 1, 1, 1}, {1, 1, 1, 1}},
		{"add", 2, 10, 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
		{"sub", 2, 11, 2, {1, 1, 1, 1}, {0, 1, 1, 1}}, 
		{"lea", 8, 0, 2, {0, 1, 1, 0}, {0, 1, 1, 1}}, 
		{"clr", 16, 10, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},  
		{"not", 16, 11, 1, {0, 0, 0, 0}, {0, 1, 1, 1}}, 
		{"inc", 16, 12, 1, {0, 0, 0, 0}, {0, 1, 1, 1}}, 
		{"dec", 16, 13, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
		{"jmp", 256, 10, 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
		{"bne", 256, 11, 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
		{"jsr", 256, 12, 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
		{"red", 2048, 0, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
		{"prn", 4096, 0, 1, {0, 0, 0, 0}, {1, 1, 1, 1}},
		{"rts", 8192, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{"stop", 16385, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{"invalid", 0, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};

