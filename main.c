#include "main.h"

int main(int argc, char *argv[])
{
    char *file_name, **test_arr = NULL;
    /*char *original_line, *line;*/
    FILE *fp; /*, *newMacroFile*/
    macro_list *macro_head;
    symbol_list *symbol_head;
    command_list* command_head;

    int num_of_files = argc - 1, file_index = 1, i = 0;
    bool test[] = {false, false, false, false};
    hregister *IC = (hregister *)malloc(sizeof(hregister));
    hregister *DC = (hregister *)malloc(sizeof(hregister));
    PSW* flag_register = (PSW*)malloc(sizeof(PSW));

    /* if there are no files, we print an error and exit. */
    if (argc == 1)
    {
        printf("No files provided\n");
        return 1;
    }
    
    /* open each file and process it */
    while (num_of_files > 0)
    {
        /* initialize the variables */
        IC->data = 100;
        DC->data = 0;
        flag_register->ERR = 0;
        flag_register->SYM = 0;
        flag_register->ENC = 1;
        
        macro_head = init_node(NULL, "someMacro", "NULL");
        symbol_head = init_symbol_node(NULL, "someSymbol", 'h', 220, 10, test);
        test_arr = (char**)calloc(sizeof(char*) * LINES, sizeof(char*));
        for (i = 0; i < LINES; i++)
        {
            test_arr[i] = (char*)malloc(MAX_LEN);
            strcpy(test_arr[i], "abcdefg");
        }
        command_head = init_command_node(NULL, 4, 1, 1, test_arr);
        /* ************************************************************ */

        /* Getting the file pointer */
        file_name = argv[file_index++];
        fp = open_file(file_name, false);

        if (fp)
        {
            preassemble(file_name, fp, macro_head);
            fp = open_file(file_name, true);
            if (fp)
            {
                firstIteration(file_name, fp, symbol_head, IC, DC, flag_register, command_head);
                fclose(fp);
            }
            fp = open_file(file_name, true);
            if (fp)
            {
                second_iteration(file_name, fp, IC->data, DC->data, symbol_head, command_head, flag_register);
                fclose(fp);
            }
        }
        else
        {
            printf("%s doesn't exist\n", file_name);
        }
        num_of_files--;
        free_symbol_list(symbol_head);
        free_macro_list(macro_head);
        free_command_list(command_head);
    }
    free(IC);
    free(flag_register);
    free(DC);
    
    return 0;
}
