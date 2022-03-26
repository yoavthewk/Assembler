#include "firstIteration.h"

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
void handle_entry(char *line, SymbolList* head, PSW* flag_register, int line_number);