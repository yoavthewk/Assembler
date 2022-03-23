#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_LENGTH 21
#define LINES 4

typedef struct CommandList {
    struct CommandList *next;
    int L;
    int IC;
    char** arr;
} CommandList;

/**
 * This function create a new list node.
 * Notice that the output is on the heap -
 * meaning it has memory allocated to it.
 * @param next optional, a pointer to the next node
 * @return a new node for the list
 */
CommandList *initCommandNode(CommandList* next, int L, int IC, char** strArray);

/**
 * This function inserts a node to the end of the list
 * @param head a pointer to the start of the list
 * @param node the node to insert
 */
void insertCommandLast(CommandList *head, CommandList *node);

/**
 * This function frees the list from the given node onwards
 * @param node the node to free from
 */
void freeCommandList(CommandList *node);

/**
 * This function prints a string linked list
 * @param head a pointer to the start of the list
 */
void printCommandList(CommandList *head);

#endif /* COMMAND_LIST_H */