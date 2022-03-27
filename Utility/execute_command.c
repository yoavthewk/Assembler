#include "exec.h"

void parse_command(char *line, symbol_list *head, int action_index, int line_number, hregister *IC, hregister *DC, PSW *flag_register, command_list *command_head)
{
    int number = 0, number1 = 0, index = 0, index1 = 0;
    char *tok, line_backup[MAX_LEN] = {0}, tmp_tok[MAX_LEN] = {0};
    int command_length = 0;
    char **arr = NULL;
    int src, dst;

    /* firstly, we check if the command has any continuation */
    if (line[0] != ' ')
    {
        /* if it is a 0 operand command, we're okay, and we encode it */
        if (!action_table[action_index].operands)
        {
            command_length += 1;
            if (!flag_register->ENC) {
                arr = (char **) calloc(sizeof(char *) * MAX_WORD_SIZE, sizeof(char *));
                arr[0] = encode_command_opcode(action_index);
                insert_command_list(&command_head, init_command_node(NULL, command_length, IC->data, false, arr));
            }
            IC->data += command_length;
            return;
        }
        /* if not, we raise an error and return */
        throw_error("Not enough operands passed!", line_number);
        return;
    }
    else if (!action_table[action_index].operands)
    {
        /* alert extraneous text error */
        throw_error("Extraneous text!", line_number);
        return;
    }

    memmove(line, line + 1, strlen(line)); /* skip the space */
    strcpy(line_backup, line);
    command_length += 2;
    tok = strtok(line, ",");
    strcpy(tmp_tok, tok);

    /* find the addressing mode of the first operand */
    
    if ((dst = find_addressing(tok, &index, &command_length, &number, action_index, line_number, head, flag_register, false)) != -1)
        goto found;

    /* alert error */
    if (flag_register->ERR) return;
    throw_error("Invalid or missing first operand!\n", line_number);
    /* break */
    return;

found: /* it means the first operand is being addressed in a valid way, therefore we search the second */
    if (flag_register->ERR || flag_register->ENC) return;
    
    strcpy(line, line_backup);

    if (action_table[action_index].operands == 2)
        strtok(line, ",");

    tok = strtok(NULL, ","); /* get the rest of the line */
    if (!tok || tok[0] == '\n' || !tok[0])
    {
        if (action_table[action_index].operands == 1)
        {
            encode_one_operand(action_index, command_head, dst, number, index, IC->data, command_length);
            IC->data += command_length;
            return;
        }

        /* not enough operands */
        throw_error("Not enough operands passsed!", line_number);
        return;
    }
    else if (action_table[action_index].operands == 1)
    {
        throw_error("Extraneous text!", line_number);
        return;
    }

    /*
       we find the addressing mode of the second operand
    */
    if ((src = find_addressing(tok, &index1, &command_length, &number1, action_index, line_number, head, flag_register, true)) != -1)
        goto found2;

    /* alert error and break */
    if (flag_register->ERR) return;

    fflush(stdin);
    throw_error("Invalid or Missing Second Operand!", line_number);
    flag_register->ERR = 1;
    return;

found2:
    if (flag_register->ERR || flag_register->ENC) return;
    
    /*            encode                */
    /************************************/
    encode_two_operand(command_head, action_index, number, number1, index, index1, src, dst, IC->data, command_length);
    /**********************************************************/

    IC->data += command_length;
}

int find_addressing(char* tok, int* index, int* command_length, int* number, const int action_index, const int line_number, symbol_list* head, PSW* flag_register, bool is_second)
{
    char tmp_tok[MAX_LEN] = {0};
    int i = 0;
    /* find the addressing mode of the first operand */
    for (i = 0; i < NUM_OF_ADDRESSING; i++)
    {
        strcpy(tmp_tok, tok);
        if (is_second ? action_table[action_index].second_operand_valid[i] : (action_table[action_index].operands == 2 && action_table[action_index].first_operand_valid[i]) || (action_table[action_index].operands == 1 && action_table[action_index].second_operand_valid[i]))
        {
            switch (i)
            {
            case IMMEDIATE:
                if (is_immediate(tok, number, flag_register, line_number))
                {
                    (*command_length)++; /* add the word of the immediate */
                    return i;
                }
                break;
            case INDEX:
                if (is_index(tok, index, flag_register, line_number))
                {
                    (*command_length) += 2; /* add the base address and the offset */
                    return i;
                }
                break;
            case REGISTER_DIRECT:
                if (is_register_direct(tok, number, flag_register))
                {
                    return i;
                }
                break;
            case DIRECT:
                if (is_direct(tok, head))
                {
                    (*command_length) += 2; /* add base and offset */
                    return i;
                }
                break;
            }
        }
        strcpy(tok, tmp_tok);
    }
    return -1;
}

void encode_one_operand(int action_index, command_list* command_head, int dst, int number, int index, int IC, int command_length){
    int list_index = 0;
    char** arr = (char **)calloc(sizeof(char *) * MAX_WORD_SIZE, sizeof(char *));
    arr[list_index++] = encode_command_opcode(action_index);

    /* encode */
    if (dst == IMMEDIATE)
    {
        arr[list_index++] = encode_command_registers(0, 0, action_index, 0, dst, false);
        arr[list_index++] = encode_immediate(number);
    }
    else
    {
        int dst_register;
        dst_register = dst == REGISTER_DIRECT ? number : dst == DIRECT ? 0
                                                                    : index;

        arr[list_index++] = encode_command_registers(0, dst_register, action_index, 0, dst, false);
    }

    insert_command_list(&command_head, init_command_node(NULL, command_length, IC, false, arr));
}

void encode_two_operand(command_list* command_head, int action_index, int number, int number1, int index, int index1, int src, int dst, int IC, int command_length){
    int list_index = 0;
    char** arr = (char **)calloc(sizeof(char *) * MAX_WORD_SIZE, sizeof(char *));

    arr[list_index++] = encode_command_opcode(action_index);

    if (src == REGISTER_DIRECT && dst == REGISTER_DIRECT)
    {
        arr[list_index++] = encode_command_registers(number1, number, action_index, src, dst, true);
    }
    else
    {
        int dst_register, src_register;
        dst_register = dst == REGISTER_DIRECT ? number : dst == DIRECT ? 0
                                                                       : index;
        src_register = src == REGISTER_DIRECT ? number1 : src == DIRECT ? 0
                                                                        : index1;
        arr[list_index++] = encode_command_registers(src_register, dst_register, action_index, src, dst, true);
    }

    if (src == IMMEDIATE && dst == IMMEDIATE)
    {
        arr[list_index++] = encode_immediate(number);
        arr[list_index++] = encode_immediate(number1);
    }
    else if (src == IMMEDIATE)
    {
        arr[list_index + 2] = encode_immediate(number1); /* we leave 2 spots for the labels */
    }
    else if (dst == IMMEDIATE)
    {
        arr[list_index++] = encode_immediate(number);
    }

    insert_command_list(&command_head, init_command_node(NULL, command_length, IC, false, arr));
}

