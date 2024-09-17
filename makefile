cc = gcc -std=c99 -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -fsanitize=address,undefined
objects = main.o shl_cli.o shl_debug.o shl_string.o

shl : $(objects)
	$(cc) -o shl $(objects)

main.o : main.c shl_debug.h shl_types.h shl_cli.h shl_string.h
	$(cc) -c main.c
shl_cli.o : shl_cli.c shl_cli.h shl_string.h shl_types.h
	$(cc) -c shl_cli.c
shl_debug.o : shl_debug.c shl_debug.h shl_types.h
	$(cc) -c shl_debug.c
shl_string.o : shl_string.c shl_string.h
	$(cc) -c shl_string.c

clean :
	rm shl $(objects)
