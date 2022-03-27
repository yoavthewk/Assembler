#ifndef LINE_PARSING_H
#define LINE_PARSING_H
#include "hardware.h"
#include <string.h>
#include <ctype.h>

#define MAX_LEN 83

/**
 * @brief this function parses the line for the macro parsing
 * 
 * @param line the line to parse.
 * @return char* the parsed line.
 */
char* parse_line(char* line);

/**
 * @brief this function parses the line for the first iteration (is also used in the second)
 * 
 * @param line the line to parse.
 * @param flagRegister the flag register.
 * @return char* the parsed line.
 */
char *parse_line_first_iteration(char *line, PSW *flagRegister);

#endif /* LINE_PARSING_H */