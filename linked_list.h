 #include <stdio.h>

#define SIZE_OF_COMMAND 80

typedef struct LinkedList {
    char command[SIZE_OF_COMMAND];
    struct Linkedlist* next;
} LinkedList;

LinkedList* initNode(LinkedList* next, char cmd[]);
void freeListFromMemory(LinkedList* head);
void insertAtEnd(LinkedList** head, LinkedList* node);
void printCommandList(LinkedList* head);