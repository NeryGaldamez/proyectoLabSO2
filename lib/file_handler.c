#include <stdlib.h>
#include <stdio.h>
#include <string.h> //bzeros
#include <errno.h>
#include "file_handler.h"
#include "server_handle_cli.h"


char *get_file_contents(const char *url, int *http_code) {
    char path[LONG_BUFFER];
    //se guarda en el path la dirección del archivo buscando desde la carpeta /html
    if (strcmp(url, "/") != 0){
        //si se provee url
        snprintf(path, sizeof(path), "%s%s", HTML_ROOT, url);
    }
    else{
        //si no se provee url
        snprintf(path, sizeof(path), "%s/index.html", HTML_ROOT);
    }
    //printf("La dirección a buscar del archivo es: %s.", path);

    FILE *file = fopen(path, "r");
    //si no se encuentra el archivo de la URL
    if (file == NULL) {
        *http_code = 404; //codigo de Not Found
        return NULL; //se devuelve como nulo el contenido
    }

   // Leer el archivo en un búfer
    char buffer[1024]; //tamaño del buffer
    size_t leido; // tamaño de lo leído en la iteracion
    size_t total = 0; //tamaño total 
    char *contenido = malloc(1); //puntero que guardará el contenido
    contenido[0] = '\0'; //se inicia el contenido con un caracter nulo

    while ((leido = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        //aumento el tamaño total leido
        total += leido;
        //hago la reasignacion dinamica de memoria 
        contenido = realloc(contenido, total + 1);
        //se concatena el contenido a content
        strncat(contenido, buffer, leido);
    }

    fclose(file);
    *http_code = 200; // OK
    return contenido;
}
