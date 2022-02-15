#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 83

#define get_file_name(name) strcat(name, ".as")
#define get_file_name_pre(name) strcat(name, ".am")

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

/*
 * This function opens or creates the pre assembler file and returns the file pointer.
 * Parameters:
 * char* filename - the name of the file to open.
*/
FILE* open_file_create(char* filename);

/*
 * This function writes a line to the pre-assembled file.
 * Parameters:
 * FILE* fp - the file pointer.
 * char* input - the line to write.
*/
void write_line(FILE* fp, char *input);

/*
 * This function clears the pre-assembled file.
 * Parameters:
 * char* filename - the name of the file to open.
*/
int clear_file(char* file_name);
