#include "../LinkedList/linked_list.h"
#include "../FileHandling/files.h"
#include <ctype.h>


bool inMacro = FALSE;

void copyMacroToFile(token);
bool isMacro();
void addMacroToTable(char *name);

int main(){
    
    return 0;
}

/*
* This function checks whether a given line contains a macro
* Input: a specific line in the program
* Output: Boolean, true if macro.
*/
bool isMacro(char* input){
    int i = 0;
    char* name;
    char *line;
    strcpy(line, input);

    name = strtok(line, "\n");
    if(containsName(token)){
        copyMacroToFile(token);
        return;
    }

    if (strcmp(strtok(line, " "), "macro")) return FALSE; // if string is not macro
    i += 5;

    strcpy(line, line + i);
    name = strtok(line, "\n");
    // Add the macro to the table.
    inMacro = TRUE;
    return TRUE;
}

/*
* This function checks whether the macro table contains a given macro
* Input: a macro name, and the macro table head
* Output: Boolean, true if in the list.
*/
bool containsName(LinkedList* macroTableHead, char* name){
    while (macroTableHead){
        if (!strncmp(macroTableHead->command, name)){
            return TRUE;
        }

        macroTableHead = macroTableHead->next;
    }
    return FALSE;
}

void addMacroToTable(char *name){

}

void copyMacroToFile(char *name){

}