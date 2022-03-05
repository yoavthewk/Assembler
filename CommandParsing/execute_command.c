#include "exec.h"

void parse_command(char *line, SymbolList *head, int action_index, int line_number) {
    int i, number, index, address;
    char *label;
    char* tok;

    /* firstly, we check if the command has any continuation */
    if (line[0] != ' ') {
        /* if it is a 0 operand command, we're okay, and we encode it */
        if (!action_table[action_index].operands) {
            /* encode and return. */
        }
        /* if not, we raise an error and return */
        return;
    } else if (!action_table[action_index].operands) {
        /* alert extraneous text error */
        return;
    }
    strcpy(line, line + 1); /* skip the space */
    
    tok = strtok(line, ",");
    /* else, it has operands, and we check whether it is valid */
    for (i = 0; i < NUM_OF_ADDRESSING; i++) {
        if (action_table[action_index].first_operand_valid[i]) {
            switch (i) {
                case IMMEDIATE:
                    if (isImmediate(tok, &number)) goto found;
                    break;
                case DIRECT:
                    if (isDirect(tok, &address, head)) goto found;
                    break;
                case INDEX:
                    if (isIndex(tok, label, &index)) goto found;
                    break;
                case REGISTER_DIRECT:
                    if (isRegisterDirect(tok, &number)) goto found;
                    break;
            }
        }
    }
    /* alert error */
    throw_error("Invalid or Missing First Operand!", line_number);
    /* raise error flag: */
    /* break */
    return;

    found: /* it means the first operand is being addressed in a valid way, therefore we search the second */
    tok = strtok(NULL, ","); /* get the rest */
    if(!tok || tok[0] == '\n' || !tok[0]){
        if(action_table[action_index].operands == 1){
            if(/*error flag is activated */ 1){
                return;
            }
            else{
                /* encode */
                return;
            }
        }
        /* not enough operands */
        throw_error("Not Enough Operands Passsed!", line_number);
        return;
    }

    /* we can just use strtok to get to the , and then to the rest of the word.
    /* encode the first operand with what we found */
    /* afterwards, do the same for the rest */
    for (i = 0; i < NUM_OF_ADDRESSING; i++) {
        if (action_table[action_index].first_operand_valid[i]) {
            switch (i) {
                case IMMEDIATE:
                    if (isImmediate(tok, &number)) goto found2;
                    break;
                case DIRECT:
                    if (isDirect(tok, &address, head)) goto found2;
                    break;
                case INDEX:
                    if (isIndex(tok, label, &index)) goto found2;
                    break;
                case REGISTER_DIRECT:
                    if (isRegisterDirect(tok, &number)) goto found2;
                    break;
            }
        }
    }

    /* alert error and break */
    throw_error("Invalid or Missing Second Operand!", line_number);
    found2:
        return;
}