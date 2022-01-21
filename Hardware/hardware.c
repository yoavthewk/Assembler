#include "hardware.h"

int main(){
    init_memory();
    init_registers();
    
    return 0;
}

/* Initialize the memory */
void init_memory(){
    int i;
    
    /* Initialize the memory to zeros. */
    for(i = 0; i < MEMORY_SIZE; i++){
        memory[i].data = 0;
    }
}

/* Initialize the registers */
void init_registers(){
    int i;

    /* Initialize the registers to zeros. */
    for(i = 0; i < NUM_OF_REGISTERS; i++){
        registers[i].data = 0;
    }
}