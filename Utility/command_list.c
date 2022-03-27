#include "command_list.h"

/**
 * This function create a new list node.
 * Notice that the output is on the heap -
 * meaning it has memory allocated to it.
 * @param next optional, a pointer to the next node
 * @return a new node for the list
 */
command_list *init_command_node(command_list *next, int L, int IC, bool data, char **str_array)
{
    int i = 0;
    command_list *tmp = (command_list *)calloc(sizeof(command_list), sizeof(command_list));

    tmp->IC = IC;
    tmp->L = L;
    tmp->arr = (char **)calloc(sizeof(char *) * L, sizeof(char *));
    tmp->data = data;

    for (i = 0; i < L; i++)
    {
        tmp->arr[i] = (char *)calloc(sizeof(char) * ENCODE_LENGTH, sizeof(char));
        if (str_array[i])
        {
            strcpy(tmp->arr[i], str_array[i]);
            free(str_array[i]);
        }
        else
        {
            strncpy(tmp->arr[i], "????????????????????", ENCODE_LENGTH);
        }
    }

    free(str_array);
    return tmp;
}


/**
 * This function inserts a node to the end of the list
 * @param head a pointer to the start of the list
 * @param node the node to insert
 */
void insert_command_list(command_list **head, command_list *node)
{
    command_list *temp = *head;
    int i = 0;
    static int callNum = 0;
    if (++callNum == 1)
    {
        temp->next = node->next;
        temp->IC = node->IC;
        temp->data = node->data;
        for (i = 0; i < temp->L; i++)
        {
            strcpy(temp->arr[i], node->arr[i]);
        }
        temp->L = node->L;
        free_command_list(node);
        return;
    }

    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = node;
}


/**
 * This function prints a string linked list
 * @param head a pointer to the start of the list
 */
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
            if (head->arr[i])
                puts(head->arr[i]);
        }

        head = head->next;
    }
}

/**
 * This function frees the list from the given node onwards
 * @param node the node to free from
 */
void free_command_list(command_list *node)
{
    command_list *tmp = node;
    int i = 0;

    if (!node)
        return;

    while (tmp)
    {
        tmp = node->next;
        for (i = 0; i < node->L; ++i)
        {
            free(node->arr[i]);
        }
        free(node->arr);
        free(node);
        node = tmp;
    }
    node = NULL;
}

/**
 * @brief Get the size of the command list
 * @param head pointer to the head of the list
 * @return size_t size of the list
 */
size_t get_command_size(command_list *head)
{
    size_t size = 0;
    while (head)
    {
        size++;
        head = head->next;
    }
    return size;
}