#ifndef FIRSTITERATION_H
#define FIRSTITERATION_H
#include "../Hardware/hardware.h"
#include "symbol_list.h"
#include "../PreAssembler/pre_assembler.h"
#include "../CommandParsing/exec.h"
#include "command_list.h"

#define BASE 0
#define OFFSET 0

void firstIteration(char *file_name, FILE *fp, SymbolList* head, hregister* IC, hregister* DC, PSW* flagRegister, command_list *commad_head);

/* helper functions */
bool isValidLabel(char* label, SymbolList* head);
void contains_label(char *line, SymbolList* head, int line_number, PSW *flagRegister);
bool is_extern(char *line, PSW *flagRegister);
bool is_entry(char *line, PSW *flagRegister);
void process_data(char *line, hregister* DC, int line_number, PSW *flagRegister, command_list *head);
bool handle_data(char *line, SymbolList *head, hregister* IC, hregister* DC, int line_number, PSW *flagRegister, command_list *command_head);
void process_line(char *line, SymbolList *head, int line_number, hregister* IC, hregister* DC, PSW* flagRegister, command_list *command_head);
void handle_extern(char *line, SymbolList *head, int line_number, PSW *flagRegister);
void updateSymbolList(SymbolList* head, hregister* IC);
bool externousText(char *line, int operands, PSW *flagRegister, int line_number);
void check_illegal_commas(char* line, PSW* flagRegister);
bool contains_space(char* data, PSW* flagRegister);
void update_command_list(command_list *command_head, hregister* IC);

#endif /* FIRSTITERATION_H */
