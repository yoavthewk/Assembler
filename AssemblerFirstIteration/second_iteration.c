#include "second_iteration.h"

void second_iteration(char *line, int ICF, int line_number, SymbolList* head, PSW* flagRegister){
    char* line_backup;
    char** binary_encoding = (char**)malloc(ICF * sizeof(char*));

    line = parse_line_first_iteration(line, flagRegister); /* getting the parsed command */

    line[strcspn(line, "\n")] = 0;
    strcpy(line_backup, line);
    if (!line[0])
    {
        free(line);
        return;
    }

    contains_label(line_backup, head, line_number, flagRegister);
    strcpy(line_backup, line);

    if(is_data(line_backup, flagRegister)){
        return;
    }
    strcpy(line_backup, line);
    if(is_extern(line_backup, flagRegister)){
        return;
    }
    strcpy(line_backup, line);

    if(is_entry(line_backup, flagRegister)){
        handle_entry(line_backup, head, flagRegister, line_number);
        return;
    }
}

bool is_data(char *line, PSW* flagRegister){
    char* cmd;

    cmd = strtok(line, " ");
    if (flagRegister->SYM) cmd = strtok(NULL, " "); /* get the next word (or the only word) */

    return !strcmp(cmd, ".data") || !strcmp(cmd, ".string");
}

void handle_entry(char *line, SymbolList* head, PSW* flagRegister, int line_number){
    char* label_name; 

    /* skip the .entry */
    strtok(line, " ");

    /* get the symbol after .entry */
    label_name = strtok(NULL, " ");

    if(!contains(head, label_name)){
        throw_error("Entry label does not exist!", line_number); /* Symbol does not exist */
        flagRegister->ERR = 1;
        return;
    }

    updateEntry(head, label_name);
}