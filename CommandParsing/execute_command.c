#include "exec.h"

void parse_command(char *line, SymbolList *head, int action_index, int line_number, hregister *IC, hregister *DC, PSW *flagRegister)
{
    int i, number, index, address;
    char *label = NULL;
    char *tok, line_backup[MAX_LEN] = {0};
    int command_length = 0;
    
    /* firstly, we check if the command has any continuation */
    if (line[0] != ' ')
    {
        /* if it is a 0 operand command, we're okay, and we encode it */
        if (!action_table[action_index].operands)
        {
            command_length += 1;
            IC->data++;
            /* encode and return. */
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
    label = (char *)calloc(MAX_LEN, sizeof(char));
    tok = strtok(line, ",");

    for (i = 0; i < NUM_OF_ADDRESSING; i++)
    {
        strcpy(line, tok);
        if ((action_table[action_index].operands == 2 && action_table[action_index].first_operand_valid[i])
            || (action_table[action_index].operands == 1 && action_table[action_index].second_operand_valid[i]))
        {
            switch (i)
            {
            case IMMEDIATE:
                if (isImmediate(tok, &number, flagRegister))
                {
                    command_length++; /* add the word of the immediate */
                    goto found;
                }
                break;
            case INDEX:
                if (isIndex(tok, label, &index, flagRegister, line_number))
                {
                    command_length += 2; /* add the base address and the offset */
                    goto found;
                }
                break;
            case REGISTER_DIRECT:
                if (isRegisterDirect(tok, &number, flagRegister))
                {
                    goto found;
                }
                break;
            case DIRECT:
                if (isDirect(tok, &address, head))
                {
                    command_length += 2; /* add base and offset */
                    goto found;
                }
                break;
            }
        }
        strcpy(tok, line);
    }

    /* alert error */
    throw_error("Invalid or missing first operand!\n", line_number);
    /* raise error flag: */
    /* break */
    free(label);
    return;
    
found: /* it means the first operand is being addressed in a valid way, therefore we search the second */
    if (flagRegister->ERR)
    {
        free(label);
        return;
    }

    strcpy(line, line_backup);
    if(action_table[action_index].operands == 2) strtok(line, ",");
    tok = strtok(NULL, ","); /* get the rest */
    if (!tok || tok[0] == '\n' || !tok[0])
    {
        if (action_table[action_index].operands == 1)
        {
            /* encode */
            IC->data += command_length;
            free(label);
            return;
        }
        /* not enough operands */
        throw_error("Not enough operands passsed!", line_number);
        free(label);
        return;
    } else if(action_table[action_index].operands == 1){
        throw_error("Extraneous text!", line_number);
        free(label);
        return;
    }

    strcpy(line_backup, tok);

    /* we can just use strtok to get to the , and then to the rest of the word.
    /* encode the first operand with what we found */
    /* afterwards, do the same for the rest */
    for (i = 0; i < NUM_OF_ADDRESSING; i++)
    {
        if (action_table[action_index].second_operand_valid[i])
        {
            switch (i)
            {
            case IMMEDIATE:
                if (isImmediate(tok, &number, flagRegister))
                {
                    command_length += 1; /* add the word of the immediate */
                    goto found2;
                }
                break;
            case INDEX:
                if (isIndex(tok, label, &index, flagRegister, line_number))
                {
                    command_length += 2; /* add the base address and the offset */
                    goto found2;
                }
                break;
            case REGISTER_DIRECT:
                if (isRegisterDirect(tok, &number, flagRegister))
                {
                    goto found2;
                }
                break;
            case DIRECT:
                if (isDirect(tok, &address, head))
                {
                    command_length += 2; /* add base and offset */
                    goto found2;
                }
                break;
            }
        }
        strcpy(tok, line_backup);
    }

    /* alert error and break */
    fflush(stdin);
    throw_error("Invalid or Missing Second Operand!", line_number);
found2:
    if(flagRegister->ERR){
        free(label);
        return;
    }
    IC->data += flagRegister->ERR ? 0 : command_length;
    free(label);
}