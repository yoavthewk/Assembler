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
    unsigned int ERR : 1;
	unsigned int SYM : 1;
} PSW;

static word memory[MEMORY_SIZE] = {0};
static hregister registers[NUM_OF_REGISTERS] = {0};
static PSW flagRegister = {0, 0};

#endif /* !HARDWARE_SEEN */
