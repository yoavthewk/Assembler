#include "first_iteration.h"

/**
 * @brief performs the first iteration over the file.
 *
 * @param file_name the file name.
 * @param fp the file pointer.
 * @param head the head of the symbol list.
 * @param IC the instruction count.
 * @param DC the data count.
 * @param flagRegister the flag register.
 * @param commad_head the head of the command list.
 */
void firstIteration(char *file_name, FILE *fp, symbol_list *head, hregister *IC, hregister *DC, PSW *flag_register, command_list *command_head)
{
    int line_number = 1;
    char *line = NULL;

    while ((line = get_next_line(fp)) != NULL)
    {
        if (flag_register->ERR)
            flag_register->ENC = 1;
        process_line(line, head, line_number++, IC, DC, flag_register, command_head);
        free(line);
    }
    if (!flag_register->ENC)
    {
        update_symbol_list(head, IC);
        update_command_list(command_head, IC);
    }
}

/**
 * @brief processes a command line, validates it, analyzes it and encodes it accordingly if it is valid by calling a helper function.
 *        If it isn't it throws an error.
 *
 * @param line the line to process
 * @param head the head of the symbol list.
 * @param line_number the number of the current line.
 * @param IC the instruction count.
 * @param DC the data count.
 * @param flag_register the flag register.
 * @param command_head the head of the command list.
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
    if (!line[0] || line[0] == ';')
    {
        free(line);
        return;
    }

    /* check whether there's a symbol declaration */
    contains_label(line_backup, head, line_number, flag_register);

    if (flag_register->ERR)
    {
        free(line);
        return;
    }

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
        if (!is_valid_label(name, head))
        {
            /* alert error */
            throw_error("Label name is invalid!", line_number);
            flag_register->ERR = 1;
            free(line);
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
    {
        free(line);
        return;
    }
    strcpy(line, line_backup);

    /* if it is, we call the function that validates and encodes the command */
    parse_command(line, head, i, line_number, IC, DC, flag_register, command_head);
    free(line);
}

/**
 * @brief checks for illegal commas error in the line.
 *
 * @param line the line to check.
 * @param flag_register the flag register.
 */
void check_illegal_commas(char *line, PSW *flag_register)
{
    int i, comma_count = 0;

    for (i = 0; i < strlen(line); i++)
    {
        comma_count += *(line + i) == ',' ? 1 : 0;
    }

    flag_register->ERR = comma_count >= 2 ? 1 : flag_register->ERR;
}

/**
 * @brief checks whether there's an extraneous text error in the line.
 *
 * @param line the line to check.
 * @param operands the number of operands of the command.
 * @param flag_register the flag register.
 * @param line_number the current line number
 * @return true if there is an extraneous text error.
 */
bool extraneous_text(char *line, int operands, PSW *flag_register, int line_number)
{
    char *tok;
    char *tmp = line;
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

/**
 * @brief helper function of is_valid_label, checks if the name of the label is valid.
 *
 * @param name name to check.
 */
bool is_valid_label_name(char *name)
{
    bool valid = true;
    int i, register_number;

    /* first we check that the first character is a letter */
    if (!isalpha(name[0]))
        return false;

    for (i = 1; i < strlen(name) - 1; i++)
    {
        if (!isalpha(name[i]) && !isdigit(name[i]))
            return false;
    }

    /* Loop over each command, and check if it's name is equal to a known command */
    for (i = 0; strcmp(action_table[i].name, "invalid"); i++)
    {
        if (!strcmp(name, action_table[i].name))
            return false;
    }

    /* check other assembly saved words */
    if (!strcmp(name, "extern") || !strcmp(name, "entry") || !strcmp(name, "string") || !strcmp(name, "data"))
        return false;

    /* check if it's a register name */
    if (name[0] == 'r')
    {
        int j = 0;
        char number[MAX_LEN] = {0};

        i = 1;

        while ((isdigit(name[i]) || name[i] == '-' || name[i] == '+') && name[i] != ',')
            number[j++] = name[i++];

        register_number = atoi(number);
        if (register_number >= 0 && register_number <= 15)
        {
            return false;
        }
    }

    return valid;
}

/**
 * @brief checks if a label is valid.
 *
 * @param label label to check.
 * @param head head of the symbol list.
 * @return true if the label given is valid.
 */
bool is_valid_label(char *label, symbol_list *head)
{
    return is_valid_label_name(label) && !contains(head, label);
}

/**
* @brief helper function of is_valid_extern, checks if the symbol is already
         in the symbol list without the extern attribute.
*
* @param head the head of the symbol list.
* @param name name to check.
*/
bool contains_not_extern(symbol_list *head, char *name)
{
    do
    {
        if (!strncmp(head->s.name, name, SYMBOL_SIZE) && !head->s.attributes[EXTERN])
            return true;
    } while ((head = head->next));
    return false;
}

/**
 * @brief helper function of handle_extern, checks if the extern is valid.
 *
 * @param label the extern name.
 * @param head the head of the symbol list.
 * @return true if it is valid.
 */
bool is_valid_extern(char *label, symbol_list *head)
{
    return !contains_not_extern(head, label) && is_valid_label_name(label);
}

/**
 * @brief handles a line with extern declaration by checking whether it's valid and if so
 *        adding it to the symbol list.
 *
 * @param line the line to handle.
 * @param head the head of the symbol list.
 * @param line_number the number of the current line.
 * @param flag_register the flag register.
 */
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
        throw_error("Invalid name!", line_number);
        flag_register->ERR = 1;
        return;
    }

    att[EXTERN] = true;
    insert_symbol(&head, init_symbol_node(NULL, name, 0, 0, 0, att));
}

