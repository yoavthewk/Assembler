CFLAGS = -c -ansi -Wall -pedantic -m32 -g

prog: firstIteration.o symbolList.o macro_parsing.o line_parsing.o main.o
	gcc $(CFLAGS) firstIteration.c symbol_list.c ../PreAssembler/macro_parsing.c ../PreAssembler/line_parsing.c ../MacroLinkedList/macro_list.c ../FileHandling/files.c line_parsing.h -o prog
	
main.o: ../main.c ../main.h
	gcc $(CFLAGS) main.c
	
firstIteration.o: firstIteration.c firstIteration.h
	gcc $(CFLAGS) firstIteration.c
	
symbolList.o: symbol_list.c symbol_list.h
	gcc $(CFLAGS) symbol_list.c

line_parsing.o: line_parsing.c ../FileHandling/files.c ../PreAssembler/line_parsing.h
	gcc $(CFLAGS) ../PreAssembler/line_parsing.c ../FileHandling/files.c
	
macro_parsing.o: ../PreAssembler/line_parsing.h ../PreAssembler/macro_parsing.c ../MacroLinkedList/macro_list.c ../FileHandling/files.c 
	gcc $(CFLAGS) ../PreAssembler/macro_parsing.c ../MacroLinkedList/macro_list.c ../FileHandling/files.c
