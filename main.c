#include "main.h"

int main(int argc, char *argv[])
{
    char *file_name;
    /*char *original_line, *line;*/
    FILE *fp; /*, *newMacroFile*/
    MacroList *macroHead;
    SymbolList *symbolHead;
    command_list* command_head;

    int num_of_files = argc - 1, file_index = 1, line_number = 0;
    bool test[] = {false, false, false, true}; /* delete this later */
    hregister *IC = (hregister *)malloc(sizeof(hregister));
    hregister *DC = (hregister *)malloc(sizeof(hregister));
    PSW* flagRegister = (PSW*)malloc(sizeof(PSW));
    IC->data = 100;
    DC->data = 0;
    /* if there are no files, we print an error and exit. */
    if (argc == 1)
    {
        printf("No files provided\n");
        return 1;
    }
    macroHead = initNode(NULL, "someMacro", "NULL");
    symbolHead = initSymbolNode(NULL, "someSymbol", 'h', 220, 10, test);
    command_head = NULL;
    /* open each file and process it */
    while (num_of_files > 0)
    {
        /* Getting the file pointer */
        file_name = argv[file_index++];
        fp = open_file(file_name, false);

        if (fp)
        {
            preassemble(file_name, fp, macroHead);
            fp = open_file(file_name, true);
            if (fp)
            {
                firstIteration(file_name, fp, symbolHead, IC, DC, flagRegister, command_head);
            }
        }
        else
        {
            printf("%s doesn't exist\n", file_name);
        }
        num_of_files--;
    }
    free(IC);
    free(flagRegister);
    free(DC);
    freeSymbolList(symbolHead);
    freeListFromMemory(macroHead);
    free_command_list(command_head);
    return 0;
}
