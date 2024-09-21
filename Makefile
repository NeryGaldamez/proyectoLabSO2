dependencias = lib/server_functions.c lib/log.c lib/server_handle_cli.c lib/file_handler.c lib/http_status_codes.c

all: main.o 

%.o: %.c $(dependencias)
	gcc -o $@ $^

clean:
	rm -f *.o
