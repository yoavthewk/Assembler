CFLAGS = -c -ansi -Wall -pedantic -m32 -g
EXEFLAGS = -ansi -Wall -pedantic -g -m32
OBJ_SRC = main.o first_iteration.o macro_parsing.o files.o symbol_list.o line_parsing.o execute_command.o command_parse.o macro_list.o command_list.o
CC = gcc
OUT = prog
 
prog: $(OBJ_SRC)
	$(CC) $(EXEFLAGS) $(OBJ_SRC) -o $(OUT)
    
main.o: main.c main.h
	$(CC) $(CFLAGS) main.c
    
first_iteration.o: Main/first_iteration.c Main/first_iteration.h
	$(CC) $(CFLAGS) Main/first_iteration.c
    
macro_parsing.o: Main/macro_parsing.c Main/pre_assembler.h
	$(CC) $(CFLAGS) Main/macro_parsing.c
    
macro_list.o: Utility/macro_list.c Utility/macro_list.h
	$(CC) $(CFLAGS) Utility/macro_list.c
    
line_parsing.o: Utility/line_parsing.c Main/pre_assembler.h
	$(CC) $(CFLAGS) Utility/line_parsing.c -std=c90
    
files.o: Utility/files.c Utility/files.h
	$(CC) $(CFLAGS) Utility/files.c -std=c90
 
command_parse.o: Utility/command_parse.c Utility/exec.h
	$(CC) $(CFLAGS) Utility/command_parse.c
    
execute_command.o: Utility/execute_command.c Utility/exec.h
	$(CC) $(CFLAGS) Utility/execute_command.c

symbol_list.o: Utility/symbol_list.c Utility/symbol_list.h
	$(CC) $(CFLAGS) Utility/symbol_list.c -std=c90

command_list.o: Utility/command_list.c Utility/command_list.h
	$(CC) $(CFLAGS) Utility/command_list.c -std=c90
  
clean:
	rm -f $(OBJ_SRC) $(OUT)
 
