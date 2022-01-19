target: suleyman

suleyman:main.c library.c library.h
	gcc main.c library.c -lm -o 1801042656
	./1801042656

debug:
	gcc main.c library.c -g -lm -o 1801042656
	gdb -q 1801042656

error:
	gcc main.c library.c -lm -Wall -Wextra -o 1801042656
	./1801042656	