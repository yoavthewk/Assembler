#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_COMMAND 80

typedef struct macro {
    char* name;
    char* data;
} macro;

typedef struct MacroList {
    macro m;
    struct MacroList* next;
} MacroList;

MacroList* initNode(MacroList* next, char* name, char* content);
void freeListFromMemory(MacroList* head);
void insertAtEnd(MacroList** head, MacroList* node);
void printCommandList(MacroList* head);
MacroList* findNodeAt(MacroList* head, unsigned int place);
int getSize(MacroList* head);
