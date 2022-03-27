#include "second_iteration.h"
#include <stdio.h>

/**
 * @brief the second iteration over the file.
 * 
 * @param file_name the file to iterate over.
 * @param fp the file pointer.
 * @param ICF the final IC.
 * @param DCF the final DC.
 * @param head the head of the symbol list.
 * @param command_head the head of the command list.
 * @param flag_register the flag register.
 */
void second_iteration(char *file_name, FILE *fp, int ICF, int DCF, symbol_list *head, command_list *command_head, PSW *flag_register)
{
    char *line = NULL;
    int line_number = 1, IC = 100;
    FILE *obfp = create_object_file(file_name);
    FILE *entfp = create_entry_file(file_name);
    FILE *extfp = create_extern_file(file_name);

    while ((line = get_next_line(fp)) != NULL)
    {
        if (flag_register->ERR)
            flag_register->ENC = 1;
        second_line_process(extfp, line, ICF, line_number++, head, command_head, flag_register, &IC);
        free(line);
    }
    if (!flag_register->ENC)
    {
        print_command_list(command_head);
        format_object_file(obfp, ICF, DCF, command_head);
        write_entry_to_file(entfp, head);
    }
    fclose(obfp);
    fclose(extfp);
    fclose(entfp);
}

/**
 * @brief processes each line in the second iteration, it is the main logic function.
 * 
 * @param fp the file pointer of the extern file.
 * @param line the line to process.
 * @param ICF the final IC.
 * @param line_number the current line number.
 * @param head the head of the symbol list.
 * @param command_head the head of the command list.
 * @param flag_register the flag register.
 * @param IC the current IC.
 */
void second_line_process(FILE *fp, char *line, int ICF, int line_number, symbol_list *head, command_list *command_head, PSW *flag_register, int* IC)
{
    char line_backup[MAX_LEN] = {0};
    char *tok;
    char *label = NULL;
    int offset = 0;
    flag_register->ERR = flag_register->SYM = 0;

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
    if (flag_register->SYM)
    {
        tok = strtok(line_backup, " ");
        offset += strlen(tok) + 1;
    }
    else
    {
        offset += 2;
    }
    strcpy(line_backup, line);

    if (is_data(line_backup, flag_register))
    {
        free(line);
        return;
    }
    strcpy(line_backup, line);
    if (is_extern(line_backup, flag_register))
    {
        free(line);
        return;
    }
    strcpy(line_backup, line);

    if (is_entry(line_backup, flag_register))
    {
        strcpy(line_backup, line);
        handle_entry(line_backup, head, flag_register, line_number);
        free(line);
        return;
    }
    strcpy(line_backup, line);
    /* if we got this far, it is a command */
    /* check if we need to complete words */
    if (!need_completion(command_head, *IC))
    {
        *IC = get_next_IC(*IC, command_head);
        /* if not, we return */
        free(line);
        return;
    }

    /* else, we find the label that needs to be completed */

    /* skip the command name */
    tok = strtok(line, " ");
    offset += strlen(tok);
    strcpy(line, line_backup);
    memmove(line, line + offset - 1, strlen(line) + 1); /* get the rest of the line after the command */
    /* get the first operand */
    tok = strtok(line, ",");
    /* if it starts with # it's an immediate, and we need to encode the second operand. */
    label = (char *)calloc(MAX_LEN, sizeof(char));
    if (tok[0] == '#')
        strcpy(label, strtok(NULL, ","));

    /* check if it's a register direct */
    else if (tok[0] == 'r')
    {
        int j = 0, i = 1, register_number = 0;
        char number[MAX_LEN] = {0};

        while ((isdigit(tok[i]) || tok[i] == '-' || tok[i] == '+') && tok[i] != ',')
            number[j++] = tok[i++];

        register_number = atoi(number);
        if (register_number >= 0 && register_number <= 15)
        {
            strcpy(label, strtok(NULL, ","));
        }
    }
    else
    {
        strcpy(label, tok);
    }

    fill_command_list(head, &command_head, flag_register, fp, label, *IC);
    if (flag_register->ERR)
    {
        throw_error("Label does not exist!", line_number);
    }
    *IC = get_next_IC(*IC, command_head);
    free(label);
    free(line);
}

/**
 * @brief returns the next IC.
 * 
 * @param IC the current IC
 * @param head the head of the command list.
 * @return int the next IC.
 */
int get_next_IC(int IC, command_list *head)
{
    while (head)
    {
        if (IC < head->IC)
            return head->IC;
        head = head->next;
    }
    return 0;
}

/**
 * @brief checks if a command needs completion to it's encoding.
 * 
 * @param head the head of the command list.
 * @param IC the current IC.
 * @return true if it needs completion.
 */
bool need_completion(command_list *head, int IC)
{
    do
    {
        if (head->IC == IC && head->L > 3)
        {
            return true;
        }
    } while ((head = head->next));
    return false;
}

/**
 * @brief completes the encoding of a certain command in the command list.
 * 
 * @param head the head of the symbol list.
 * @param command_head the head of the command list.
 * @param flag_register the flag register.
 * @param fp the extern file file pointer.
 * @param label the label to fill.
 * @param IC the current IC.
 */
