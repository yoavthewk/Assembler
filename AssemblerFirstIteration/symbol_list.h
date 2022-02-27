#ifndef SYMBOL_LIST_SEEN
#define SYMBOL_LIST_SEEN
#define SIZE_OF_COMMAND 80
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct symbol
{
    char name[5];
    unsigned char value;
    unsigned char baseAddress;
    unsigned char offset;
    bool attributes[4];
    /* att[0] = external; att[1] = entry; att[2] = code; att[3] = data; */
} symbol;

typedef struct SymbolList
{
    symbol s;
    struct SymbolList *next;
} SymbolList;

SymbolList *initSymbolNode(SymbolList *next, char name[], unsigned char value, unsigned char addr, unsigned char offset, bool att[]);
void freeSymbolList(SymbolList *head);
void insertSymbol(SymbolList **head, SymbolList *node);
int getSymbolListSize(SymbolList *head);

#endif /* !SYMBOL_LIST_SEEN */