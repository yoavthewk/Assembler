#include "exec.h"

void mov_exec(char *line){
    int num;
    if(line[0] == ' ') strcpy(line, line + 1);
    else{
        /* throw an error */
        return;
    }

    isImmediate(line, &num);
    
}

void cmp_exec(char *input){

}

void add_exec(char *input){

}

void sub_exec(char *input){

}

void lea_exec(char *input){

}

void clr_exec(char *input){

}

void not_exec(char *input){

}

void inc_exec(char *input){

}

void dec_exec(char *input){

}

void jmp_exec(char input[]){

}

void bne_exec(char input[]){

}

void jsr_exec(char input[]){

}

void red_exec(char input[]){

}

void prn_exec(char input[]){

}

void rts_exec(char input[]){

}

void stop_exec(char input[]){

}
