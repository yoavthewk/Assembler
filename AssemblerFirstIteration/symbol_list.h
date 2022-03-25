#ifndef SYMBOL_LIST_SEEN
#define SYMBOL_LIST_SEEN
#define SIZE_OF_COMMAND 80
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
    unsigned int baseAddress;
    unsigned int offset;
    bool attributes[4];
    /* att[0] = external; att[1] = entry; att[2] = code; att[3] = data; */
} symbol;

typedef struct SymbolList
{
    symbol s;
    struct SymbolList *next;
} SymbolList;

/*
 * This function creates a node and allocates memory to it
 * Input: the next node, the name of the curr node, the value of the current node, the addr of the node,
 * the offset of the node, the attributes of the node.
 * Output: none
 */
SymbolList *initSymbolNode(SymbolList *next, char name[], unsigned int value, unsigned int addr, unsigned int offset, bool att[]);

/*
 * This function releases the memory allocated to the linked list
 * Input: the head of the list
 * Output: none
 */
void freeSymbolList(SymbolList *head);

/*
 * This function inserts a node at the end of the list
 * Input: the head of the list, the node to insert
 * Output: none
 */
void insertSymbol(SymbolList **head, SymbolList *node);

/*
 * This function returns the size of a linked list
 * Input: the head of the list
 * Output: the size of the list
 */
int getSymbolListSize(SymbolList *head);

/*
* This function checks if the symbol list contains a symbol by name
* Input: the head of the list, the name to find
* Output: true if name in list
*/
bool contains(SymbolList *head, char *name);

/*
* This function updates an attribute in a node, by name
* Input: the head, and the name of the node to update
* Output: none
*/
void updateEntry(SymbolList* head, char *name);

/*
* This function prints the symbol list
* Input: the head of the list
* Output: none
*/
void printSymbolList(SymbolList* head);

#endif /* !SYMBOL_LIST_SEEN */