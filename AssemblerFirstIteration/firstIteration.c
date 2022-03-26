#include "firstIteration.h"


void firstIteration(char *file_name, FILE *fp, symbol_list *head, hregister *IC, hregister *DC, PSW *flag_register, command_list *command_head)
{
    int line_number = 0;
    char *line = NULL;

    while ((line = get_next_line(fp)) != NULL)
    {
        process_line(line, head, line_number++, IC, DC, flag_register, command_head);
        free(line);
    }
    update_symbol_list(head, IC);
    update_command_list(command_head, IC);
    print_command_list(command_head);
    print_symbol_list(head);
    fclose(fp);
}

/* 
    This method processes the line given and analyzes it 
*/
void process_line(char *line, symbol_list *head, int line_number, hregister *IC, hregister *DC, PSW *flag_register, command_list *command_head)
{
    /* Initialization of variables */
    int i = 0;
    size_t offset = 0;
    char *name = NULL;
    char line_backup[MAX_LEN] = {0};
    bool att[] = {false, false, false, false};
    flag_register->SYM = 0;
    flag_register->ERR = 0;

    line = parse_line_first_iteration(line, flag_register); /* getting the parsed command */
    line[strcspn(line, "\n")] = 0;
    strcpy(line_backup, line);
    if (!line[0])
    {
        free(line);
        return;
    }

    /* check whether there's a symbol declaration */
    contains_label(line_backup, head, line_number, flag_register);

    strcpy(line_backup, line);
    
    /* check if we need to handle data and if so, process it */
    if (handle_data(line_backup, head, IC, DC, line_number, flag_register, command_head))
    {
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    /* if it is an entry line, we'll handle it in the second iteration. */
    if (is_entry(line_backup, flag_register))
    {
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    /* if it is an extern line, we handle it */
    if (is_extern(line_backup, flag_register))
    {
        strcpy(line_backup, line);
        handle_extern(line_backup, head, line_number, flag_register);
        free(line);
        line = NULL;
        return;
    }
    strcpy(line_backup, line);

    /* if it's not any one of those, it is a command, and so if it is a symbol we add it */
    /* check if there are illegal commas */
    check_illegal_commas(line_backup, flag_register);

    if (flag_register->ERR)
    {
        throw_error("Invalid comma", line_number);
        free(line);
        return;
    }
    strcpy(line_backup, line);
    name = strtok(line_backup, " ");

    /* If there's a symbol declaration we check whether the symbol is valid and if it is insert it */
    /* to our symbol list. */
    if (flag_register->SYM)
    {
        name[strlen(name) - 1] = 0;
        if (!isValidLabel(name, head))
        {
            /* alert error */
            printf("Line %d: label name \"%s\" already in use!\n", line_number, name);
            flag_register->ERR = 1;
            return;
        }
        att[CODE] = true;
        insert_symbol(&head, init_symbol_node(NULL, name, IC->data, IC->data - (IC->data % 16), IC->data % 16, att)); /* add the command to the list */
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
    if (extraneous_text(line, action_table[i].operands, flag_register, line_number))
        return;

    strcpy(line, line_backup);
    
    /* if it is, we call the function that validates and encodes the command */
    parse_command(line, head, i, line_number, IC, DC, flag_register, command_head);
    free(line);
}

void check_illegal_commas(char* line, PSW* flag_register){
    int i, comma_count = 0;

    for(i = 0; i < strlen(line); i++){
        comma_count += *(line + i) == ',' ? 1 : 0;
    }

    flag_register->ERR = comma_count >= 2 ? 1 : flag_register->ERR;
}

bool extraneous_text(char *line, int operands, PSW *flag_register, int line_number)
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
                flag_register->ERR = 1;
                return true;
            }
            line++;
        }
        line = tmp;

        memmove(line, line + 1, strlen(line));
        tok = strtok(line, " ");
        tok = strtok(NULL, " ");
    }
    /* trying to check for extraneous text; */
    if (tok)
    {
        throw_error("Extraneous text!", line_number);
        flag_register->ERR = 1;
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

bool isValidLabel(char *label, symbol_list *head)
{
    return is_valid_label_name(label) && !contains(head, label);
}

bool contains_not_extern(symbol_list *head, char *name)
{
    do
    {
        if (!strncmp(head->s.name, name, SYMBOL_SIZE) && !head->s.attributes[EXTERN])
            return true;
    } while ((head = head->next));
    return false;
}

bool is_valid_extern(char *label, symbol_list *head)
{
    return !contains_not_extern(head, label) && is_valid_label_name(label);
}

void handle_extern(char *line, symbol_list *head, int line_number, PSW *flag_register)
{
    /* get the name of the symbol first */
    char *name;
    bool att[] = {false, false, false, false};
    strtok(line, " ");
    name = strtok(NULL, " ");

    if (!name || !is_valid_extern(name, head))
    {
        printf("%s", name);
        /* throw errors */
        printf("Line %d: Invalid name or already in use!\n", line_number);
        flag_register->ERR = 1;
        return;
    }

    att[EXTERN] = true;
    insert_symbol(&head, init_symbol_node(NULL, name, 0, 0, 0, att));
}

void update_symbol_list(symbol_list *head, hregister *IC)
{
    symbol_list *temp = head;
    while (temp)
    {
        if (temp->s.attributes[DATA])
        {
            temp->s.value += IC->data;
            temp->s.offset = temp->s.value % 16;
            temp->s.base_address = temp->s.value - (temp->s.value % 16);
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

void contains_label(char *line, symbol_list *head, int line_number, PSW *flag_register)
{
    char *canBeLabel = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcspn(canBeLabel, ":") || strcspn(canBeLabel, ":") == strlen(canBeLabel))
    {
        flag_register->SYM = 0;
    }
    else
    {
        if (!isValidLabel(canBeLabel, head))
        {
            /* raise an error */
            printf("Line %d: Label %s Already Exists!", line_number, canBeLabel);
            flag_register->ERR = 1;
            flag_register->SYM = 0;
            return;
        }
        /* if there is a definition of symbol */
        flag_register->SYM = 1;
    }
}

bool handle_data(char *line, symbol_list *head, hregister *IC, hregister *DC, int line_number, PSW *flag_register, command_list* command_head)
{
    char lineBackup[MAX_LEN] = {0}, name[MAX_LEN] = {0};
    char *canBeData = NULL; /* get the first (or only) word in the line. */
    bool att[] = {false, false, false, false};
    strcpy(lineBackup, line);
    canBeData = strtok(line, " ");
    if (flag_register->SYM)
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
        if (flag_register->SYM)
        {
            insert_symbol(&head, init_symbol_node(NULL, name, DC->data, 0, 0, att));
        }
        /* add to data table */
        process_data(line, DC, line_number, flag_register, command_head);
        return true;
    }
    return false;
}

void process_data(char *line, hregister *DC, int line_number, PSW *flag_register, command_list* head)
{
    int num, i, list_index = 0, length = 0;
    char *data = strtok(line, " "); /* get the first (or only) word in the line. */
    char** arr = (char**)calloc(sizeof(char*) * MAX_WORD_SIZE, sizeof(char*));
    data = flag_register->SYM ? strtok(NULL, " ") : data;
    if (!strcmp(data, ".data"))
    {
        printf(".Data: \n");
        
        while ((data = strtok(NULL, ",")))
        {
            length++;
            if (length > LINES) {
                arr = (char**)realloc(&arr, sizeof(char*) * length);
            }
            if(contains_space(data, flag_register)){
                throw_error("Extraneous text!", line_number);
                for (i = 0; i < list_index; i++)
                {
                    free(arr[i]);
                }
                
                free(arr);
                return;
            }
            num = getNumber(data, flag_register);
            if (flag_register->ERR)
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
        insert_command_list(&head, init_command_node(NULL, length, DC->data, true, arr));
        DC->data += length;
    }
    else
    {
        data = strtok(NULL, " \"");
        if (strlen(data) > LINES) {
            arr = (char**)realloc(&arr, sizeof(char*) * strlen(data));
        }
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
        insert_command_list(&head, init_command_node(NULL, strlen(data) + 1, DC->data, true, arr));

        DC->data += strlen(data) + 1;
    }
}

bool is_extern(char *line, PSW *flag_register)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flag_register->SYM ? !strcmp(strtok(NULL, " "), ".extern") : !strcmp(cmd, ".extern"); /* if .extern can be with a symbol before */
    /* else: if(flag_register.SYM) show error return false... else return !strcmp(cmd, ".extern"); */
}

bool is_entry(char *line, PSW *flag_register)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flag_register->SYM ? !strcmp(strtok(NULL, " "), ".entry") : !strcmp(cmd, ".entry");
}

bool contains_space(char* data, PSW* flag_register){
    int i = 0;
    for(; i < strlen(data); i++){
        if(*(data + i) == ' ') return true;
    }
    return false;
}