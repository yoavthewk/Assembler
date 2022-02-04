#include "macro_list.h"

/*
* This function creates a node and allocates memory to it
* Input:
* Optional - next, the next node (can be NULL)
* Optional - name, the name of the macro
* Optional - data, the data of the macro
* Output: none
*/
MacroList* initNode(MacroList* next, char* name, char* content)
{
    MacroList* node = (MacroList*)malloc(sizeof(MacroList));
    if(content != NULL){
        node->m.data = (char*)malloc(strlen(content) + 1);
        strcpy(node->m.data, content);
    }

    if(name != NULL){
        node->m.name = (char*)malloc(strlen(name) + 1);
        strcpy(node->m.name, name);    
    }

    node->next = next;
    return node;
}

/*
* This function releases the memory allocated to the linked list
* Input: the head of the list
* Output: none
*/
void freeListFromMemory(MacroList *head)
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

    MacroList* temp;
    while(head){
        temp = head;
        head = head->next;
        free(temp->m.data);
        free(temp->m.name);
        free(temp);
    }
}

/*
* This function inserts a node at the end of the list
* Input: the head of the list, the node to insert
* Output: none
*/
void insertAtEnd(MacroList** head, MacroList* node)
{
    MacroList* temp = *head;
    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = node;
}

/*
* This function prints a linked List
* Input: the head of the list
* Output: nonevoid writeListToFile(MacroList* head, FILE* f);
void writeListToFile(MacroList* head, FILE* f);
*/
void printCommandList(MacroList* head)
{
    while (head)
    {
        printf("Name: %s\n{\n\t%s\n}\n", head->m.name, head->m.data);
        head = head->next;
    }
}


/*
* This function returns the size of a linked list
* Input: the head of the list
* Output: the size of the list
*/
int getSize(MacroList* head)
{
    if (!head->next)
    {
        return 1;
    }
    return getSize(head->next) + 1;
}

/*
* This function finds a node at a specific index
* Input: the head of the list, the place of the node
* Output: the node which has been found, NULL if out of bounds
*/
MacroList* findNodeAt(MacroList* head, unsigned int place)
{
    if (place >= getSize(head))
    {
        puts("This index is out of bounds");
        return NULL;
    }

    while (place > 0)
    {
        head = head->next;
        place--;
    }
    
    return head;
}


/* This is a main used to debug MacroList.c and MacroList.h 
int main()
{
    MacroList* head, *second, *third;
    third = initNode(NULL, "3", "Command of 3");
    second = initNode(third, "2", "Command of 2");
    head = initNode(second, "1", "Command of 1");

    printCommandList(head);
    freeListFromMemory(head);
    return 0;
}
*/