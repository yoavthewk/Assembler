#ifndef MACRO_LIST_SEEN
#define MACRO_LIST_SEEN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_COMMAND 83

/*
* Struct to represent a macro in the form of {name: data}
*/
typedef struct macro {
    char* name;
    char* data;
} macro;

/*
* Struct to represent a linked list containing macros
*/
typedef struct MacroList {
    macro m;
    struct MacroList* next;
} MacroList;

/*
* @brief initiates a node for the macro list
* @param next [optional]: a node that the newly created node will point to
* @param name: the name of the macro
* @param content: the content of the macro
* @return pointer to a newly created node, allocated on the heap
*/
MacroList* initNode(MacroList* next, char* name, char* content);

/*
* @brief frees all dynamic allocations done for the macro list
* @param head pointer to the start of the list
*/
void freeListFromMemory(MacroList* head);

/**
* @brief inserting a node at the end of the list
* @param head double pointer to the start of the list
* @param node node to insert
*/
void insertAtEnd(MacroList** head, MacroList* node);

/**
* @brief printing the command list
* @param head pointer to the start of the list
*/
void printCommandList(MacroList* head);

/**
* @brief finding a node at given poisiton
* @param head pointer to the start of the list
* @param place the index to retrieve node from
* @return node; null if not found
*/
MacroList* findNodeAt(MacroList* head, unsigned int place);

/**
* @brief return the size of the list
* @param head pointer to the start of the list
* @return the amount of nodes in the lists
*/
int getSize(MacroList* head);

#endif /* !MACRO_LIST_SEEN */