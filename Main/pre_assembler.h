#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include "../Utility/files.h"
#include "../Utility/macro_list.h"
#include "../Utility/hardware.h"
#include "../Utility/line_parsing.h"
#include <ctype.h>

/*
 * This function replaces a macro name with its content inside the assembly file
 * Input: the macro name, the file name, and the macro list
 * Output: none.
 */
void copy_macro_to_file(macro_list* head, char* macroName, char* filename);

/*
 * This function checks whether a given line contains a macro
 * Input: a specific line in the program
 * Output: Boolean, true if macro.
 */
bool is_macro(macro_list* head, char* input, FILE* fp, char* filename);

/*
 * This function adds a new macro to the macro table
 * Input: the macto list, the assembly file (to get the content of the macro), and the macro name
 * Output: none.
 */
void add_macro_to_table(macro_list* head, FILE* fp, char *name);

/*
 * This function checks whether the macro table contains a given macro
 * Input: a macro name, and the macro table head
 * Output: Boolean, true if in the list.
 */
bool contains_name(macro_list* macroTableHead, char* name);

/*
 * This function checks if a possible macro name has a space.
 * Input: possible name.
 * Output: Boolean, true if it does, false elsewise.
 */
bool has_space(char* name);



/**
 * @brief This function takes a line from the preassembled file and removes spaces after the ','
 * example: mov a, b -> mov a,b
 * @param line the given line
 * @return char* the changed line
 */
char *parse_line_first_iteration(char *line, PSW *flagRegister);

/*
 * This function pre-assembles the input file.
 * Parameters:
 * char* file_name - the name of the input file.
 * FILE* fp - the file pointer of the input file.
 * head - the head of the macro list.
 */
void preassemble(char *file_name, FILE *fp, macro_list *head);

#endif /* PRE_ASSEMBLER_H */