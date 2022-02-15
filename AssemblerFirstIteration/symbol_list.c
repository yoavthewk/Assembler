#include "symbol_list.h"

/*
 * This function creates a node and allocates memory to it
 * Input: the next node, the name of the curr node, the value of the current node, the addr of the node,
 * the offset of the node, the attributes of the node.
 * Output: none
 */
SymbolList *initSymbolNode(SymbolList *next, char name[], unsigned char value, unsigned char addr, unsigned char offset, bool att[])
{
    int i = 0;
    SymbolList *node = (SymbolList *)malloc(sizeof(SymbolList));

    if (name)
    {
        strncpy(node->s.name, name, 4);
    }

    node->s.baseAddress = addr;
    node->s.offset = offset;
    node->s.value = value;
    for (i = 0; i < 4; i++)
    {
        node->s.attributes[i] = att[i];
    }

    node->next = next;
    return node;
}

/*
 * This function releases the memory allocated to the linked list
 * Input: the head of the list
 * Output: none
 */
void freeSymbolList(SymbolList *head)
{
    /*
    if (head)
    {
        freeListFromMemory(head->next);
    }
    free(head->m.data);
    free(head->m.name);
    free(head);
    */

    SymbolList *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/*
 * This function inserts a node at the end of the list
 * Input: the head of the list, the node to insert
 * Output: none
 */
void insertSymbol(SymbolList **head, SymbolList *node)
{
    SymbolList *temp = *head;
    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = node;
}

/*
 * This function returns the size of a linked list
 * Input: the head of the list
 * Output: the size of the list
 */
int getSymbolListSize(SymbolList *head)
{
    if (!head->next)
    {
        return 1;
    }
    return getSymbolListSize(head->next) + 1;
}