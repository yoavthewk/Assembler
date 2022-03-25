#ifndef HARDWARE_SEEN
#define HARDWARE_SEEN
#include <stdio.h>
#include <stdlib.h>
#define MEMORY_SIZE 8192 
#define NUM_OF_REGISTERS 20

/* struct to represent a register with 20 bits of memory */
typedef struct hregister {
    unsigned int data : 20;
} hregister;

/* struct to represent a word in assmebly; also 20 bits */
typedef struct word {
    unsigned int data : 20;
} word;

/* special flag register that holds two flags, each one bit */
typedef struct PSW {
    unsigned int ERR : 1; /* error flag */
	unsigned int SYM : 1; /* symbol flag */
} PSW;

#endif /* !HARDWARE_SEEN */
