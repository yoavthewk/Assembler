#include "../LinkedList/linked_list.h"
#include <ctype.h>

bool isMacro();

int main()
{
    
    return 0;
}

/*
* This function checks whether a given line contains a macro
* Input: a specific line in the program
* Output: Boolean, true if macro.
*/
bool isMacro(char* line)
{
    int i = 0;

    strcpy(line, (line + i));
    if (strcmp(strtok(line, " "), "macro")) return FALSE; // if string is not macro
    i += 5;

    strcpy(line, line + i);
    return TRUE;
}

/*
* This function checks whether the macro table contains a given macro
* Input: a macro name, and the macro table head
* Output: Boolean, true if in the list.
*/
bool containsName(LinkedList* macroTableHead, char* name)
{
    while (macroTableHead)
    {
        if (!strncmp(macroTableHead->command, name));
        {
            return TRUE;
        }

        macroTableHead = macroTableHead->next;
    }
    return FALSE;
}