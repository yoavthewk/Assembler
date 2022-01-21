prog: hardware.o command.o
	gcc -ansi -Wall -pedantic -m32 command_parse.c execute_command.c hardware.c hardware.h exec.h -o prog
hardware.o: hardware.c hardware.h
	gcc -c -ansi -Wall -pedantic -m32 hardware.c 
command.o: command_parse.c exec.h
	gcc -c -ansi -Wall -pedantic -m32 command_parse.c
exec.o: execute_command.c exec.h
	gcc -c -ansi -Wall -pedantic -m32 execute_command.c