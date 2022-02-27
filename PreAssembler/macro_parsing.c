#include "pre_assembler.h"
/*
 * This function checks whether a given line contains a macro
 * Input: a specific line in the program
 * Output: Boolean, true if macro.
 */
bool isMacro(MacroList *head, char *input, FILE *fp, char *filename)
{
    char *line = (char *)malloc(strlen(input) + 1), *cmd;
    strcpy(line, input);

    cmd = strtok(line, " "); /* get the first (or only) word in the line. */
    if (cmd && strcmp(cmd, "macro"))
    {
        /* If the word is not "macro" */
        cmd[strlen(cmd) - 1] = 0;
        if (!containsSpace(cmd) && containsName(head, cmd))
        {                                         /* we check if it is a name of a macro */
            copyMacroToFile(head, cmd, filename); /* if it is, we copy it's contents to the pre-assembled file */
            free(line);
            return true;
        }
        free(line);
        return false;
    }
    else if (!cmd)
        return false;

    cmd = strtok(NULL, "\n");       /* get the name of the macro */
    addMacroToTable(head, fp, cmd); /* add the macro to the table. */
    free(line);
    return true;
}

/*
 * This function checks whether the macro table contains a given macro
 * Input: a macro name, and the macro table head
 * Output: Boolean, true if in the list.
 */
bool containsName(MacroList *macroTableHead, char *name)
{
    while (macroTableHead)
    {
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
void addMacroToTable(MacroList *head, FILE *fp, char *name)
{
    char *content = (char *)calloc(MAX_LEN, sizeof(char));
    char *line;
    char *originalLine;

    line = get_next_line(fp); /* get the content of the macro line by line */
    originalLine = (char *)malloc(strlen(line) + 1);
    strcpy(originalLine, line); /* copy the original line before parsing */
    line = parse_line(line);
    if (!line)
    {
        free(content);
        return;
    }
    while (strcmp(line, "endm\n"))
    {
        /* while the macro definition has not ended */
        content = (char *)realloc(content, strlen(originalLine) + strlen(content) + 1);
        strcat(content, originalLine); /* append the original line to the content */
        free(line);
        free(originalLine);
        line = get_next_line(fp); /* get the next line of content */
        originalLine = (char *)malloc(strlen(line) + 1);
        strcpy(originalLine, line); /* copy the original line before parsing */
        line = parse_line(line);
        if (!line)
            break;
    }
    free(line);
    free(originalLine);
    insertAtEnd(&head, initNode(NULL, name, content)); /* insert the macro to the macro list */
    free(content);
}

/*
 * This function replaces a macro name with its content inside the assembly file
 * Input: the macro name, the file name, and the macro list
 * Output: none.
 */
void copyMacroToFile(MacroList *head, char *macroName, char *filename)
{
    FILE *newMacroFile;

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
bool containsSpace(char *name)
{
    int i = 0;
    for (; i < strlen(name); i++)
    {
        if (isspace(*(name + i)) && *(name + i) != '\n')
        {
            return true;
        }
    }
    return false;
}

void preassemble(char *file_name, FILE *fp, MacroList *head)
{
    char *line;
    char *original_line;
    FILE *preassembled_file;

    if (clear_file(file_name))
    { /* clear the pre assembler file */
        while ((line = get_next_line(fp)) != NULL)
        {
            original_line = (char *)malloc(strlen(line) + 1);
            strcpy(original_line, line);
            line = parse_line(line);
            if (!isMacro(head, line, fp, file_name))
            {
                preassembled_file = open_file_create(file_name);
                write_line(preassembled_file, original_line);
                fclose(preassembled_file);
            }
            free(line);
            free(original_line);
        }
        fclose(fp);
    }
}

