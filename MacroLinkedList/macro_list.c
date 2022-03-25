#include "macro_list.h"

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

void freeListFromMemory(MacroList *head)
{
    MacroList* temp;
    while(head){
        temp = head;
        head = head->next;
        free(temp->m.data);
        free(temp->m.name);
        free(temp);
    }
}

void insertAtEnd(MacroList** head, MacroList* node)
{
    MacroList* temp = *head;
    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = node;
}

void printCommandList(MacroList* head)
{
    while (head)
    {
        printf("Name: %s\n{\n\t%s\n}\n", head->m.name, head->m.data);
        head = head->next;
    }
}

int getSize(MacroList* head)
{
    if (!head->next)
    {
        return 1;
    }
    return getSize(head->next) + 1;
}

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
