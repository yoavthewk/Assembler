#include "files.h"

FILE *open_file(char *filename, bool beenPreAssembled)
{
    FILE *fp;
    char *temp = (char *)malloc(strlen(filename) + strlen(".as") + 1);

    strcpy(temp, filename);
    if (beenPreAssembled)
    {
        get_file_name_pre(temp);
    }
    else
    {
        get_file_name(temp);
    }

    fp = fopen(temp, "r");
    free(temp);

    return fp;
}

char *get_next_line(FILE *fp)
{
    char *input = (char *)calloc(MAX_LEN, sizeof(char));
    if (fgets(input, MAX_LEN, fp) && strlen(input) < MAX_LEN)
    {
        return input;
    }
    free(input);
    return NULL;
}

FILE *open_file_create(char *file_name)
{
    FILE *fp;
    char *temp = (char *)calloc(strlen(file_name) + strlen(".am") + 1, sizeof(char));
    strcpy(temp, file_name);
    get_file_name_pre(temp);
    fp = fopen(temp, "a");
    free(temp);

    return fp;
}

int clear_file(char *file_name)
{
    FILE *fp;
    char *temp = (char *)calloc(strlen(file_name) + strlen(".am") + 1, sizeof(char));
    strcpy(temp, file_name);
    get_file_name_pre(temp);
    fp = fopen(temp, "w");
    free(temp);

    if (fp)
    {
        fclose(fp);
        return 1;
    }
    return 0;
}

void write_line(FILE *fp, char *input)
{
    fprintf(fp, "%s", input);
}