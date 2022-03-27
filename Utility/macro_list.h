#ifndef MACRO_LIST_SEEN
#define MACRO_LIST_SEEN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_COMMAND 83
#define DEFAULT_MACRO_NAME "fish"

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
typedef struct macro_list {
    macro m;
    struct macro_list* next;
} macro_list;

/*
* @brief initiates a node for the macro list
* @param next [optional]: a node that the newly created node will point to
* @param name: the name of the macro
* @param content: the content of the macro
* @return pointer to a newly created node, allocated on the heap
*/
macro_list* init_node(macro_list* next, char* name, char* content);

/*
* @brief frees all dynamic allocations done for the macro list
* @param head pointer to the start of the list
*/
void free_macro_list(macro_list* head);

/**
* @brief inserting a node at the end of the list
* @param head double pointer to the start of the list
* @param node node to insert
*/
void insert_at_end(macro_list** head, macro_list* node);

/**
* @brief printing the command list
* @param head pointer to the start of the list
*/
void print_macro_list(macro_list* head);

/**
* @brief return the size of the list
* @param head pointer to the start of the list
* @return the amount of nodes in the lists
*/
int get_size(macro_list* head);

#endif /* !MACRO_LIST_SEEN */