prog: command.o
	gcc -ansi -Wall -pedantic -m32 command_parse.c execute_command.c exec.h -o prog
command.o: command_parse.c exec.h
	gcc -c -ansi -Wall -pedantic -m32 command_parse.c
exec.o: execute_command.c exec.h
	gcc -c -ansi -Wall -pedantic -m32 execute_command.c