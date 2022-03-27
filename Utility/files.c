#include "files.h"

/*
 * This function opens the file and returns the file pointer.
 * Parameters:
 * char* filename - the name of the file to open.
 */
FILE *open_file(char *filename, bool pre_assembled)
{
    FILE *fp;
    char *temp = (char *)malloc(strlen(filename) + strlen(".as") + 1);

    strcpy(temp, filename); /* getting file name with correct extension */
    if (pre_assembled)      /* .am */
    {
        get_file_name_pre(temp);
    }
    else /* .as */
    {
        get_file_name(temp);
    }

    fp = fopen(temp, "r"); /* opening file with read permissions */
    free(temp);            /* freeing the name from the heap */

    return fp;
}

/*
 * This function reads the next line from a file.
 * Parameters:
 * FILE* fp - the file pointer.
 */
char *get_next_line(FILE *fp)
{
    /* creating line of size 83 to get file input */
    char *input = (char *)calloc(MAX_LEN, sizeof(char));
    /* if line was read correctly and contains 80 chars or less */
    if (fgets(input, MAX_LEN, fp) && strlen(input) < MAX_LEN)
    {
        return input; /* return the line */
    }
    free(input); /* else, free the line and return null */
    return NULL;
}

/*
 * This function opens or creates the pre assembler file and returns the file pointer.
 * Parameters:
 * char* filename - the name of the file to open.
 */
FILE *open_file_create(char *file_name)
{
    FILE *fp;
    char *temp = (char *)calloc(strlen(file_name) + strlen(".am") + 1, sizeof(char));
    strcpy(temp, file_name); /* getting the file name with .am extension */
    get_file_name_pre(temp);
    fp = fopen(temp, "a"); /* opening the file */
    free(temp);            /* freeing the name */

    return fp;
}

/*
 * This function clears the pre-assembled file.
 * Parameters:
 * char* filename - the name of the file to open.
 */
int clear_file(char *file_name, char *extension)
{
    FILE *fp; /* initiating a file pointer */
    char *temp = (char *)calloc(strlen(file_name) + strlen(extension) + 1, sizeof(char));
    strcpy(temp, file_name); /* temp holds the name of the file with the .am extension */
    strncat(temp, extension, strlen(extension));
    fp = fopen(temp, "w"); /* opening with write permissions; therefore clearing */
    free(temp);            /* we have no use for the filename from now on */

    if (fp) /* if file was succesfully opened */
    {
        fclose(fp); /* close it */
        return 1;
    }
    return 0;
}

/*
 * This function writes a line to the pre-assembled file.
 * Parameters:
 * FILE* fp - the file pointer.
 * char* input - the line to write.
 */
void write_line(FILE *fp, char *input)
{
    fprintf(fp, "%s", input);
}

/**
 * @brief creates \ opens an object file.
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_object_file(char *file_name)
{
    FILE *object_fp;
    const int ob_length = 3;
    char *tmp_name = (char *)malloc(MAX_LEN);
    clear_file(file_name, ".ob");
    strcpy(tmp_name, file_name);
    strncat(tmp_name, ".ob", ob_length);
    object_fp = fopen(tmp_name, "a");
    free(tmp_name);
    return object_fp;
}

/**
 * @brief creates \ opens an entry file.
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_entry_file(char *file_name)
{
    FILE *entry_fp;
    const int ent_length = 4;
    char *tmp_name = (char *)malloc(MAX_LEN);
    clear_file(file_name, ".ent");
    strcpy(tmp_name, file_name);
    strncat(tmp_name, ".ent", ent_length);
    entry_fp = fopen(tmp_name, "a");
    free(tmp_name);
    return entry_fp;
}

/**
 * @brief creates \ opens an extern file.
 * @param file_name the name of the file
 * @return FILE* of the object file
 */
FILE *create_extern_file(char *file_name)
{
    const int ext_length = 4;
    FILE *ext_fp;
    char *tmp_name = (char *)malloc(MAX_LEN);
    clear_file(file_name, ".ext");
    strcpy(tmp_name, file_name);
    strncat(file_name, ".ext", ext_length);
    ext_fp = fopen(file_name, "a");
    free(tmp_name);
    return ext_fp;
}

/**
 * @brief deletes an object file.
 * @param file_name the name of the file
 */
void delete_object_file(char *file_name)
{
    const int ob_length = 3;
    char *tmp_name = (char *)malloc(MAX_LEN);
    clear_file(file_name, ".ob");
    strcpy(tmp_name, file_name);
    strncat(tmp_name, ".ob", ob_length);
    if (remove(tmp_name) != 0)
    {
        printf("Failed to delete %s", tmp_name);
    }
    free(tmp_name);
}

/**
 * @brief deletes an entry file.
 * @param file_name the name of the file
 */
void delete_entry_file(char *file_name)
{
    const int ob_length = 4;
    char *tmp_name = (char *)malloc(MAX_LEN);
    clear_file(file_name, ".ent");
    strcpy(tmp_name, file_name);
    strncat(tmp_name, ".ent", ob_length);
    if (remove(tmp_name) != 0)
    {
        printf("Failed to delete %s", tmp_name);
    }
    free(tmp_name);
}


/**
 * @brief deletes an extern file.
 * @param file_name the name of the file
 */
void delete_extern_file(char *file_name)
{
    const int ob_length = 4;
    char *tmp_name = (char *)malloc(MAX_LEN);
    clear_file(file_name, ".ext");
    strcpy(tmp_name, file_name);
    strncat(tmp_name, ".ext", ob_length);
    if (remove(tmp_name) != 0)
    {
        printf("Failed to delete %s", tmp_name);
    }
    free(tmp_name);
}


/**
 * @brief deletes an am file.
 * @param file_name the name of the file
 */
void delete_am_file(char *file_name)
{
    const int ob_length = 3;
    char *tmp_name = (char *)malloc(MAX_LEN);
    clear_file(file_name, ".am");
    strcpy(tmp_name, file_name);
    strncat(tmp_name, ".am", ob_length);
    if (remove(tmp_name) != 0)
    {
        printf("Failed to delete %s", tmp_name);
    }
    free(tmp_name);
}