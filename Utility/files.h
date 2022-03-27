#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LEN 83

#define get_file_name(name) strcat(name, ".as")
#define get_file_name_pre(name) strcat(name, ".am")

/*
 * This function opens the file and returns the file pointer.
 * Parameters:
 * char* filename - the name of the file to open.
 */
FILE *open_file(char *filename, bool pre_assembled);

/*
 * This function reads the next line from a file.
 * Parameters:
 * FILE* fp - the file pointer.
 */
char *get_next_line(FILE *fp);

/*
 * This function opens or creates the pre assembler file and returns the file pointer.
 * Parameters:
 * char* filename - the name of the file to open.
 */
FILE *open_file_create(char *filename);

/*
 * This function writes a line to the pre-assembled file.
 * Parameters:
 * FILE* fp - the file pointer.
 * char* input - the line to write.
 */
void write_line(FILE *fp, char *input);

/*
 * This function clears the pre-assembled file.
 * Parameters:
 * char* filename - the name of the file to open.
 */
int clear_file(char *file_name, char *extension);

/**
 * @brief creates \ opens an object file.
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_object_file(char *file_name);

/**
 * @brief creates \ opens an entry file.
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_entry_file(char *file_name);

/**
 * @brief creates \ opens an extern file.
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_extern_file(char *file_name);

/**
 * @brief deletes an am file.
 * @param file_name the name of the file
 */
void delete_am_file(char *file_name);

/**
 * @brief deletes an extern file.
 * @param file_name the name of the file
 */
void delete_extern_file(char *file_name);

/**
 * @brief deletes an entry file.
 * @param file_name the name of the file
 */
void delete_entry_file(char *file_name);

/**
 * @brief deletes an object file.
 * @param file_name the name of the file
 */
void delete_object_file(char *file_name);

#endif /* FILES_H */