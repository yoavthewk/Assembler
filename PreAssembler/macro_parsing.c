#include "line_parsing.h"

bool inMacro = false;

/*
* This function checks whether a given line contains a macro
* Input: a specific line in the program
* Output: Boolean, true if macro.
*/
bool isMacro(MacroList* head, char* input, FILE* fp, char* filename)
{
    int i = 0;
    char *line = (char*)malloc(strlen(input) + 1), *cmd;
    strcpy(line, input);

    cmd = strtok(line, " ");
    if (cmd && strcmp(cmd, "macro"))
    {
        free(line);
        return false; /* if string is not macro */
    }
    else if(!cmd) 
    {  
        cmd = strtok(line, "\n");
        if (containsName(head, cmd))
        {
            copyMacroToFile(head, cmd, filename);
            free(line);
            return true;
        }
    }
    i += 5;
    strcpy(line, line + i);

    /* Add the macro to the table. */
    addMacroToTable(head, fp, cmd);
    inMacro = true;
    free(line);
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
    char* content = (char*)malloc(sizeof(char));
    char* line;

    line = get_next_line(fp);
    while (strcmp(line, "endm")){
        content = (char*)realloc(content, strlen(line) + strlen(content));
        strcat(content, line);
        line = get_next_line(fp);
    } 

    insertAtEnd(&head, initNode(NULL, name, content));
}

/*
* This function replaces a macro name with its content inside the assembly file
* Input: the macro name, the file name, and the macro list
* Output: none. 
*/
void copyMacroToFile(MacroList* head, char* macroName, char* filename)
{
    FILE* newMacroFile;

    while (strcmp(macroName, head->m.name))
    {
        head = head->next;
    }

    newMacroFile = open_file_create(filename);

    write_line(newMacroFile, head->m.data);

    fclose(newMacroFile);
}
