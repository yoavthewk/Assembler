#include "macro_list.h"

macro_list* init_node(macro_list* next, char* name, char* content)
{
    macro_list* node = (macro_list*)malloc(sizeof(macro_list));
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

void free_macro_list(macro_list *head)
{
    macro_list* temp;
    while(head){
        temp = head;
        head = head->next;
        free(temp->m.data);
        free(temp->m.name);
        free(temp);
    }
}

void insert_at_end(macro_list** head, macro_list* node)
{
    macro_list * temp = *head;
    int i = 0;
    
    if (!strcmp(temp->m.name, "mov"))
    {
        temp->next = node->next;
        strcpy(temp->m.name, node->m.name);
        strcpy(temp->m.data, node->m.data);
        free(node->m.data);
        free(node->m.name);
        free(node);
        return;
    }

    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = node;
}

void print_macro_list(macro_list* head)
{
    while (head)
    {
        printf("Name: %s\n{\n\t%s\n}\n", head->m.name, head->m.data);
        head = head->next;
    }
}

int get_size(macro_list* head)
{
    if (!head->next)
    {
        return 1;
    }
    return get_size(head->next) + 1;
}
