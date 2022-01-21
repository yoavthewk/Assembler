#include "linked_list.h"

/*
* This function creates a node and allocates memory to it
* Input:
* Optional - next, the next node (can be NULL)
* Optional - cmd, the data of the initialized node (can be NULL)
* Output: none
*/
LinkedList* initNode(LinkedList* next, char cmd[])
{
    LinkedList* node = (LinkedList*)malloc(sizeof(LinkedList));
    node->next = next;
    strncpy(node->command, cmd, SIZE_OF_COMMAND);
    return node;
}

/*
* This function releases the memory allocated to the linked list
* Input: the head of the list
* Output: none
*/
void freeListFromMemory(LinkedList *head)
{
    if (head)
    {
        freeListFromMemory(head->next);
    }
    free(head);
}

/*
* This function inserts a node at the end of the list
* Input: the head of the list, the node to insert
* Output: none
*/
void insertAtEnd(LinkedList** head, LinkedList* node)
{
    LinkedList* temp = *head;
    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = node;
}

/*
* This function prints a linked List
* Input: the head of the list
* Output: none
*/
void printCommandList(LinkedList* head)
{
    while (head)
    {
        puts(head->command);
        head = head->next;
    }
}

/*
* This function prints a linked List
* Input: the head of the list
* Output: none
*/
LinkedList* findNodeAt(LinkedList* head, const unsigned int place)
{
    int count = 0;
    while (place != count)
    {
        head = head->next;
        count++;
    }
    return head;
}

int main()
{
    LinkedList* head, *second, *third;
    third = initNode(NULL, "Command of 3");
    second = initNode(third, "Command of 2");
    head = initNode(second, "Command of 1");


    printCommandList(head);
    freeListFromMemory(head);
    return 0;
}
