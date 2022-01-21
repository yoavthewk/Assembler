#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_LEN 81

/* Helper functions */
void executeCommand();
int isWhiteSpaceOnly(char input[]);

/* Execution functions */
void mov_exec(char input[]);
void cmp_exec(char input[]);
void add_exec(char input[]);
void sub_exec(char input[]);
void lea_exec(char input[]);
void clr_exec(char input[]);
void not_exec(char input[]);
void inc_exec(char input[]);
void dec_exec(char input[]);
void jmp_exec(char input[]);
void bne_exec(char input[]);
void jsr_exec(char input[]);
void red_exec(char input[]);
void prn_exec(char input[]);
void rts_exec(char input[]);
void stop_exec(char input[]);
