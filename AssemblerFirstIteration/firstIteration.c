#include "firstIteration.h"


void firstIteration(char *file_name, FILE *fp, SymbolList *head, hregister *IC, hregister *DC, PSW *flagRegister, command_list *command_head)
{
    int line_number = 0;
    char *line = NULL;

    while ((line = get_next_line(fp)) != NULL)
    {
        process_line(line, head, line_number++, IC, DC, flagRegister, command_head);
        free(line);
    }
    updateSymbolList(head, IC);
    update_command_list(command_head, IC);
    printSymbolList(head);
    fclose(fp);
}

/* 
    This method processes the line given and analyzes it 
*/
void process_line(char *line, SymbolList *head, int line_number, hregister *IC, hregister *DC, PSW *flagRegister, command_list *command_head)
{
    /* Initialization of variables */
    int i = 0;
    size_t offset = 0;
    char *name = NULL, *tok = NULL;
    char line_backup[MAX_LEN] = {0};
    bool att[] = {false, false, false, false};
    flagRegister->SYM = 0;
    flagRegister->ERR = 0;

    line = parse_line_first_iteration(line, flagRegister); /* getting the parsed command */
    line[strcspn(line, "\n")] = 0;
    strcpy(line_backup, line);
    if (!line[0])
    {
        free(line);
        return;
    }

    /* check whether there's a symbol declaration */
    contains_label(line_backup, head, line_number, flagRegister);

    strcpy(line_backup, line);
    
    /* check if we need to handle data and if so, process it */
    if (handle_data(line_backup, head, IC, DC, line_number, flagRegister, command_head))
    {
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    /* if it is an entry line, we'll handle it in the second iteration. */
    if (is_entry(line_backup, flagRegister))
    {
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    /* if it is an extern line, we handle it */
    if (is_extern(line_backup, flagRegister))
    {
        strcpy(line_backup, line);
        handle_extern(line_backup, head, line_number, flagRegister);
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    /* if it's not any one of those, it is a command, and so if it is a symbol we add it */
    /* check if there are illegal commas */
    check_illegal_commas(line_backup, flagRegister);

    if (flagRegister->ERR)
    {
        throw_error("Invalid comma", line_number);
        free(line);
        return;
    }
    strcpy(line_backup, line);
    name = strtok(line_backup, " ");

    /* If there's a symbol declaration we check whether the symbol is valid and if it is insert it */
    /* to our symbol list. */
    if (flagRegister->SYM)
    {
        name[strlen(name) - 1] = 0;
        if (!isValidLabel(name, head))
        {
            /* alert error */
            printf("Line %d: label name \"%s\" already in use!\n", line_number, name);
            flagRegister->ERR = 1;
            return;
        }
        att[CODE] = true;
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
        printf("Line %d: Command does not exist: %s\n", line_number, name);
        free(line);
        return;
    }

    strcpy(line_backup, line);

    /* trying to check for externous text; */
    if (externousText(line, action_table[i].operands, flagRegister, line_number))
        return;

    strcpy(line, line_backup);
    
    /* if it is, we call the function that validates and encodes the command */
    parse_command(line, head, i, line_number, IC, DC, flagRegister, command_head);
    free(line);
}

void check_illegal_commas(char* line, PSW* flagRegister){
    int i, comma_count = 0;

    for(i = 0; i < strlen(line); i++){
        comma_count += *(line + i) == ',' ? 1 : 0;
    }

    flagRegister->ERR = comma_count >= 2 ? 1 : flagRegister->ERR;
}

bool externousText(char *line, int operands, PSW *flagRegister, int line_number)
{
    char *tok;
    char* tmp = line;
    if (operands == 2)
    {
        tok = strtok(line, ",");
        tok = strtok(NULL, " ");
        tok = strtok(NULL, " ");
    }
    else
    {
        while (*line)
        {
            if (*line == ',')
            {
                throw_error("Extraneous text!", line_number);
                flagRegister->ERR = 1;
                return true;
            }
            line++;
        }
        line = tmp;

        memmove(line, line + 1, strlen(line));
        tok = strtok(line, " ");
        tok = strtok(NULL, " ");
    }
    /* trying to check for externous text; */
    if (tok)
    {
        throw_error("Extraneous text!", line_number);
        flagRegister->ERR = 1;
        return true;
    }
    return false;
}

bool is_valid_label_name(char *name)
{
    bool valid = true;
    int i;

    /* first we check that the first character is a letter */
    valid = isalpha(name[0]) ? valid : false;

    for (i = 1; i < strlen(name) - 1; i++)
    {
        if (!isalpha(name[i]) && !isdigit(name[i]))
            valid = false;
    }

    return valid;
}

bool isValidLabel(char *label, SymbolList *head)
{
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

bool isValidExtern(char *label, SymbolList *head)
{
    return !contains_not_extern(head, label) && is_valid_label_name(label);
}

void handle_extern(char *line, SymbolList *head, int line_number, PSW *flagRegister)
{
    /* get the name of the symbol first */
    char *name;
    bool att[] = {false, false, false, false};
    strtok(line, " ");
    name = strtok(NULL, " ");

    if (!name || !isValidExtern(name, head))
    {
        printf("%s", name);
        /* throw errors */
        printf("Line %d: Invalid name or already in use!\n", line_number);
        flagRegister->ERR = 1;
        return;
    }

    att[EXTERN] = true;
    insertSymbol(&head, initSymbolNode(NULL, name, 0, 0, 0, att));
}

void updateSymbolList(SymbolList *head, hregister *IC)
{
    SymbolList *temp = head;
    while (temp)
    {
        if (temp->s.attributes[DATA])
        {
            temp->s.value += IC->data;
            temp->s.offset = temp->s.value % 16;
            temp->s.baseAddress = temp->s.value - (temp->s.value % 16);
        }
        temp = temp->next;
    }
}

void update_command_list(command_list *head, hregister *IC){
    command_list *temp = head;
    while (temp)
    {
        if (temp->data)
        {
            temp->IC += IC->data;
        }
        temp = temp->next;
    }
}

void contains_label(char *line, SymbolList *head, int line_number, PSW *flagRegister)
{
    char *canBeLabel = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcspn(canBeLabel, ":") || strcspn(canBeLabel, ":") == strlen(canBeLabel))
    {
        flagRegister->SYM = 0;
    }
    else
    {
        if (!isValidLabel(canBeLabel, head))
        {
            /* raise an error */
            printf("Line %d: Label %s Already Exists!", line_number, canBeLabel);
            flagRegister->ERR = 1;
            flagRegister->SYM = 0;
            return;
        }
        /* if there is a definition of symbol */
        flagRegister->SYM = 1;
    }
}

bool handle_data(char *line, SymbolList *head, hregister *IC, hregister *DC, int line_number, PSW *flagRegister, command_list* command_head)
{
    char lineBackup[MAX_LEN] = {0}, name[MAX_LEN] = {0};
    char *canBeData = NULL; /* get the first (or only) word in the line. */
    bool att[] = {false, false, false, false};
    strcpy(lineBackup, line);
    canBeData = strtok(line, " ");
    if (flagRegister->SYM)
    {
        /* canBeData is the name of the symbol in this case. */
        canBeData[strlen(canBeData) - 1] = 0; /* removed the : */
        /* add to symbol table */
        att[DATA] = true;
        strcpy(name, canBeData);
        canBeData = strtok(NULL, " "); /* get the next word (or the only word) */
    }

    if (!strcmp(canBeData, ".data") || !strcmp(canBeData, ".string"))
    {
        strcpy(line, lineBackup);
        /* if it is a symbol, we add it to the symbol list */
        if (flagRegister->SYM)
        {
            insertSymbol(&head, initSymbolNode(NULL, name, DC->data, 0, 0, att));
        }
        /* add to data table */
        process_data(line, DC, line_number, flagRegister, command_head);
        return true;
    }
    return false;
}

void process_data(char *line, hregister *DC, int line_number, PSW *flagRegister, command_list* head)
{
    int num, i, list_index = 0, length = 0;
    char *binary_line;
    char *data = strtok(line, " "); /* get the first (or only) word in the line. */
    char** arr = (char**)calloc(sizeof(char*) * MAX_WORD_SIZE, sizeof(char*));
    data = flagRegister->SYM ? strtok(NULL, " ") : data;
    if (!strcmp(data, ".data"))
    {
        printf(".Data: \n");
        
        while ((data = strtok(NULL, ",")))
        {
            length++;
            if(contains_space(data, flagRegister)){
                throw_error("Extraneous text!", line_number);
                for (i = 0; i < list_index; i++)
                {
                    free(arr[i]);
                }
                
                free(arr);
                return;
            }
            num = getNumber(data, flagRegister);
            if (flagRegister->ERR)
            {
                /* alert error */
                throw_error("Invalid number entered!", line_number);
                for (i = 0; i < list_index; i++)
                {
                    free(arr[i]);
                }
                free(arr);
                return;
            }
            printf("num: %d\n", num);
            arr[list_index++] = encode_immediate(num);
            printf("%s\n", arr[list_index - 1]);
        }
        insert_command_list(head, init_command_node(NULL, length, DC->data, true, arr));
        DC->data += length;
    }
    else
    {
        data = strtok(NULL, " \"");
        /*printf("%s\n", data);
        data = strtok(NULL, "\"");*/
        printf("%s String: \n", data);
        for (i = 0; data[i] != 0; i++)
        {
            arr[list_index++] = encode_immediate(data[i]);
            printf("%s\n", arr[list_index - 1]);
        }
        arr[list_index++] = encode_immediate(0);
        printf("%s\n", arr[list_index - 1]);
        insert_command_list(head, init_command_node(NULL, strlen(data) + 1, DC->data, true, arr));

        DC->data += strlen(data) + 1;
    }
}

bool is_extern(char *line, PSW *flagRegister)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flagRegister->SYM ? !strcmp(strtok(NULL, " "), ".extern") : !strcmp(cmd, ".extern"); /* if .extern can be with a symbol before */
    /* else: if(flagRegister.SYM) show error return false... else return !strcmp(cmd, ".extern"); */
}

bool is_entry(char *line, PSW *flagRegister)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flagRegister->SYM ? !strcmp(strtok(NULL, " "), ".entry") : !strcmp(cmd, ".entry");
}

bool contains_space(char* data, PSW* flagRegister){
    int i = 0;
    for(; i < strlen(data); i++){
        if(*(data + i) == ' ') return true;
    }
    return false;
}