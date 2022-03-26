#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../AssemblerFirstIteration/symbol_list.h"
#include "../Hardware/hardware.h"
#include "../AssemblerFirstIteration/command_list.h"

#define MAX_LEN 83
#define NUM_OF_ADDRESSING 4
#define MAX_WORD_SIZE 4
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

/**
* @brief checks if an operand is using immediate addressing
* @param line pointer to the line to check
* @param number pointer to the number addressed
* @param flag_register pointer to the flag register
* @return true if addressing method is immediate
*/
bool is_immediate(char *line, int *number, PSW *flag_register);

/**
* @brief checks if an operand is using direct addressing
* @param line pointer to the line to check
* @param address pointer to the address
* @param head pointer to the head of the symbol list
* @return true if addressing method is direct
*/
bool is_direct(char *line, int *address, symbol_list *head);

/**
* @brief checks if an operand is using index addressing
* @param line pointer to the line to check
* @param label pointer to a label
* @param flag_register pointer to the special flag register
* @param line_number number of current line
* @return true if addressing method is index
*/
bool is_index(char *line, char *label, int *index, PSW *flag_register, int line_number);

/**
* @brief checks if an operand is using register-direct addressing
* @param line pointer to the line to check
* @param number the number addressed
* @param flag_register pointer to the special flag register
* @return true if operand is using register-direct addressing
*/
bool is_register_direct(char *line, int *number, PSW *flag_register);

/**
* @brief throw an error message
* @param message the error
* @param line_number the no. of the line in which the error occured
*/
void throw_error(char *message, int line_number);

/**
* @brief encoding an immediate operand
* @param num the immediate number
* @return the encoded operand
*/
char *encode_immediate(int num);

/**
* @brief encoding the opcode
* @param action_index index of the action in the action table
* @return the encoding of the opcode
*/
char *encode_command_opcode(int action_index);

/**
* @brief encoding the registers
* @param src
* @param dst
* @param action_index index of the action in the action table
* @param src_addressing
* @param dst_addressing
* @param two_operands true if the command has two operands
*/
char *encode_command_registers(int src, int dst, int action_index, int src_addressing, int dst_addressing, bool two_operands);

/**
* @brief getting a number from a string
* @param num the number
* @param flag_register a pointer to the special flag register
* @return the number from the string
*/
int getNumber(char *num, PSW *flag_register);

/**
* @brief checks whether a line is empty
* @param line pointer to the line to check
* @return true if empty
*/
bool is_empty_line(char *line);

/* Execution functions */
/**
* @brief this function parses a command line
* @param line the line to parse
* @param head pointer to the start of the symbol list
* @param action_index index of the action in the action table
* @param line_number the number of the current line
* @param IC pointer to the instruction counter register
* @param DC pointer to the data counter register
* @param flag_register pointer to the special flag register
* @param command_head pointer to the head of the command list
*/
void parse_command(char *line, symbol_list *head, int action_index, int line_number, hregister *IC, hregister *DC, PSW *flag_register, command_list *command_head);

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
