#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ENCODE_LENGTH 21
#define LINES 4

typedef struct command_list {
    struct command_list *next;
    int L;
    int IC;
    bool data;
    char** arr;
} command_list;

/**
 * This function create a new list node.
 * Notice that the output is on the heap -
 * meaning it has memory allocated to it.
 * @param next optional, a pointer to the next node
 * @return a new node for the list
 */
command_list *init_command_node(command_list* next, int L, int IC, bool data, char** str_array);

/**
 * This function inserts a node to the end of the list
 * @param head a pointer to the start of the list
 * @param node the node to insert
 */
void insert_command_list(command_list **head, command_list *node);

/**
 * This function frees the list from the given node onwards
 * @param node the node to free from
 */
void free_command_list(command_list *node);

/**
 * This function prints a string linked list
 * @param head a pointer to the start of the list
 */
void print_command_list(command_list *head);

/**
 * @brief Get the size of the command list
 * @param head pointer to the head of the list
 * @return size_t size of the list
 */
size_t get_command_size(command_list* head);

#endif /* COMMAND_LIST_H */
