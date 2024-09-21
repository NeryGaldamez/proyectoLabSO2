#ifndef HTTP_STATUS_CODES_H
#define HTTP_STATUS_CODES_H

//estructura que contiene el codigo y la descripcion
typedef struct {
    int codigo;
    const char *descripcion;
} HttpStatus;

//array que almacenará los codigos y su respectiva descripcion
extern const HttpStatus http_status_codes_arreglo[];

//funcion que devolverá la descripcion a partir del codigo
const char *get_http_status_message(int code);


#endif