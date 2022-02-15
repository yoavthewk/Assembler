prog: macro_parsing.o line_parsing.o
	gcc -ansi -Wall -pedantic -g -m32 macro_parsing.c line_parsing.c ../MacroLinkedList/macro_list.c ../FileHandling/files.c line_parsing.h -o prog
line_parsing.o: line_parsing.c ../FileHandling/files.c line_parsing.h
	gcc -c -ansi -Wall -g -pedantic -m32 line_parsing.c ../FileHandling/files.c line_parsing.h
macro_parsing.o: macro_parsing.c ../MacroLinkedList/macro_list.c ../FileHandling/files.c line_parsing.h
	gcc -c -ansi -Wall -g -pedantic -m32 macro_parsing.c ../MacroLinkedList/macro_list.c ../FileHandling/files.c line_parsing.h
