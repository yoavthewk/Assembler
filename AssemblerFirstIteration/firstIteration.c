#include "firstIteration.h"
#include "../CommandParsing/exec.h"

void process_line(char *line, SymbolList *head, int line_number)
{
    int i;
    char *name;
    char line_backup[MAX_LEN];
    bool att[] = {false, false, false, false};
    flagRegister.SYM = 0;
    line = parse_line(line); /* getting the parsed command */
    strcpy(line_backup, line);

    /* returns the full line if there is no label, otherwise, it cuts the label definition off after handling it */
    contains_label(line_backup);
    strcpy(line_backup, line);

    if (handle_data(line_backup, head))
    {
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    if (is_entry(line_backup))
    {
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    if (is_extern(line_backup))
    {
        strcpy(line_backup, line);
        handle_extern(line_backup, head);
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    /* if it's not any one of those, it is a command, and so if it is a symbol we add it */
    /* check if the symbol exists or is illegal */
    name = strtok(line_backup, " ");
    if (flagRegister.SYM)
    {
        name[strlen(name) - 1] = 0;
        att[2] = true;
        insertSymbol(&head, initSymbolNode(NULL, name, OFFSET + BASE, BASE, OFFSET, att)); /* add the command to the list */
        name = strtok(NULL, " ");
    }

    /* Loop over each command, and check if it's name is equal to a known command */
    for (i = 0; strcmp(action_table[i].name, "invalid"); i++)
    {
        if (!strcmp(name, action_table[i].name))
            break;
    }
    
    strcpy(line, line + strlen(name) + 1); /* get the rest of the line after the command */

    /* if it isn't, we print an error */
    if (!action_table[i].op_code && !action_table[i].operands)
    {
        fprintf(stderr, "Command does not exist: %s\n", name);
        free(line);
        return;
    }

    /* if it is, we call the function that executes the command */
    parse_command(line, head, i, line_number);
    
    free(line);
}

void handle_extern(char *line, SymbolList *head)
{
    /* get the name of the symbol first */
    char *name;
    bool att[] = {false, false, false, false};
    name = strtok(line, " ");
    while (!strcmp(name, ".extern"))
    {
        name = strtok(NULL, " ");
    }
    name = strtok(NULL, " ");

    if (!name)
        return; /* throw an error - no name provided */
    /* check for errors and stuff */

    att[0] = true;
    insertSymbol(&head, initSymbolNode(NULL, name, 0, 0, 0, att)); /* TEMP!!! */
}

void contains_label(char *line)
{
    char *canBeLabel = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcspn(canBeLabel, ":") || strcspn(canBeLabel, ":") == strlen(canBeLabel))
    {
        flagRegister.SYM = 0;
    }
    else
    {
        /* if there is a definition of symbol */
        flagRegister.SYM = 1;
    }
}

bool handle_data(char *line, SymbolList *head)
{
    char lineBackup[MAX_LEN] = {0};
    char *canBeData = NULL; /* get the first (or only) word in the line. */
    bool att[] = {false, false, false, false};
    strcpy(lineBackup, line);
    canBeData = strtok(line, " ");
    if (flagRegister.SYM)
    {
        /* canBeData is the name of the symbol in this case. */
        canBeData[strlen(canBeData) - 1] = 0; /* removed the : */
        /* add to symbol table */
        att[3] = true;
        insertSymbol(&head, initSymbolNode(NULL, canBeData, BASE + OFFSET, BASE, OFFSET, att)); /* TEMP */
        
        canBeData = strtok(NULL, " "); /* get the next word (or the only word) */
    }
    strcpy(line, lineBackup);
    if (!strcmp(canBeData, ".data") || !strcmp(canBeData, ".string"))
    {
        /* add to data table */
        process_data(line);
        return true;
    }
    return false;
}

void process_data(char *line)
{
    char *data = strtok(line, " "); /* get the first (or only) word in the line. */
    size_t length = 0;
    if (!strcmp(data, ".data"))
    {
        while ((data = strtok(NULL, ",")))
        {
            length += sizeof(word);
        }
    }
    else
    {
        strtok(NULL, "\"");
        data = strtok(NULL, "\"");
        length += strlen(data);
    }
    DC.data += length;
}

bool is_extern(char *line)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flagRegister.SYM ? !strcmp(strtok(NULL, " "), ".extern") : !strcmp(cmd, ".extern"); /* if .extern can be with a symbol before */
    /* else: if(flagRegister.SYM) show error return false... else return !strcmp(cmd, ".extern"); */
}

bool is_entry(char *line)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flagRegister.SYM ? !strcmp(strtok(NULL, " "), ".entry") : !strcmp(cmd, ".entry");
}
