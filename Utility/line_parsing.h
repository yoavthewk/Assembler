#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include "hardware.h"
#include <string.h>
#include <ctype.h>

#define MAX_LEN 83

char* parse_line(char* line);
char *parse_line_first_iteration(char *line, PSW *flagRegister);

#endif /* PRE_ASSEMBLER_H */