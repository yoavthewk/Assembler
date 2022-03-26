#ifndef FIRSTITERATION_H
#define FIRSTITERATION_H
#include "../Hardware/hardware.h"
#include "symbol_list.h"
#include "../PreAssembler/pre_assembler.h"
#include "../CommandParsing/exec.h"
#include "command_list.h"

#define BASE 0
#define OFFSET 0

void firstIteration(char *file_name, FILE *fp, symbol_list* head, hregister* IC, hregister* DC, PSW* flagRegister, command_list *commad_head);

/* Helper Functions */

/**
* @brief checks if a label is valid.
*
* @param label label to check.
* @param head head of the symbol list.
* @return true if the label given is valid.
*/
bool isValidLabel(char* label, symbol_list* head);

/**
* @brief checks if the line contains label declaration.
*
* @param line line to check.
* @param head head of the symbol list.
* @param line_number the number of the current line.
* @param flag_register the flag register.
*/
void contains_label(char *line, symbol_list* head, int line_number, PSW *flag_register);

/**
* @brief checks if there's a declaration of an extern in the line.
*
* @param line line to check.
* @param flag_register the flag register.
* @return true if there's a declaration of an extern.
*/
bool is_extern(char *line, PSW *flag_register);

/**
* @brief checks if theres a declaration of an entry in the line.
*
* @param line line to check.
* @param flag_register the flag register.
* @return true if there's a declaration of an entry.
*/
bool is_entry(char *line, PSW *flag_register);

/**
* @brief processes the data from the line, validates it and encodes it.
*
* @param line line to process.
* @param DC the data count.
* @param line_number the number of the current line.
* @param flag_register the flag register.
* @param head the head of the command list.
* @return true if there's a declaration of an entry.
*/
void process_data(char *line, hregister* DC, int line_number, PSW *flag_register, command_list *head);

/**
* @brief handles a line with data initialization.
*
* @param line line to handle.
* @param head head of the symbol list.
* @param DC the instruction count.
* @param DC the data count.
* @param line_number the number of the current line.
* @param flag_register the flag register.
* @param command_head the head of the command list.
* @return true if there's data initialization.
*/
bool handle_data(char *line, symbol_list *head, hregister* IC, hregister* DC, int line_number, PSW *flag_register, command_list *command_head);

/**
 * @brief processes a command line, validates it, analyzes it and encodes it accordingly if it is valid. 
 *        If it isn't it throws an error.
 * 
 * @param line the line to process
 * @param head the head of the symbol list.
 * @param line_number the number of the current line.
 * @param IC the instruction count.
 * @param DC the data count.
 * @param flag_register the flag register.
 * @param command_head the head of the command list.
 */
void process_line(char *line, symbol_list *head, int line_number, hregister* IC, hregister* DC, PSW* flag_register, command_list *command_head);

/**
 * @brief handles a line with extern declaration by checking whether it's valid and if so
 *        adding it to the symbol list.
 * 
 * @param line the line to handle.
 * @param head the head of the symbol list.
 * @param line_number the number of the current line.
 * @param flag_register the flag register.
 */
void handle_extern(char *line, symbol_list *head, int line_number, PSW *flag_register);

/**
 * @brief updates the symbol list after the first iteration by re-adjusting the value, offset 
 *        and base address of data lines (value --> value + ICF).
 * 
 * @param head the head of the symbol list.
 * @param IC the instruction count.
 */
void update_symbol_list(symbol_list* head, hregister* IC);

/**
 * @brief checks whether there's an extraneous text error in the line.
 * 
 * @param line the line to check.
 * @param operands the number of operands of the command.
 * @param flag_register the flag register.
 * @param line_number the current line number
 * @return true if there is an extraneous text error.
 */
bool extraneous_text(char *line, int operands, PSW *flag_register, int line_number);

/**
 * @brief checks for illegal commas error in the line.
 * 
 * @param line the line to check.
 * @param flag_register the flag register.
 */
void check_illegal_commas(char* line, PSW* flag_register);

/**
 * @brief checks if the data contains a space
 * 
 * @param data the data to check.
 * @param flag_register the flag register.
 * @return true if it contains a space.
 */
bool contains_space(char* data, PSW* flag_register);

/**
 * @brief updates the command list after the first iteration by re-adjusting 
 *        the IC of data lines (IC --> IC + ICF).
 * 
 * @param head the head of the command list.
 * @param IC the final instruction count.
 */
void update_command_list(command_list *command_head, hregister* IC);

#endif /* FIRSTITERATION_H */
