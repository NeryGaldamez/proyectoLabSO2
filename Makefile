dependencias = lib/server_functions.c

all: main.o 

%.o: %.c $(dependencias)
	gcc -o $@ $^
