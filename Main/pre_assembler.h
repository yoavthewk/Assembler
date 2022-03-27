#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include "../Utility/files.h"
#include "../Utility/macro_list.h"
#include "../Utility/hardware.h"
#include "../Utility/line_parsing.h"
#include <ctype.h>

void copy_macro_to_file(macro_list* head, char* macroName, char* filename);
bool is_macro(macro_list* head, char* input, FILE* fp, char* filename);
void add_macro_to_table(macro_list* head, FILE* fp, char *name);
bool contains_name(macro_list* macroTableHead, char* name);
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