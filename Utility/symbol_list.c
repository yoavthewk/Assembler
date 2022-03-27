#include "symbol_list.h"

/*
 * This function creates a node and allocates memory to it
 * Input: the next node, the name of the curr node, the value of the current node, the addr of the node,
 * the offset of the node, the attributes of the node.
 * Output: none
 */
symbol_list *init_symbol_node(symbol_list *next, char name[], unsigned int value, unsigned int addr, unsigned int offset, bool att[])
{
    int i = 0;
    symbol_list *node = (symbol_list *)malloc(sizeof(symbol_list));

    if (name)
    {
        strcpy(node->s.name, name);
    }
    node->s.base_address = addr;
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
void free_symbol_list(symbol_list *head)
{
    symbol_list *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/*
 * This function inserts a node at the end of the list
 * Input: the head of the list, the node to insert, is new file
 * Output: none
 */
void insert_symbol(symbol_list **head, symbol_list *node)
{
    symbol_list *temp = *head;
    int i = 0;
    
    if (!strcmp(temp->s.name, DEFAULT_SYMBOL_NAME))
    {
        temp->next = node->next;
        strcpy(temp->s.name, node->s.name);
        temp->s.offset = node->s.offset;
        temp->s.value = node->s.value;
        temp->s.base_address = node->s.base_address;
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
int get_symbol_list_size(symbol_list *head)
{
    if (!head->next)
    {
        return 1;
    }
    return get_symbol_list_size(head->next) + 1;
}

/*
* This function checks if the symbol list contains a symbol by name
* Input: the head of the list, the name to find
* Output: true if name in list
*/
bool contains(symbol_list *head, char *name)
{
    do
    {
        if (!strncmp(head->s.name, name, SYMBOL_SIZE))
            return true;
    } while ((head = head->next));
    return false;
}

/*
* This function updates an attribute in a node, by name
* Input: the head, and the name of the node to update
* Output: none
*/
void update_entry(symbol_list* head, char *name){
    do
    {
        if (!strncmp(head->s.name, name, SYMBOL_SIZE)){
            head->s.attributes[ENTRY] = true;
            return;
        }
    } while ((head = head->next));
}

/*
* This function prints the symbol list
* Input: the head of the list
* Output: none
*/
void print_symbol_list(symbol_list *head)
{
    while (head)
    {
        printf("name: %s\nval: %d\noffset: %d\nbase: %d\n", head->s.name, head->s.value, head->s.offset, head->s.base_address);
        head = head->next;
    }
}