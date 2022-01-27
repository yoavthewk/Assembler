#include <stdio.h>
#include <stdlib.h>
#define MEMORY_SIZE 8192 
#define NUM_OF_REGISTERS 20

typedef struct hregister{
    int data : 20;
}hregister;

typedef struct word{
    int data : 20;
}word;

typedef struct PSW{
    int Z : 1;
}PSW;

struct{
	char *name;
	int op_code : 16;
	int funct : 4;
	}action_table[] = {
		{"mov", 0, NULL},
		{"cmp", 1, NULL},
		{"add", 2, 10},
		{"sub", 2, 11}, 
		{"lea", 8, NULL}, 
		{"clr", 16, 10}, 
		{"not", 16, 11}, 
		{"inc", 16, 12}, 
		{"dec", 16, 13},
		{"jmp", 256, 10},
		{"bne", 256, 11},
		{"jsr", 256, 12},
		{"red", 2048, NULL},
		{"prn", 4096, NULL},
		{"rts", 8192, NULL},
		{"stop", 16385, NULL},
		{"not_valid", NULL}
	};

word memory[MEMORY_SIZE] = {0};
hregister registers[NUM_OF_REGISTERS] = {0};
hregister PC = {100};

/* Initialization functions */
void init_memory();
void init_registers();
