#include "command_list.h"

command_list *init_command_node(command_list *next, int L, int IC, bool data, char **str_array)
{
    int i = 0;
    command_list *tmp = (command_list *)calloc(sizeof(command_list), sizeof(command_list));

    tmp->IC = IC;
    tmp->L = L;
    tmp->arr = (char**)calloc(sizeof(char*) * L, sizeof(char*));
    tmp->data = data;

    for (i = 0; i < L; i++)
    {
        tmp->arr[i] = (char *)calloc(sizeof(char) * ENCODE_LENGTH, sizeof(char));
        if (str_array[i]){
            strcpy(tmp->arr[i], str_array[i]);
            free(str_array[i]);
        }
        else {
            strncpy(tmp->arr[i], "????????????????????", ENCODE_LENGTH);
        }
    }
    free(str_array);
    return tmp;
}

void insert_command_list(command_list **head, command_list *node)
{
    command_list *temp = *head;
    int i = 0;
    static int callNum = 0;
    if (++callNum == 1)
    {
        temp->next = node->next;
        temp->IC = node->IC;
        temp->L = node->L;
        temp->data = node->data;
        for (i = 0; i < LINES; i++)
        {
            strcpy(temp->arr[i], node->arr[i]);
        }
        free_command_list(node);
        return;
    }

    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = node;
}

void print_command_list(command_list *head)
{
    int i = 0;
    if (!head)
        return;

    while (head)
    {
        printf("IC=%d, L=%d\n", head->IC, head->L);
        for (i = 0; i < head->L; i++)
        {
            puts(head->arr[i]);
        }
        
        head = head->next;
    }
}

void free_command_list(command_list *node)
{
    command_list *tmp = node;
    int i = 0;

    if (!node)
        return;
    
    while (tmp)
    {
        tmp = node->next;
        for (i = 0; i < node->L; ++i) {
            free(node->arr[i]);
        }
        free(node->arr);
        free(node);
        node = tmp;
    }
    node = NULL;
}

size_t get_command_size(command_list* head)
{
    size_t size = 0;
    while (head)
    {
        size++;
        head = head->next;
    }
    return size;
    
}