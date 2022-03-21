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
#define REGISTER_DIRECT 1
#define INDEX 2
#define DIRECT 3
#define WORD_SIZE 20
#define EXTERN 0
#define ENTRY 1
#define CODE 2
#define DATA 3

/* Helper functions */
void executeCommand();
bool isImmediate(char *line, int *number, PSW *flagRegister);
bool isDirect(char *line, int *address, SymbolList *head);
bool isIndex(char *line, char *label, int *index, PSW *flagRegister);
bool isRegisterDirect(char *line, int *number, PSW *flagRegister);
void throw_error(char *message, int line_number);
char *encode_immediate(int num);
char *encode_command_opcode(int action_index);
int getNumber(char *num, PSW *flagRegister);
bool is_empty_line(char *line);

/* Execution functions */
void parse_command(char *line, SymbolList *head, int action_index, int line_number, hregister *IC, hregister *DC, PSW *flagRegister);

struct
{
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
	{"lea", 4, 0, 2, {0, 0, 1, 1}, {0, 1, 1, 1}},
	{"clr", 5, 10, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
	{"not", 5, 11, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
	{"inc", 5, 12, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
	{"dec", 5, 13, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
	{"jmp", 9, 10, 1, {0, 0, 0, 0}, {0, 0, 1, 1}},
	{"bne", 9, 11, 1, {0, 0, 0, 0}, {0, 0, 1, 1}},
	{"jsr", 9, 12, 1, {0, 0, 0, 0}, {0, 0, 1, 1}},
	{"red", 12, 0, 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
	{"prn", 13, 0, 1, {0, 0, 0, 0}, {1, 1, 1, 1}},
	{"rts", 14, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{"stop", 15, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{"invalid", 0, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}};
