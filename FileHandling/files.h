#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 81

#define get_file_name(name) strcat(name, ".as")

/*
 * This function opens the file and returns the file pointer.
 * Parameters:
 * char* filename - the name of the file to open.
*/
FILE* open_file(char* filename);

/*
 * This function reads the next line from a file.
 * Parameters:
 * FILE* fp - the file pointer.
*/
char* get_next_line(FILE* fp);