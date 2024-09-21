#include <stdlib.h>
#include <stdio.h>
#include <string.h> //bzeros
#include <errno.h>
#include "http_status_codes.h"

const HttpStatus http_status_codes_arreglo[] = {
    {200, "OK"}, 
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {500, "Internal Server Error"},
};

const char *get_http_status_message(int code) {
    //se recorre por la cantidad de elementos en el arreglo, hasta encontrar el codigo adecuado
    for (int i = 0; i < (sizeof(http_status_codes_arreglo) / sizeof(HttpStatus)); i++) {
        if (http_status_codes_arreglo[i].codigo == code) {
            return http_status_codes_arreglo[i].descripcion;
        }
    }
    return "Status Code desconocido"; //si no es ninguno de los codigos conocidos
}
