#include "files.h"

FILE *open_file(char *filename, bool pre_assembled)
{
    FILE *fp;
    char *temp = (char *)malloc(strlen(filename) + strlen(".as") + 1);

    strcpy(temp, filename); /* getting file name with correct extension */
    if (pre_assembled) /* .am */
    {
        get_file_name_pre(temp);
    }
    else /* .as */
    {
        get_file_name(temp);
    }

    fp = fopen(temp, "r"); /* opening file with read permissions */
    free(temp); /* freeing the name from the heap */

    return fp;
}

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

FILE *open_file_create(char *file_name)
{
    FILE *fp;
    char *temp = (char *)calloc(strlen(file_name) + strlen(".am") + 1, sizeof(char));
    strcpy(temp, file_name); /* getting the file name with .am extension */
    get_file_name_pre(temp);
    fp = fopen(temp, "a"); /* opening the file */
    free(temp); /* freeing the name */

    return fp;
}

int clear_file(char *file_name)
{
    FILE *fp; /* initiating a file pointer */
    char *temp = (char *)calloc(strlen(file_name) + strlen(".am") + 1, sizeof(char));
    strcpy(temp, file_name); /* temp holds the name of the file with the .am extension */
    get_file_name_pre(temp);
    fp = fopen(temp, "w"); /* opening with write permissions; therefore clearing */
    free(temp); /* we have no use for the filename from now on */

    if (fp) /* if file was succesfully opened */
    {
        fclose(fp); /* close it */
        return 1;
    }
    return 0;
}

void write_line(FILE *fp, char *input)
{
    fprintf(fp, "%s", input);
}