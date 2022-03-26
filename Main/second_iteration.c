#include "second_iteration.h"

void second_iteration(char *file_name, FILE *fp, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register)
{
    int line_number = 0;
    char *line = NULL;
    FILE *obfp = create_object_file(file_name);

    while ((line = get_next_line(fp)) != NULL)
    {
        second_line_process(obfp, line, ICF, line_number, head, command_head, flag_register);
        free(line);
    }

    fclose(obfp);
    fclose(fp);
}

void second_line_process(FILE *fp, char *line, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register)
{
    char *line_backup;
    char *tok;

    line = parse_line_first_iteration(line, flag_register); /* getting the parsed command */

    line[strcspn(line, "\n")] = 0;
    strcpy(line_backup, line);
    if (!line[0])
    {
        free(line);
        return;
    }

    contains_label(line_backup, head, line_number, flag_register);
    strcpy(line_backup, line);
    tok = strtok(line_backup, " ");
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

    if (flag_register->SYM)
    {
        memmove(line, line + strlen(tok) + 1, strlen(line)); /* skip the name of the symbol and a space */
    }
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

    updateEntry(head, label_name);
}

FILE *create_object_file(char *file_name)
{
    const int obLength = 3;
    FILE *objectFp = fopen(file_name, "w");
    file_name = (char *)realloc(file_name, strlen(file_name) + obLength);
    strncat(file_name, ".ob", obLength);
    return objectFp;
}
