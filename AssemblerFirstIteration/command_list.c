#include "command_list.h"

command_list *init_command_node(command_list *next, int L, int IC, bool data, char **str_array)
{
    int i = 0;
    command_list *tmp = (command_list *)calloc(sizeof(command_list), sizeof(command_list));

    tmp->IC = IC;
    tmp->L = L;
    tmp->arr = (char**)calloc(sizeof(char*) * LINES, sizeof(char*));
    tmp->data = data;

    for (i = 0; i < LINES; i++)
    {
        tmp->arr[i] = (char *)calloc(sizeof(char) * ENCODE_LENGTH, sizeof(char));
        if (str_array[i]){
            strcpy(tmp->arr[i], str_array[i]);
            free(str_array[i]);
        }
    }
    free(str_array);
    return tmp;
}

void insert_command_list(command_list *head, command_list *node)
{
    if (!node)
    {
        fprintf(stderr, "ERROR: node is pointing to NULL");
        return;
    }

    if (!head)
    {
        head = node;
        return;
    }

    while (head->next)
    {
        head = head->next;
    }
    head->next = node;
}

void print_command_list(command_list *head)
{
    if (!head)
        return;

    while (head)
    {
        printf("%s, %s, %s, %s; IC=%d, L=%d\n", head->arr[0], head->arr[1], head->arr[2], head->arr[3], head->IC, head->L);
        head = head->next;
    }
}

void free_command_list(command_list *node)
{
    command_list *tmp = node;

    if (!node)
        return;
    
    while (tmp)
    {
        tmp = node->next;
        free(node->arr[0]);
        free(node->arr[1]);
        free(node->arr[2]);
        free(node->arr[3]);
        free(node->arr);
        free(node);
        node = tmp;
    }
    node = NULL;
}
