#include "first_iteration.h"


/**
 * @brief checks if the line is data declaration.
 * 
 * @param line the line to check.
 * @param flag_register the flag register.
 * @return true if the line is data declaration.
 */
bool is_data(char *line, PSW* flag_register);

/**
 * @brief handles entry declaration by updating the symbol list.
 * 
 * @param line the line to check.
 * @param head the head of the symbol list.
 * @param flag_register the flag register.
 * @param line_number the current line number.
 */
void handle_entry(char *line, symbol_list *head, PSW *flag_register, int line_number);

void write_value_offset(FILE* fp, symbol_list *head, char* label_name);

void second_line_process(FILE *fp, char *line, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register);

void write_entry_to_file(FILE* fp, symbol_list *head);

void write_extern_to_file(FILE* fp, symbol_list *head);

bool need_completion(command_list *head, int IC);

void fill_command_list(symbol_list *head, command_list *command_head, PSW* flag_register, char* label, int IC);