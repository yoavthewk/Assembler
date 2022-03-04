#include "exec.h"

void parse_command(char *line, SymbolList *head, int action_index, int line_number) {
    int i, number, index, address;
    char *label;

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

    /* else, it has operands, and we check whether it is valid */
    for (i = 0; i < NUM_OF_ADDRESSING; i++) {
        if (action_table[action_index].first_operand_valid[i]) {
            switch (i) {
                case 0:
                    if (isImmediate(line, &number)) goto found;
                    break;
                case 1:
                    if (isDirect(line, &address)) goto found;
                    break;
                case 2:
                    if (isIndex(line, label, &index)) goto found;
                    break;
                case 3:
                    if (isRegisterDirect(line, &number)) goto found;
                    break;
            }
        }
    }
    /* alert error and break */

    found: /* it means the first operand is being addressed in a valid way, therefore we search the second */
    if(action_table[action_index].operands == 1){
        /* encode */
        return;
    }
    /* encode the first operand with what we found */
    /* afterwards, do the same for the rest */
    for (i = 0; i < NUM_OF_ADDRESSING; i++) {
        if (action_table[action_index].first_operand_valid[i]) {
            switch (i) {
                case 0:
                    if (isImmediate(line, &number)) goto found2;
                    break;
                case 1:
                    if (isDirect(line, &address)) goto found2;
                    break;
                case 2:
                    if (isIndex(line, label, &index)) goto found2;
                    break;
                case 3:
                    if (isRegisterDirect(line, &number)) goto found2;
                    break;
            }
        }
    }
    /* alert error and break */

    found2:

}