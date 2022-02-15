#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Hardware/hardware.h"
#include "../FileHandling/files.h"
#define BASE 0 
#define OFFSET 0

void contains_label(char* line);
bool is_extern(char* line);
bool is_entry(char* line);
void process_data(char* line);
bool handle_data(char* line, symbolList *head);
void process_line(char *line, symbolList* head);