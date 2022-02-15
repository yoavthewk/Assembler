#ifndef LINE_PARSING_H
#define LINE_PARSING_H

#include <ctype.h>
#include "../FileHandling/files.h"
#include "../MacroLinkedList/macro_list.h"

void copyMacroToFile(MacroList* head, char* macroName, char* filename);
bool isMacro(MacroList* head, char* input, FILE* fp, char* filename);
void addMacroToTable(MacroList* head, FILE* fp, char *name);
bool containsName(MacroList* macroTableHead, char* name);
char* parse_line(char* line);
bool containsSpace(char* name);

/*
 * This function pre-assembles the input file.
 * Parameters:
 * char* file_name - the name of the input file.
 * FILE* fp - the file pointer of the input file.
 * head - the head of the macro list.
 */
void preassemble(char *file_name, FILE *fp, MacroList *head);

#endif /* LINE_PARSING_H */