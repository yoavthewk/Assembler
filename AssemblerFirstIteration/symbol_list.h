#ifndef SYMBOL_LIST_SEEN
#define SYMBOL_LIST_SEEN
#define SYMBOL_SIZE 83
#define ENTRY 1
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct symbol
{
    char name[SYMBOL_SIZE];
    unsigned int value;
    unsigned int base_address;
    unsigned int offset;
    bool attributes[4];
    /* att[0] = external; att[1] = entry; att[2] = code; att[3] = data; */
} symbol;

typedef struct symbol_list
{
    symbol s;
    struct symbol_list *next;
} symbol_list;

/*
 * This function creates a node and allocates memory to it
 * Input: the next node, the name of the curr node, the value of the current node, the addr of the node,
 * the offset of the node, the attributes of the node.
 * Output: none
 */
symbol_list *init_symbol_node(symbol_list *next, char name[], unsigned int value, unsigned int addr, unsigned int offset, bool att[]);

/*
 * This function releases the memory allocated to the linked list
 * Input: the head of the list
 * Output: none
 */
void free_symbol_list(symbol_list *head);

/*
 * This function inserts a node at the end of the list
 * Input: the head of the list, the node to insert
 * Output: none
 */
void insert_symbol(symbol_list **head, symbol_list *node);

/*
 * This function returns the size of a linked list
 * Input: the head of the list
 * Output: the size of the list
 */
int get_symbol_list_size(symbol_list *head);

/*
* This function checks if the symbol list contains a symbol by name
* Input: the head of the list, the name to find
* Output: true if name in list
*/
bool contains(symbol_list *head, char *name);

/*
* This function updates an attribute in a node, by name
* Input: the head, and the name of the node to update
* Output: none
*/
void update_entry(symbol_list* head, char *name);

/*
* This function prints the symbol list
* Input: the head of the list
* Output: none
*/
void print_symbol_list(symbol_list* head);

#endif /* !SYMBOL_LIST_SEEN */