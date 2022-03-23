#include "CommandList.h"

CommandList *initCommandNode(CommandList *next, int L, int IC, char *str1, char **strArray)
{
    int i = 0;
    CommandList *tmp = (CommandList *)malloc(sizeof(CommandList));

    tmp->IC = IC;
    tmp->L = L;
    tmp->arr = (char **)malloc(sizeof(char *) * LINES);

    for (i = 0; i < LINES; i++)
    {
        tmp->arr[i] = (char *)calloc(sizeof(char) * ENCODE_LENGTH, sizeof(char));
        if (strArray[i])
            strcpy(tmp->arr[i], strArray[i]);
    }
}

void insertCommandLast(CommandList *head, CommandList *node)
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

void printCommandList(CommandList *head)
{
    if (!head)
        return;

    while (head)
    {
        printf("%s, %s, %s, %s; IC=%d, L=%d", head->arr[0], head->arr[1], head->arr[2], head->arr[3], head->IC, head->L);
        head = head->next;
    }
}

void freeCommandList(CommandList *node)
{
    CommandList *tmp = node;

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