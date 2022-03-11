#ifndef SYMBOL_LIST_SEEN
#define SYMBOL_LIST_SEEN
#define SIZE_OF_COMMAND 80
#define SYMBOL_SIZE 83
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

SymbolList *initSymbolNode(SymbolList *next, char name[], unsigned int value, unsigned int addr, unsigned int offset, bool att[]);
void freeSymbolList(SymbolList *head);
void insertSymbol(SymbolList **head, SymbolList *node);
int getSymbolListSize(SymbolList *head);
bool contains(SymbolList *head, char *name);
void printSymbolList(SymbolList* head);

#endif /* !SYMBOL_LIST_SEEN */