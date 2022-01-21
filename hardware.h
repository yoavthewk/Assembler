#define MEMORY_SIZE 8192 
#define NUM_OF_REGISTERS 20

typedef struct hregister{
    int data : 20;
}hregister;

typedef struct word{
    int data : 20;
}word;

word memory[MEMORY_SIZE] = {0};
hregister registers[NUM_OF_REGISTERS] = {0};

void init_memory();
void init_registers();