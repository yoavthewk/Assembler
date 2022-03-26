#include "second_iteration.h"

void second_iteration(char *file_name, FILE *fp, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register)
{
    char *line = NULL;
    FILE *obfp = create_object_file(file_name);
    FILE *entfp = create_entry_file(file_name);
    FILE *extfp = create_extern_file(file_name);

    while ((line = get_next_line(fp)) != NULL)
    {
        second_line_process(obfp, line, ICF, line_number, head, command_head, flag_register);
        free(line);
    }

    write_entry_to_file(entfp, head);
    write_extern_to_file(extfp, head);
    fclose(obfp);
    fclose(extfp);
    fclose(entfp);
    fclose(fp);
}

void second_line_process(FILE *objfp, char *line, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register)
{
    char *line_backup = {0};
    char *tok, offset;
    char* label = NULL;
    int IC = 0;

    line = parse_line_first_iteration(line, flag_register); /* getting the parsed command */

    line[strcspn(line, "\n")] = 0;
    strcpy(line_backup, line);
    if (!line[0] || line[0] == ';')
    {
        free(line);
        return;
    }

    contains_label(line_backup, head, line_number, flag_register);
    strcpy(line_backup, line);
    tok = strtok(line_backup, " ");
    offset += strlen(tok) + 1;
    strcpy(line_backup, line);

    if (is_data(line_backup, flag_register))
    {
        return;
    }
    strcpy(line_backup, line);
    if (is_extern(line_backup, flag_register))
    {
        return;
    }
    strcpy(line_backup, line);

    if (is_entry(line_backup, flag_register))
    {
        handle_entry(line_backup, head, flag_register, line_number);
        return;
    }

    /* if we got this far, it is a command */    

    /* check if we need to complete words */
    if(!need_completion(command_head, IC)){
        /* if not, we return */
        return;
    }

    /* else, we find the label that needs to be completed */

    /* skip the command name */
    tok = strtok(line, " ");
    offset += strlen(tok);

    memmove(line, line + offset, strlen(line)); /* get the rest of the line after the command */

    /* get the first operand */
    tok = strtok(line, ",");

    /* if it starts with # it's an immediate, and we need to encode the second operand. */
    if(tok[0] == '#')
        label = strtok(NULL, ",");
    
    /* check if it's a register direct */
    else if(tok[0] == 'r'){
        int j = 0, i = 1, register_number = 0;
	    char number[MAX_LEN] = {0};

	    while ((isdigit(tok[i]) || tok[i] == '-' || tok[i] == '+') && tok[i] != ',')
		    number[j++] = tok[i++];

        register_number = atoi(number);
        if(register_number >= 0 && register_number <= 15){
            label = strtok(NULL, ",");
        }
    }
    else{
        label = tok;
    }

    fill_command_list(head, command_head, flag_register, label, IC);
    if(flag_register->ERR){
        throw_error("Label does not exist!", line_number);
    }
}

bool need_completion(command_list *head, int IC)
{
    do
    {
        if (head->IC == IC && head->L > 3){
            return true;
        }
    } while ((head = head->next));
    return false;
}

void fill_command_list(symbol_list *head, command_list *command_head, PSW* flag_register, char* label, int IC)
{
    int i;
    bool found = false;

    do
    {
        if (command_head->IC == IC){
            break;
        }
    } while ((command_head = command_head->next));

    do
    {
        if (!strcmp(label, head->s.name)){
            found = true;
            break;
        }
    } while ((head = head->next));

    if(!found){
        flag_register->ERR = 1;
        return;
    }

    for(i = 0; command_head->arr[i][0] != '?'; i++)
        ;

    command_head->arr[i++] = encode_label_value(head->s.value);
    command_head->arr[i] = encode_label_offset(head->s.offset);
}

bool is_data(char *line, PSW *flag_register)
{
    char *cmd;

    cmd = strtok(line, " ");
    if (flag_register->SYM)
        cmd = strtok(NULL, " "); /* get the next word (or the only word) */

    return !strcmp(cmd, ".data") || !strcmp(cmd, ".string");
}

void handle_entry(char *line, symbol_list *head, PSW *flag_register, int line_number)
{
    char *label_name;

    /* skip the .entry */
    strtok(line, " ");

    /* get the symbol after .entry */
    label_name = strtok(NULL, " ");

    if (!contains(head, label_name))
    {
        throw_error("Entry label does not exist!", line_number); /* Symbol does not exist */
        flag_register->ERR = 1;
        return;
    }
    
    update_entry(head, label_name);
}

void write_entry_to_file(FILE* fp, symbol_list *head)
{
    do
    {
        if (head->s.attributes[ENTRY]){
            fprintf(fp, "%s,%04d,%04d", head->s.name, head->s.base_address, head->s.offset);
        }
    } while ((head = head->next));
}

void write_extern_to_file(FILE* fp, symbol_list *head)
{
    do
    {
        if (head->s.attributes[EXTERN]){
            fprintf(fp, "%s BASE %04d", head->s.name, head->s.base_address);
            fprintf(fp, "%s OFFSET %04d\n", head->s.name, head->s.offset);
        }
    } while ((head = head->next));
}



