#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE_OF_COMMAND 80

typedef struct symbol {
    char name[5];
    unsigned char value;
    unsigned char baseAddress;
    unsigned char offset;
    bool attributes[4];
    /* att[0] = external; att[1] = entry; att[2] = code; att[3] = data; */
} symbol;

typedef struct symbolList {
    symbol s;
    struct symbolList* next;
} symbolList;

symbolList* initList(symbolList* next, char name[], unsigned char value, unsigned char addr, unsigned char offset, bool att[]);
void freeListFromMemory(symbolList* head);
void insertAtEnd(symbolList** head, symbolList* node);
int getSize(symbolList* head);