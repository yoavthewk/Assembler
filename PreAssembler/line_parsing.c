#include "line_parsing.h"

char* parse_line(char* line){
    char new_line[strlen(line) + 1];
    int i = j = 0;

    while(i < strlen(line)){
        while(isspace(line[i]) && line[i] != '\n')
            i++;
    
        while(!(isspace(line[i]) && line[i] != '\n')) 
            new_line[j] = *(line + i++);
    
        line[i] = ' '; // replacing the white space with a space.

        while(isspace(line[i]) && line[i] != '\n')
            i++;
    }

    new_line[strlen(line)] = 0;
    return &new_line;
}

