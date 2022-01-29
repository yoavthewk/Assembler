#include "line_parsing.h"

char* parse_line(char* line){
    char* new_line;
    int i, j;
    if (!line)
    {
        return NULL;
    }
    new_line = (char*)calloc(MAX_LEN, sizeof(char));
    i = j = 0;

    while(i < strlen(line)){
        while(isspace(line[i]) && line[i] != '\n'  && strlen(line) > i)
            i++;
    
        while(!(isspace(line[i]) && line[i] != '\n') && strlen(line) > i) 
            new_line[j++] = *(line + i++);
    
        line[i] = ' '; /* replacing the white space with a space. */

        while(isspace(line[i]) && line[i] != '\n'  && strlen(line) > i)
            i++;
    }

    new_line[strlen(line)] = 0;
    free(line); 
    return new_line;
}