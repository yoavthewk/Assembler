#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include "../FileHandling/files.h"
#include "../MacroLinkedList/macro_list.h"
#include "../Hardware/hardware.h"
#include <ctype.h>

void copyMacroToFile(MacroList* head, char* macroName, char* filename);
bool isMacro(MacroList* head, char* input, FILE* fp, char* filename);
void addMacroToTable(MacroList* head, FILE* fp, char *name);
bool containsName(MacroList* macroTableHead, char* name);
char* parse_line(char* line);
bool containsSpace(char* name);

/**
 * @brief This function creates a new dynamicly-allocated char* that consists of the given line,
 * but only from a specific index.
 * @param line the line to get substring from
 * @param start index of the first char
 * @return char* - substring from index until the end.
 */
char* getNewSubstringFrom(char* line, int start);

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
void preassemble(char *file_name, FILE *fp, MacroList *head);

/**
 * @brief Get a new substring from given line, from position start to end (exclusive)
 * @param line the input line
 * @param start the start of the substring
 * @param end the end of the substring
 * @return char* 
 */
char* getNewSubstringWithIndexes(char* line, int start, int end);

#endif /* PRE_ASSEMBLER_H */