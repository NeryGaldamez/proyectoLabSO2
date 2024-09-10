dependencias = lib/server_functions.c lib/log.c lib/server_handle_cli.c

all: main.o 

%.o: %.c $(dependencias)
	gcc -o $@ $^

clean:
	rm -f *.o
