dependencias = lib/server_functions.c lib/log.c

all: main.o 

%.o: %.c $(dependencias)
	gcc -o $@ $^

clean:
	rm -f *.o
