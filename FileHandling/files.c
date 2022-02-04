#include "files.h"
#include "../PreAssembler/line_parsing.h"

int main(int argc, char* argv[]){
    int num_of_files = argc - 1, file_index = 1;
    char* file_name;
    char* line;
    char* original_line;
    FILE* fp, *newMacroFile;
    MacroList* head;

    /* if there are no files, we print an error and exit. */
    if(argc == 1){
        printf("No files provided\n");
        return 1;
    }
    head = initNode(NULL, "someMacro", "NULL");
    /* open each file and process it */
    while(num_of_files > 0){
        /* Getting the file pointer */
        file_name = argv[file_index++];
        fp = open_file(file_name);
        
        if(fp){
            /* clear the pre assembler file */
            if(clear_file(file_name)){
                while((line = get_next_line(fp)) != NULL) { 
                    original_line = (char*)malloc(strlen(line) + 1);
                    strcpy(original_line, line);
                    line = parse_line(line);
                    if(!isMacro(head, line, fp, file_name)){
                        newMacroFile = open_file_create(file_name);
                        write_line(newMacroFile, original_line);
                        fclose(newMacroFile);
                    }
                    free(line);
                    free(original_line);
                }
                fclose(fp);
            }
        }
        else{
            printf("%s doesn't exist\n", file_name);
        }
        num_of_files--;
    }
    freeListFromMemory(head);
    return 0;
}

FILE* open_file(char* filename){
    FILE* fp;
    char* temp = (char*)malloc(strlen(filename) + strlen(".as") + 1);

    strcpy(temp, filename);
    get_file_name(temp);
    fp =  fopen(temp, "r");
    free(temp);

    return fp;
}

char* get_next_line(FILE* fp){
    char* input = (char*)calloc(MAX_LEN, sizeof(char));
    if(fgets(input, MAX_LEN, fp)){
        return input;
    }
    free(input);
    return NULL;
}

FILE* open_file_create(char* file_name){
    FILE* fp;
    char* temp = (char*)calloc(strlen(file_name) + strlen(".am") + 1, sizeof(char));   
    strcpy(temp, file_name);
    get_file_name_pre(temp);
    fp = fopen(temp, "a");
    free(temp);

    return fp;
}

int clear_file(char* file_name){
    FILE* fp;
    char* temp = (char*)calloc(strlen(file_name) + strlen(".am") + 1, sizeof(char));   
    strcpy(temp, file_name);
    get_file_name_pre(temp);
    fp = fopen(temp, "w");
    free(temp);

    if(fp){
        fclose(fp);
        return 1;
    }
    return 0;
}

void write_line(FILE* fp, char *input){
    fprintf(fp, "%s", input);
}


