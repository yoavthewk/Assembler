all: firstIteration.o symbolList.o
CFLAGS = -c -ansi -Wall -pedantic -m32

firstIteration.o: firstIteration.c firstIteration.h
	gcc $(CFLAGS) firstIteration.c
	
symbolList.o: symbol_list.c symbol_list.h
	gcc $(CFLAGS) symbol_list.c

