#ifndef HARDWARE_SEEN
#define HARDWARE_SEEN
#include <stdio.h>
#include <stdlib.h>
#define MEMORY_SIZE 8192 
#define NUM_OF_REGISTERS 20

typedef struct hregister {
    unsigned int data : 20;
} hregister;

typedef struct word {
    unsigned int data : 20;
} word;

typedef struct PSW {
    unsigned int Z : 1;
	unsigned int SYM : 1;
} PSW;

struct {
	char name[8];
	unsigned int op_code : 16;
	unsigned int funct : 4;
} static action_table[] = {
		{"mov", 0, 0},
		{"cmp", 1, 0},
		{"add", 2, 10},
		{"sub", 2, 11}, 
		{"lea", 8, 0}, 
		{"clr", 16, 10}, 
		{"not", 16, 11}, 
		{"inc", 16, 12}, 
		{"dec", 16, 13},
		{"jmp", 256, 10},
		{"bne", 256, 11},
		{"jsr", 256, 12},
		{"red", 2048, 0},
		{"prn", 4096, 0},
		{"rts", 8192, 0},
		{"stop", 16385, 0},
		{"invalid", 0, 0}
};
static word memory[MEMORY_SIZE] = {0};
static hregister registers[NUM_OF_REGISTERS] = {0};
static hregister IC = {100};
static hregister DC = {0};
static PSW flagRegister = {0, 0};

/* Initialization functions */
void init_memory();
void init_registers();
#endif /* !HARDWARE_SEEN */