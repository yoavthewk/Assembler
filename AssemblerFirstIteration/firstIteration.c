#include "firstIteration.h"
#include "../PreAssembler/line_parsing.h"
#include "symbol_list.h"

void process_line(char *line, symbolList* head){
    line = parse_line(line); /* getting the parsed command */ 
    /* returns the full line if there is no label, otherwise, it cuts the label definition off after handling it */
    contains_label(line); 
    handle_data(line, head);
    if(is_entry(line)) return;
    if(is_extern(line)){
        /* insert to symbol table */

        /* get the name first */
        char* name;
        name = strtok(line, " ");
        while(!strcmp(name, ".extern")){
            name = strtok(NULL, " ");
        }
        name = strtok(NULL, " ");

        if(!name) return; /* throw an error - no name provided */
        
        bool *att = {true, false, false};
        insertAtEnd(&head, initList(NULL, name, 0, 0, 0, att)); /* TEMP!!! */
    }
}


void contains_label(char* line)
{
    char* canBeLabel = strtok(line, " "); /* get the first (or only) word in the line. */
    if (!strcspn(canBeLabel, ":") || strcspn(canBeLabel, ":") == strlen(canBeLabel))
    {
        flagRegister.SYM = 0;
    }
    /* if there is a definition of symbol */
    flagRegister.SYM = 1;
}

void handle_data(char* line, symbolList* head)
{
    char* canBeData = strtok(line, " "); /* get the first (or only) word in the line. */
    if(flagRegister.SYM){
        /* canBeData is the name of the symbol in this case. */
        canBeData[strlen(canBeData) - 1] = 0; /* removed the : */
        /* add to symbol table */
        bool* att = {false, false, true};
        insertAtEnd(&head, initList(NULL, canBeData, BASE + OFFSET, BASE, OFFSET, att)); /* TEMP */

        canBeData = strtok(NULL, " "); /* get the next word (or the only word) */
    }

    if (!strcmp(canBeData, ".data") || !strcmp(canBeData, ".string"))
    {
        /* add to data table */
        process_data(line); 
        return;
    }
}

void process_data(char* line)
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

bool is_extern(char* line)
{
    char* cmd; 
    cmd = strtok(line, " ");
    return flagRegister.SYM ? !strcmp(strtok(NULL, " "), ".extern") : !strcmp(cmd, ".extern");
}

bool is_entry(char* line)
{
    char* cmd; 
    cmd = strtok(line, " ");
    return flagRegister.SYM ? !strcmp(strtok(NULL, " "), ".entry") : !strcmp(cmd, ".entry");
}
