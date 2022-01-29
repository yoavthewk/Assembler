#include "../MacroLinkedList/macro_list.h"
#include "../FileHandling/files.h"
#include <ctype.h>
#include <stdbool.h>

bool inMacro = false;

void copyMacroToFile(MacroList* head, char* macroName, char* filename);
bool isMacro();
void addMacroToTable(MacroList* head, FILE* fp, char *name);

int main()
{
    
    return 0;
}

/*
* This function checks whether a given line contains a macro
* Input: a specific line in the program
* Output: Boolean, true if macro.
*/
bool isMacro(MacroList* head, char* input)
{
    int i = 0;
    char* name;
    char *line;
    strcpy(line, input);

    name = strtok(line, "\n");
    if(containsName(head, name))
    {
        copyMacroToFile(name, filename);
        return;
    }

    if (strcmp(strtok(line, " "), "macro")) return false; // if string is not macro
    i += 5;

    strcpy(line, line + i);
    name = strtok(line, "\n");
    // Add the macro to the table.
    inMacro = true;
    return true;
}

/*
* This function checks whether the macro table contains a given macro
* Input: a macro name, and the macro table head
* Output: Boolean, true if in the list.
*/
bool containsName(MacroList* macroTableHead, char* name)
{
    while (macroTableHead){
        if (!strcmp(macroTableHead->m.name, name))
        {
            return true;
        }

        macroTableHead = macroTableHead->next;
    }
    return false;
}

/* 
* This function adds a new macro to the macro table
* Input: the macto list, the assembly file (to get the content of the macro), and the macro name
* Output: none.
*/
void addMacroToTable(MacroList* head, FILE* fp, char *name)
{
    char* content = (char*)malloc(sizeof(char)), line;
	while (head->next)
    {
        head = head->next;
    }

    do {
        line = get_next_line(fp);
        content = (char*)realloc(content, strlen(line) + strlen(content));
        strcat(content, line);
    } while (strcmp(line, "endm"));

    head->next = initNode(NULL, name, content);
}

/*
* This function replaces a macro name with its content inside the assembly file
* Input: the macro name, the file name, and the macro list
* Output: none. 
*/
void copyMacroToFile(MacroList* head, char* macroName, char* filename)
{
    while (strcmp(macroName, head->m.name))
    {
        head = head->next;
    }

    FILE* newMacroFile = open_file_create(filename);

    write_line(newMacroFile, head->m.data);

    fclose(newMacroFile);
}
