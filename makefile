CFLAGS = -c -ansi -Wall -pedantic -m32 -g
EXEFLAGS = -ansi -Wall -pedantic -g -m32
OBJ_SRC = main.o firstIteration.o macro_parsing.o files.o symbol_list.o line_parsing.o execute_command.o command_parse.o macro_list.o command_list.o
CC = gcc
OUT = prog
 
prog: $(OBJ_SRC)
	$(CC) $(EXEFLAGS) $(OBJ_SRC) -o $(OUT)
    
main.o: main.c main.h
	$(CC) $(CFLAGS) main.c
    
firstIteration.o: AssemblerFirstIteration/firstIteration.c AssemblerFirstIteration/firstIteration.h
	$(CC) $(CFLAGS) AssemblerFirstIteration/firstIteration.c
    
macro_parsing.o: PreAssembler/macro_parsing.c PreAssembler/pre_assembler.h
	$(CC) $(CFLAGS) PreAssembler/macro_parsing.c
    
macro_list.o: MacroLinkedList/macro_list.c MacroLinkedList/macro_list.h
	$(CC) $(CFLAGS) MacroLinkedList/macro_list.c
    
line_parsing.o: PreAssembler/line_parsing.c PreAssembler/pre_assembler.h
	$(CC) $(CFLAGS) PreAssembler/line_parsing.c -std=c90
    
files.o: FileHandling/files.c FileHandling/files.h
	$(CC) $(CFLAGS) FileHandling/files.c -std=c90
 
command_parse.o: CommandParsing/command_parse.c CommandParsing/exec.h
	$(CC) $(CFLAGS) CommandParsing/command_parse.c
    
execute_command.o: CommandParsing/execute_command.c CommandParsing/exec.h
	$(CC) $(CFLAGS) CommandParsing/execute_command.c

symbol_list.o: AssemblerFirstIteration/symbol_list.c AssemblerFirstIteration/symbol_list.h
	$(CC) $(CFLAGS) AssemblerFirstIteration/symbol_list.c -std=c90

command_list.o: AssemblerFirstIteration/command_list.c AssemblerFirstIteration/command_list.h
	$(CC) $(CFLAGS) AssemblerFirstIteration/command_list.c -std=c90
  
clean:
	rm -f $(OBJ_SRC) $(OUT)
 
