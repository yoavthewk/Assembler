#ifndef FIRSTITERATION_H
#define FIRSTITERATION_H
#include "../Hardware/hardware.h"
#include "symbol_list.h"
#include "../PreAssembler/pre_assembler.h"

#define BASE 0
#define OFFSET 0

void contains_label(char *line);
bool is_extern(char *line);
bool is_entry(char *line);
void process_data(char *line, hregister* DC);
bool handle_data(char *line, SymbolList *head, hregister* IC, hregister* DC);
void process_line(char *line, SymbolList *head, int line_number, hregister* IC, hregister* DC);
void handle_extern(char *line, SymbolList *head);
void firstIteration(char *file_name, FILE *fp, SymbolList* head, hregister* IC, hregister* DC);


#endif /* FIRSTITERATION_H */