#include "../LinkedList/linked_list.h"
#include "../FileHandling/files.h"
#include <ctype.h>
#include <stdbool.h>

bool inMacro = false;
struct macro {
    char * name;
    char * data;
} 
void copyMacroToFile(token);
bool isMacro();
void addMacroToTable(char *name);

int main()
{
    
    return 0;
}

/*
* This function checks whether a given line contains a macro
* Input: a specific line in the program
* Output: Boolean, true if macro.
*/
bool isMacro(char* input)
{
    int i = 0;
    char* name;
    char *line;
    strcpy(line, input);

    name = strtok(line, "\n");
    if(containsName(head---, name))
    {
        copyMacroToFile(name);
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
bool containsName(LinkedList* macroTableHead, char* name)
{
    while (macroTableHead){
        if (!strcmp(macroTableHead->command, name))
        {
            return true;
        }

        macroTableHead = macroTableHead->next;
    }
    return false;
}

void addMacroToTable(char *name)
{
	//todo
}

void copyMacroToFile(char *name)
{
	//todo
}
