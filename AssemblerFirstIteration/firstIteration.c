#include "firstIteration.h"

void containsLabel(char* line)
{
    char* canBeLabel = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcspn(canBeLabel, ":") || strcspn(canBeLabel, ":") == strlen(canBeLabel))
    {
        flagRegister.SYM = 0;
        return;
    }
    flagRegister.SYM = 1;
}

void isData(char* line)
{
    char* canBeData = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcmp(canBeData, ".data") || !strcmp(canBeData, ".string"))
    {
        /* add to data table
         add to symbol table */
        return;
    }
}

void processData(char* line)
{
    char* data = strtok(line, " "); /* get the first (or only) word in the line. */
    size_t length = 0;
    if (!strcmp(data, ".data"))
    {
        while ((data = strtok(NULL, ",")))
        {
            length += sizeof(word);
        }
    }
    else
    {
        strtok(NULL, "\"");
        data = strtok(NULL, "\"");
        length += strlen(data);
    }
    DC.data += length;
}

bool isExtern(char* line)
{
    return !strcmp(strtok(line, " "), ".extern");
}

bool isEntry(char* line)
{
    return !strcmp(strtok(line, " "), ".entry");
}
