#include "exec.h"

void parse_command(char *line, SymbolList *head, int action_index, int line_number, hregister* IC, hregister* DC)
{
    int i, number, index, address;
    char *label = NULL;
    char *tok, line_backup[MAX_LEN] = {0};

    /* firstly, we check if the command has any continuation */
    if (line[0] != ' ')
    {
        /* if it is a 0 operand command, we're okay, and we encode it */
        if (!action_table[action_index].operands)
        {
            IC->data++;
            /* encode and return. */
        }
        /* if not, we raise an error and return */
        return;
    }
    else if (!action_table[action_index].operands)
    {
        /* alert extraneous text error */
        return;
    }
    memmove(line, line + 1, strlen(line)); /* skip the space */
    strcpy(line_backup, line);

    tok = strtok(line, ",");
    label = (char *)calloc(MAX_LEN, sizeof(char));
    /* else, it has operands, and we check whether it is valid */
    if (action_table[action_index].operands == 2)
    {
        for (i = 0; i < NUM_OF_ADDRESSING; i++)
        {
            strtok(line, ",");
            if (action_table[action_index].first_operand_valid[i])
            {
                switch (i)
                {
                case IMMEDIATE:
                    if (isImmediate(tok, &number))
                    {
                        IC->data += 2; /* add the word of the immediate */
                        goto found;
                    }
                    break;
                case INDEX:
                    if (isIndex(tok, label, &index))
                    {
                        IC->data += 3; /* add the base address and the offset */
                        goto found;
                    }
                    break;
                case REGISTER_DIRECT:
                    if (isRegisterDirect(tok, &number))
                        IC->data++;
                        goto found;
                    break;
                case DIRECT:
                    if (isDirect(tok, &address, head))
                    {
                        IC->data += 3; /* add base and offset */
                        goto found;
                    }
                    break;
                }
            }
            strcpy(line, line_backup);
        }
        /* alert error */
        throw_error("Invalid or Missing First Operand!", line_number);
        /* raise error flag: */
        /* break */
        free(label);
        return;

    found: /* it means the first operand is being addressed in a valid way, therefore we search the second */
        strcpy(line, line_backup);
        strtok(line, ",");
        tok = strtok(NULL, ","); /* get the rest */
        if (!tok || tok[0] == '\n' || !tok[0])
        {
            if (action_table[action_index].operands == 1)
            {
                if (/*error flag is activated */ 1)
                {
                    free(label);
                    return;
                }
                else
                {
                    /* encode */
                    free(label);
                    return;
                }
            }
            /* not enough operands */
            throw_error("Not Enough Operands Passsed!", line_number);
            free(label);
            return;
        }
    }
    strcpy(line_backup, tok);
    /* we can just use strtok to get to the , and then to the rest of the word.
    /* encode the first operand with what we found */
    /* afterwards, do the same for the rest */
    for (i = 0; i < NUM_OF_ADDRESSING; i++)
    {
        strtok(line, ",");
        if (action_table[action_index].second_operand_valid[i])
        {
            switch (i)
            {
                case IMMEDIATE:
                    if (isImmediate(tok, &number))
                    {
                        IC->data += 1; /* add the word of the immediate */
                        goto found2;
                    }
                    break;
                case INDEX:
                    if (isIndex(tok, label, &index))
                    {
                        IC->data += 2; /* add the base address and the offset */
                        goto found2;
                    }
                    break;
                case REGISTER_DIRECT:
                    if (isRegisterDirect(tok, &number)){
                        goto found2;
                    }
                    break;
                case DIRECT:
                    if (isDirect(tok, &address, head))
                    {
                        IC->data += 2; /* add base and offset */
                        goto found2;
                    }
                    break;
            }
        }
        strcpy(line_backup, line);
    }

    /* alert error and break */
    fflush(stdin);
    throw_error("Invalid or Missing Second Operand!", line_number);
found2:
    free(label);
}