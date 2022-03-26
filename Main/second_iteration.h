#include "first_iteration.h"

/**
 * @brief checks if the line is data declaration.
 *
 * @param line the line to check.
 * @param flag_register the flag register.
 * @return true if the line is data declaration.
 */
bool is_data(char *line, PSW *flag_register);

/**
 * @brief handles entry declaration by updating the symbol list.
 *
 * @param line the line to check.
 * @param head the head of the symbol list.
 * @param flag_register the flag register.
 * @param line_number the current line number.
 */
void handle_entry(char *line, symbol_list *head, PSW *flag_register, int line_number);

/**
 * @brief Open an object file
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_object_file(char *file_name);

void second_line_process(FILE *fp, char *line, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register);

/**
 * @brief this function creates a newly allocated substring from [start] to [end]
 * @param line substring origin
 * @param start the start index
 * @param end the end index
 * @return a newly allocated substring
 */
char *get_new_substring_with_indexes(char *line, int start, int end)

/**
 * @brief this function coverts a binary string to special base
 * @param line the binary string
 * @return special base string 
 */
char *special_base(char *line)
