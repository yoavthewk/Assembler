#include "line_parsing.h"

/**
 * @brief this function parses the line for the macro parsing
 * 
 * @param line the line to parse.
 * @return char* the parsed line.
 */
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

    if (strchr(line, '\n') == NULL)
    {
        free(line);
        return new_line;
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

/**
 * @brief this function parses the line for the first iteration (is also used in the second)
 * 
 * @param line the line to parse.
 * @param flagRegister the flag register.
 * @return char* the parsed line.
 */
char *parse_line_first_iteration(char *line, PSW *flagRegister)
{
    char *new_line;
    char last_char = ' ';
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

    if (strchr(line, '\n') == NULL)
    {
        return new_line;
    }
    else if (j <= 2)
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