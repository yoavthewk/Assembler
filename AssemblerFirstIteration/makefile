CFLAGS = -c -ansi -Wall -pedantic -m32 -g -o $@
EXEFLAGS = -ansi -Wall -pedantic -g -m32 -o prog
UTILITY_SRC = ../PreAssembler/line_parsing.c ../Hardware/hardware.c ../FileHandling/files.c symbol_list.c
UTILITY_HEADERS = ../PreAssembler/line_parsing.h ../Hardware/hardware.h ../FileHandling/files.h symbol_list.h
OBJ_SRC = main.o first_iteration.o pre_assembler.o utility.o
UFLAGS = -ansi -Wall -pedantic -m32 -g -o $@

prog: $(OBJ_SRC)
	gcc $(EXEFLAGS) $(OBJ_SRC)	
main.o: ../main.c ../main.h
	gcc $(CFLAGS) ../main.c
first_iteration.o: firstIteration.c
	gcc $(CFLAGS) firstIteration.c
pre_assembler.o: ../PreAssembler/macro_parsing.c
	gcc $(CFLAGS) ../PreAssembler/macro_parsing.c
utility.o: $(UTILITY_SRC)
	gcc $(UFLAGS) $(UTILITY_SRC)