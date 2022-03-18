#include "firstIteration.h"
#include "../CommandParsing/exec.h"

void firstIteration(char *file_name, FILE *fp, SymbolList *head, hregister* IC, hregister* DC)
{
    int line_number = 0;
    char *line;

    while ((line = get_next_line(fp)) != NULL)
    {
        process_line(line, head, line_number++, IC, DC);
        free(line);
    }
    printSymbolList(head);
    fclose(fp);
}

void process_line(char *line, SymbolList *head, int line_number, hregister* IC, hregister* DC)
{
    /* Initialization of variables */
    int i;
    size_t offset = 0;
    char *name;
    char line_backup[MAX_LEN] = {0};
    bool att[] = {false, false, false, false};
    flagRegister.SYM = 0;
    flagRegister.ERR = 0;

    line = parse_line_first_iteration(line); /* getting the parsed command */
    line[strcspn(line, "\n")] = 0;
    strcpy(line_backup, line);
    if (!line[0])
    {
        free(line);
        return;
    }
    /* returns the full line if there is no label, otherwise, it cuts the label definition off after handling it */
    contains_label(line_backup, head);
    strcpy(line_backup, line);

    if (handle_data(line_backup, head, IC, DC))
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
        if(!isValidLabel(name, head)){
            /* alert error */
            printf("Line %d: label name \"%s\" already in use!\n", line_number, name);
            flagRegister.ERR = 1;
            return;
        }
        att[2] = true;
        insertSymbol(&head, initSymbolNode(NULL, name, IC->data, IC->data - (IC->data % 16), IC->data % 16, att)); /* add the command to the list */
        offset += 2 + strlen(name);
        name = strtok(NULL, " ");
    }

    /* Loop over each command, and check if it's name is equal to a known command */
    for (i = 0; strcmp(action_table[i].name, "invalid"); i++)
    {
        if (!strcmp(name, action_table[i].name))
            break;
    }
    offset += strlen(name);
    memmove(line, line + offset, strlen(line)); /* get the rest of the line after the command */

    /* if it isn't, we print an error */
    if (!action_table[i].op_code && !action_table[i].operands)
    {
        fprintf(stderr, "Command does not exist: %s\n", name);
        free(line);
        return;
    }

    /* if it is, we call the function that executes the command */
    parse_command(line, head, i, line_number, IC, DC);
    free(line);
}

bool is_valid_label_name(char* name){
    bool valid = true;
    int i;

    /* first we check that the first character is a letter */
    valid = isalpha(name[0]) ? valid : false; 

    for(i = 1; i < strlen(name) - 1; i++){
        if(!isalpha(name[i]) && !isdigit(name[i])) valid = false;
    }
    
    return valid;
}

bool isValidLabel(char* label, SymbolList* head){
    return is_valid_label_name(label) && !contains(head, label);
}

bool contains_not_extern(SymbolList *head, char *name)
{
    do
    {
        if (!strncmp(head->s.name, name, SYMBOL_SIZE) && !head->s.attributes[EXTERN])
            return true;
    } while ((head = head->next));
    return false;
}

bool isValidExtern(char* label, SymbolList* head){
    return !contains_not_extern(head, label) && is_valid_label_name(label);
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

    if (!name || !isValidExtern(name, head)){
        /* throw errors */
        flagRegister.ERR = 1;
        return;
    }
    
    att[0] = true;
    insertSymbol(&head, initSymbolNode(NULL, name, 0, 0, 0, att));
}


void contains_label(char *line, SymbolList* head)
{
    char *canBeLabel = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcspn(canBeLabel, ":") || strcspn(canBeLabel, ":") == strlen(canBeLabel))
    {
        flagRegister.SYM = 0;
    }
    else
    {
        if(!isValidLabel(canBeLabel, head)){
            /* raise an error */
            printf("Label %s Already Exists!", canBeLabel);
            flagRegister.ERR = 1;
            flagRegister.SYM = 0;
            return;
        }
        /* if there is a definition of symbol */
        flagRegister.SYM = 1;
    }
}

bool handle_data(char *line, SymbolList *head, hregister* IC, hregister* DC)
{
    char lineBackup[MAX_LEN] = {0}, name[MAX_LEN] = {0};
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
        strcpy(name, canBeData);
        canBeData = strtok(NULL, " "); /* get the next word (or the only word) */
    }

    if (!strcmp(canBeData, ".data") || !strcmp(canBeData, ".string"))
    {
        strcpy(line, lineBackup);
        if (flagRegister.SYM){
            insertSymbol(&head, initSymbolNode(NULL, name, IC->data, IC->data - (IC->data % 16), IC->data % 16, att));
        }
        /* add to data table */
        process_data(line, DC);
        return true;
    }
    return false;
}

void process_data(char *line, hregister* DC)
{
    int num, i;
    char* binary_line;
    char *data = strtok(line, " "); /* get the first (or only) word in the line. */
    data = flagRegister.SYM ? strtok(NULL, " ") : data;
    if (!strcmp(data, ".data"))
    {
        printf(".Data: \n");
        while ((data = strtok(NULL, ",")))
        {
            num = getNumber(data, &flagRegister);
            if(flagRegister.ERR){
                /* alert error */
                printf("Invalid Number!\n");
                free(line);
                return;
            }
            printf("num: %d\n", num);
            binary_line = encode_immediate(num);
            printf("%s\n", binary_line);
            free(binary_line);
            DC->data++;
        }
    }
    else
    {
        data = strtok(NULL, " \"");
        /*printf("%s\n", data);
        data = strtok(NULL, "\"");*/
        printf("%s String: \n", data);
        for(i = 0; data[i] != 0; i++){
            binary_line = encode_immediate(data[i]);
            printf("%s\n", binary_line);
            free(binary_line);
        }
        binary_line = encode_immediate(0);
        printf("%s\n", binary_line);
        free(binary_line);
        DC->data += strlen(data) + 1;
    }
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
