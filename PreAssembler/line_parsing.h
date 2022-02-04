#include "../MacroLinkedList/macro_list.h"
#include "../FileHandling/files.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

void copyMacroToFile(MacroList* head, char* macroName, char* filename);
bool isMacro(MacroList* head, char* input, FILE* fp, char* filename);
void addMacroToTable(MacroList* head, FILE* fp, char *name);
bool containsName(MacroList* macroTableHead, char* name);
char* parse_line(char* line);
bool containsSpace(char* name);