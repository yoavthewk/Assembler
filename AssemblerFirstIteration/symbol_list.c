#include "symbol_list.h"

/*
 * This function creates a node and allocates memory to it
 * Input: the next node, the name of the curr node, the value of the current node, the addr of the node,
 * the offset of the node, the attributes of the node.
 * Output: none
 */
SymbolList *initSymbolNode(SymbolList *next, char name[], unsigned int value, unsigned int addr, unsigned int offset, bool att[])
{
    int i = 0;
    SymbolList *node = (SymbolList *)malloc(sizeof(SymbolList));

    if (name)
    {
        strcpy(node->s.name, name);
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
    int i = 0;
    static int callNum = 0;
    if (++callNum == 1)
    {
        temp->next = node->next;
        strcpy(temp->s.name, node->s.name);
        temp->s.offset = node->s.offset;
        temp->s.value = node->s.value;
        temp->s.baseAddress = node->s.baseAddress;
        for (i = 0; i < 4; i++)
        {
            temp->s.attributes[i] = node->s.attributes[i];
        }
        free(node);
        return;
    }

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

bool contains(SymbolList *head, char *name)
{
    do
    {
        if (!strncmp(head->s.name, name, SYMBOL_SIZE))
            return true;
    } while ((head = head->next));
    return false;
}

void updateEntry(SymbolList* head, char *name){
    do
    {
        if (!strncmp(head->s.name, name, SYMBOL_SIZE)){
            head->s.attributes[ENTRY] = true;
            return true;
        }
    } while ((head = head->next));
    return false;
}

void printSymbolList(SymbolList *head)
{
    int i;
    while (head)
    {
        printf("name: %s\nval: %d\noffset: %d\nbase: %d\n", head->s.name, head->s.value, head->s.offset, head->s.baseAddress);
        head = head->next;
    }
}