/**
 * @brief updates the symbol list after the first iteration by re-adjusting the value, offset
 *        and base address of data lines (value --> value + ICF).
 *
 * @param head the head of the symbol list.
 * @param IC the instruction count.
 */
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

/**
 * @brief updates the command list after the first iteration by re-adjusting
 *        the IC of data lines (IC --> IC + ICF).
 *
 * @param head the head of the command list.
 * @param IC the final instruction count.
 */
void update_command_list(command_list *head, hregister *IC)
{
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

/**
 * @brief checks if the line contains label declaration.
 *
 * @param line line to check.
 * @param head head of the symbol list.
 * @param line_number the number of the current line.
 * @param flag_register the flag register.
 */
void contains_label(char *line, symbol_list *head, int line_number, PSW *flag_register)
{
    char *canBeLabel = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcspn(canBeLabel, ":") || strcspn(canBeLabel, ":") == strlen(canBeLabel))
    {
        flag_register->SYM = 0;
    }
    else
    {
        if (!is_valid_label(canBeLabel, head))
        {
            /* raise an error */
            throw_error("Label name is invalid!", line_number);
            flag_register->ERR = 1;
            flag_register->SYM = 0;
            return;
        }
        /* if there is a definition of symbol */
        flag_register->SYM = 1;
    }
}

/**
 * @brief handles a line with data initialization.
 *
 * @param line line to handle.
 * @param head head of the symbol list.
 * @param DC the instruction count.
 * @param DC the data count.
 * @param line_number the number of the current line.
 * @param flag_register the flag register.
 * @param command_head the head of the command list.
 * @return true if there's data initialization.
 */
bool handle_data(char *line, symbol_list *head, hregister *IC, hregister *DC, int line_number, PSW *flag_register, command_list *command_head)
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

/**
 * @brief processes the data from the line, validates it and encodes it.
 *
 * @param line line to process.
 * @param DC the data count.
 * @param line_number the number of the current line.
 * @param flag_register the flag register.
 * @param head the head of the command list.
 * @return true if there's a declaration of an entry.
 */
void process_data(char *line, hregister *DC, int line_number, PSW *flag_register, command_list *head)
{
    int num, i, list_index = 0, length = 0;
    char *data = strtok(line, " "); /* get the first (or only) word in the line. */
    char **arr = (char **)calloc(sizeof(char *) * MAX_WORD_SIZE, sizeof(char *));
    data = flag_register->SYM ? strtok(NULL, " ") : data;
    if (!strcmp(data, ".data"))
    {
        while ((data = strtok(NULL, ",")))
        {
            length++;
            if (length > LINES)
            {
                arr = (char **)realloc(&arr, sizeof(char *) * length);
            }
            if (contains_space(data, flag_register))
            {
                throw_error("Extraneous text!", line_number);
                flag_register->ENC = 1;
                for (i = 0; i < list_index; i++)
                {
                    free(arr[i]);
                }
                free(arr);
                return;
            }
            num = get_number(data, flag_register);
            if (flag_register->ERR || num > MAX_NUM || num < MIN_NUM)
            {
                /* alert error */
                flag_register->ENC = 1;
                throw_error("Invalid number entered!", line_number);
                for (i = 0; i < list_index; i++)
                {
                    free(arr[i]);
                }
                free(arr);
                return;
            }
            arr[list_index++] = encode_immediate(num);
        }
        insert_command_list(&head, init_command_node(NULL, length, DC->data, true, arr));
        DC->data += length;
    }
    else
    {
        data = strtok(NULL, " \"");
        if (strtok(NULL, " \""))
        {
            flag_register->ENC = 1;
            flag_register->ERR = 1;
            throw_error("Extraneous text after string!", line_number);
        }
        if (strlen(data) > LINES)
        {
            arr = (char **)realloc(arr, sizeof(char *) * (strlen(data) + 1));
        }
        for (i = 0; data[i] != 0; i++)
        {
            arr[list_index++] = encode_immediate(data[i]);
        }
        arr[list_index++] = encode_immediate(0);
        insert_command_list(&head, init_command_node(NULL, strlen(data) + 1, DC->data, true, arr));

        DC->data += strlen(data) + 1;
    }
}

/**
 * @brief checks if there's a declaration of an extern in the line.
 *
 * @param line line to check.
 * @param flag_register the flag register.
 * @return true if there's a declaration of an extern.
 */
bool is_extern(char *line, PSW *flag_register)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flag_register->SYM ? !strcmp(strtok(NULL, " "), ".extern") : !strcmp(cmd, ".extern"); /* if .extern can be with a symbol before */
    /* else: if(flag_register.SYM) show error return false... else return !strcmp(cmd, ".extern"); */
}

/**
 * @brief checks if theres a declaration of an entry in the line.
 *
 * @param line line to check.
 * @param flag_register the flag register.
 * @return true if there's a declaration of an entry.
 */
bool is_entry(char *line, PSW *flag_register)
{
    char *cmd;
    cmd = strtok(line, " ");
    return flag_register->SYM ? !strcmp(strtok(NULL, " "), ".entry") : !strcmp(cmd, ".entry");
}

/**
 * @brief checks if the data contains a space
 *
 * @param data the data to check.
 * @param flag_register the flag register.
 * @return true if it contains a space.
 */
bool contains_space(char *data, PSW *flag_register)
{
    int i = 0;
    for (; i < strlen(data); i++)
    {
        if (*(data + i) == ' ')
            return true;
    }
    return false;
}