void fill_command_list(symbol_list *head, command_list **command_head, PSW *flag_register, FILE *fp, char *label, int IC)
{
    int i;
    char *is_index = strchr(label, '[');
    bool found = false;
    command_list *tmp = *command_head;
    char *substr = NULL;

    if (is_index)
    {
        substr = get_new_substring_with_indexes(label, 0, strcspn(label, "["));
        strcpy(label, substr);
        free(substr);
    }

    do
    {
        if (tmp->IC == IC)
        {
            break;
        }
    } while ((tmp = tmp->next));

    do
    {
        if (!strcmp(label, head->s.name))
        {
            found = true;
            break;
        }
    } while ((head = head->next));

    if (!found)
    {
        flag_register->ERR = 1;
        return;
    }

    for (i = 0; tmp->arr[i][0] != '?'; i++)
        ;

    if (head->s.attributes[EXTERN])
    {
        write_extern_to_file(fp, head->s.name, tmp->IC + i, tmp->IC + i + 1);
    }

    free(tmp->arr[i]);
    tmp->arr[i++] = encode_label_value(head->s.value - head->s.value % 16, head->s.attributes[EXTERN]);
    free(tmp->arr[i]);
    tmp->arr[i] = encode_label_offset(head->s.value % 16, head->s.attributes[EXTERN]);
}

/**
 * @brief checks if the line is data declaration.
 *
 * @param line the line to check.
 * @param flag_register the flag register.
 * @return true if the line is data declaration.
 */
bool is_data(char *line, PSW *flag_register)
{
    char *cmd;

    cmd = strtok(line, " ");
    if (flag_register->SYM)
        cmd = strtok(NULL, " "); /* get the next word (or the only word) */

    return !strcmp(cmd, ".data") || !strcmp(cmd, ".string");
}

/**
 * @brief handles entry declaration by updating the symbol list.
 *
 * @param line the line to check.
 * @param head the head of the symbol list.
 * @param flag_register the flag register.
 * @param line_number the current line number.
 */
void handle_entry(char *line, symbol_list *head, PSW *flag_register, int line_number)
{
    char *label_name;

    /* skip the .entry */
    strtok(line, " ");

    if (flag_register->SYM)
    {
        strtok(NULL, " ");
    }

    /* get the symbol after .entry */
    label_name = strtok(NULL, " ");

    if (!contains(head, label_name))
    {
        throw_error("Entry label does not exist!", line_number); /* Symbol does not exist */
        flag_register->ERR = 1;
        return;
    }
    else if (flag_register->SYM)
    {
        printf("Warning in line %d: Label and entry are declared in the same line.", line_number);
    }

    update_entry(head, label_name);
}

/**
 * @brief writes the entries to the entry file.
 * 
 * @param fp the entry file pointer.
 * @param head the head of the symbol list.
 */
void write_entry_to_file(FILE *fp, symbol_list *head)
{
    do
    {
        if (head->s.attributes[ENTRY])
        {
            fprintf(fp, "%s,%04d,%04d\n", head->s.name, head->s.base_address, head->s.offset);
        }
    } while ((head = head->next));
}

/**
 * @brief writes the externals to the extern file.
 * 
 * @param fp the extern file pointer.
 * @param name the name of the extern.
 * @param address the address of the word the extern is referenced in.
 * @param second_address the address of the second word the extern is referenced in.
 */
void write_extern_to_file(FILE *fp, char *name, int address, int second_address)
{
    fprintf(fp, "%s BASE %04d\n", name, address);
    fprintf(fp, "%s OFFSET %04d\n\n", name, second_address);
}

/**
 * @brief append a string to the object file
 * @param fp object file pointer
 * @param word the word to append
 */
void append_to_object_file(FILE *fp, char *word)
{
    if (fputs(word, fp) == EOF)
    {
        fprintf(stderr, "Failed to append to object file");
    }
}

/**
 * @brief writes the final encoding to the object file.
 * 
 * @param fp the object file pointer.
 * @param IC the IC.
 * @param DC the DC.
 * @param head the command list head.
 */
void format_object_file(FILE *fp, int IC, int DC, command_list *head)
{
    char str[WORD_SIZE] = {0}; /* creating a temporary string to hold the integers */
    int i = 0, cond = get_command_size(head), j = 0;

    sprintf(str, "%d", (IC - 100)); /* converting IC to char* */
    append_to_object_file(fp, str); /* appending it to the file */

    append_to_object_file(fp, " "); /* adding space */

    sprintf(str, "%d\n", DC);
    append_to_object_file(fp, str); /* same for DC */

    for (i = 0; i < cond; i++)
    {
        for (j = 0; j < head->L; j++)
        {
            fprintf(fp, "%04d %s\n", (head->IC + j), special_base(head->arr[j]));
        }
        head = head->next;
    }
}

/**
 * @brief this function creates a newly allocated substring from [start] to [end]
 * @param line substring origin
 * @param start the start index
 * @param end the end index
 * @return a newly allocated substring
 */
char *get_new_substring_with_indexes(char *line, int start, int end)
{
    char *tmp = (char *)malloc(strlen(line));
    int i = 0;

    for (; i + start < end; i++)
    {
        tmp[i] = line[i + start];
    }
    tmp[i++] = '\0';
    return tmp;
}

/**
 * @brief this function coverts a binary string to special base
 * @param line the binary string
 * @return special base string 
 */
char *special_base(char *line)
{
    int i = 0;
    int num = 0;
    char tmp[ENCODE_LENGTH] = {0}, *substr, letter = ' ', tok[4] = {0};
    for (i = 0; i < 5; i++)
    {
        substr = get_new_substring_with_indexes(line, i * 4, (i * 4 + 4));
        num = (int)strtol(substr, NULL, 2);
        letter = 'A' + i;
        strncat(tmp, &letter, 1);
        sprintf(tok, "%x", num);
        strncat(tmp, tok, 1);
        strncat(tmp, "-", 2);
        free(substr);
    }
    tmp[strlen(tmp) - 1] = '\0';
    strncpy(line, tmp, ENCODE_LENGTH);
    return line;
}
