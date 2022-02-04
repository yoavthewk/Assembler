#include "line_parsing.h"

/*
* This function checks whether a given line contains a macro
* Input: a specific line in the program
* Output: Boolean, true if macro.
*/
bool isMacro(MacroList* head, char* input, FILE* fp, char* filename)
{
    char *line = (char*)malloc(strlen(input) + 1), *cmd;
    strcpy(line, input);
    
    cmd = strtok(line, " ");
    if (cmd && strcmp(cmd, "macro"))
    {
        cmd[strlen(cmd) - 1] = 0;
        if (!containsSpace(cmd) && containsName(head, cmd))
        {
            copyMacroToFile(head, cmd, filename);
            free(line);
            return true;
        }
        free(line);
        return false; 
    } else if(!cmd) return false;
    cmd = strtok(NULL, "\n");

    /* Add the macro to the table. */
    addMacroToTable(head, fp, cmd);
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
    char* content = (char*)calloc(MAX_LEN, sizeof(char));
    char* line;
    line = get_next_line(fp);
    line = parse_line(line);
    if(!line){
        free(content);
        return;
    }
    while (strcmp(line, "endm\n")){
        printf("%s", line);
        content = (char*)realloc(content, strlen(line) + strlen(content) + 1);
        strcat(content, line);
        free(line);
        line = get_next_line(fp);
        line = parse_line(line);
        if(!line) break;
    } 
    free(line);
    insertAtEnd(&head, initNode(NULL, name, content));
    free(content);
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

/*
* This function checks if a possible macro name has a space.
* Input: possible name.
* Output: Boolean, true if it does, false elsewise.
*/
bool containsSpace(char* name){
    int i = 0; 
    for(; i < strlen(name); i++){
        if(isspace(*(name + i)) && *(name + i) != '\n'){
            return true;
        }
    }
    return false;
}
