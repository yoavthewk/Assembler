#ifndef FIRSTITERATION_H
#define FIRSTITERATION_H

#include "../Hardware/hardware.h"
#include "../PreAssembler/line_parsing.h"
#include "symbol_list.h"
#define BASE 0
#define OFFSET 0

void contains_label(char *line);
bool is_extern(char *line);
bool is_entry(char *line);
void process_data(char *line);
bool handle_data(char *line, SymbolList *head);
void process_line(char *line, SymbolList *head);
void handle_extern(char *line, SymbolList *head);

#endif /* FIRSTITERATION_H */