#include "files.h"

int main(int argc, char* argv[]){
    int num_of_files = argc - 1, file_index = 1;
    char* file_name;
    char* line  = "";
    FILE* fp;

    /* if there are no files, we print an error and exit. */
    if(argc == 1){
        printf("No files provided\n");
        return 1;
    }
    
    /* open each file and process it */
    while(num_of_files > 0){
        /* Getting the file pointer */
        file_name = argv[file_index++];
        file_name = get_file_name(file_name);
        fp = open_file(file_name);

        if(fp){
            while((line = get_next_line(fp)) != NULL) { 
                puts(line);  
                free(line);
            }
        }
        else{
            printf("%s doesn't exist\n", file_name);
        }
        num_of_files--;
    }

    return 0;
}

FILE* open_file(char* filename){
    FILE* fp = fopen(filename, "r");

    if(!fp){
        return NULL;
    }
    return fp;
}

char* get_next_line(FILE* fp){
    char* input = (char*)malloc(MAX_LEN);
    if(fgets(input, MAX_LEN, fp)){
        return input;
    }
    return NULL;
}

FILE* open_file_create(char* filename){
    filename = (char*)realloc(filename, strlen(filename) + strlen(".am") + 1);
    get_file_name_pre(filename);
    FILE* fp = fopen(filename, "a");

    if(!fp){
        return NULL;
    }
    return fp;
}

void write_line(FILE* fp, char *input){
    fprintf(fp, "%s", input);
    return NULL;
}


