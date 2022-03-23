#include "pre_assembler.h"

char *parse_line(char *line)
{
    char *new_line;
    int i, j;
    if (!line)
    {
        free(line);
        return NULL;
    }
    new_line = (char *)calloc(MAX_LEN, sizeof(char));
    i = j = 0;

    while (i < strlen(line))
    {
        while (isspace(line[i]) && line[i] != '\n' && strlen(line) > i) /* left space trim */
            i++;

        while (!(isspace(line[i]) && line[i] != '\n') && strlen(line) > i)
            new_line[j++] = *(line + i++);

        new_line[j++] = ' '; /* replacing the white space with a space. */

        while (isspace(line[i]) && line[i] != '\n' && strlen(line) > i) /* right space trim */
            i++;
    }

    if (j <= 2)
    {
        new_line[0] = '\n';
        new_line[1] = 0;
    }
    else
    {
        new_line[j - 2] = '\n';
        new_line[j - 1] = 0;
    }
    free(line);
    return new_line;
}

char *parse_line_first_iteration(char *line, PSW *flagRegister)
{
    char *new_line;
    char last_char = ' ';
    int i, j, commaCount = 0;
    if (!line)
    {
        free(line);
        return NULL;
    }
    new_line = (char *)calloc(MAX_LEN, sizeof(char));
    i = j = 0;

    while (i < strlen(line))
    {
        while (isspace(line[i]) && line[i] != '\n' && strlen(line) > i) /* left space trim */
            i++;

        while (!(isspace(line[i]) && line[i] != '\n') && strlen(line) > i)
        {
            new_line[j++] = *(line + i);
            last_char = *(line + i);
            i++;
        }
        if (last_char != ',')
        {
            new_line[j++] = ' '; /* replacing the white space with a space. */
        }

        while (isspace(line[i]) && line[i] != '\n' && strlen(line) > i) /* right space trim */
            i++;
    }

    if (j <= 2)
    {
        new_line[0] = '\n';
        new_line[1] = 0;
    }
    else
    {
        new_line[j - 2] = '\n';
        new_line[j - 1] = 0;
    }
    return new_line;
}

char *getNewSubstringWithIndexes(char *line, int start, int end)
{
    char *tmp = (char *)malloc(strlen(line));
    int i = 0;

    for (i; i + start < end; i++)
    {
        tmp[i] = line[i + start];
    }
    tmp[i++] = '\0';
    tmp = (char *)realloc(tmp, strlen(tmp));
    return tmp;
}

char *getNewSubstringFrom(char *line, int start)
{
    return getNewSubstringWithIndexes(line, start, strlen(line));
}

char *getNewSubstringByCount(char *line, int start, int count)
{
    return getNewSubstringWithIndexes(line, start, start + count);
}