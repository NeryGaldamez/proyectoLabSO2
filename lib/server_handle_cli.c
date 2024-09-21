#include <stdlib.h>
#include <stdio.h>
#include <string.h> //bzeros
#include <unistd.h> //exit
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include "server_functions.h"
#include "server_handle_cli.h"
#include "log.h"
#include "http_status_codes.h"
#include "file_handler.h"
#include <time.h>


void srv_handle_client(int connfd){
    char request[LONG_BUFFER];
    http_req req;

    HttpResponse response;


    bzero(request, sizeof(request));
    //se lee la solicitud y se guarda en request
    ssize_t bytes_recibidos = recv(connfd, request, sizeof(request), 0);

    if(bytes_recibidos == -1){
        //printf("Error en la lectura de la solicitud. Error no: %i. Nombre: %s\n", errno, strerror(errno));

        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje),"Error en la lectura de la solicitud. Error no: %i. Nombre: %s\n", errno, strerror(errno));
        log_error(mensaje);

        close(connfd);
        return;
    }

    //printf("Se recibió la petición: %s\n", request);
    //se envía la petición a la función parse_request
    req = parse_request(request);


   //Verificar si el metodo que se recibe es GET, de lo contrario mostrar el mensaje de error correspondiente
    if (strcmp(req.method, "GET") != 0) {
        response.codigo = 405; // Método no permitido
        response.content_type = "text/html";
        response.datos = "<html><body><h1>405 Method Not Allowed</h1></body></html>";

        http_response(connfd, response.codigo, response.content_type, response.datos);

        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Error 405, Method Not Allowed %s", req.method);
        log_error(mensaje);

        close(connfd);
        return;
    }

    //si el metodo sí es GET
    response.datos = get_file_contents(req.url, &response.codigo);

    //si no se encontró el archivo, retornar "404 Not Found"
    if (response.datos == NULL) {
        response.codigo = 404; // Archivo no encontrado
        response.content_type = "text/html";
        response.datos = "<html><body><h1>404 Not Found</h1></body></html>";
    } else {
        //archivo encontrado, setear el tipo de contenido
        response.content_type = "text/html";  // Esto se puede mejorar para identificar tipos de archivo (HTML, CSS, JS, etc.)
    }

    //enviar la respuesta HTTP al cliente
    http_response(connfd, response.codigo, response.content_type, response.datos);

    // Liberar la memoria del body si es necesario (si se asigna dinámicamente)
    if (response.codigo != 404 && response.codigo == 200) {
        free(response.datos);
    }


    //se obtiene la dirección IP y el puerto del cliente que hizo la solicitud
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    if (getpeername(connfd, (struct sockaddr *)&client_addr, &client_addr_len) == -1) {
        log_error("Error al obtener la dirección del cliente");
        close(connfd);
        return;
    }

    //se guarda en cliente_ip la ip del cliente que hizo la solicitud
    char cliente_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, cliente_ip, sizeof(cliente_ip));
    //se guarda en cliente_port el puerto desde el cual el cliente solicitó en el navegador
    int cliente_port = ntohs(client_addr.sin_port);

    //se registra la solicitud del cliente como un evento.
    char event_msg[256];
    snprintf(event_msg, sizeof(event_msg), "El cliente %s:%d solicita %s con el método GET", cliente_ip, cliente_port, req.url);
    log_event("Solicitud GET", event_msg);

    close(connfd);

    //return;
}


http_req parse_request(const char *request){
    http_req req;
    bzero(&req, sizeof(req));

    //se busca la primera línea de la peticón
    //porque es la que contiene la info que interesa
    //strchr lo que hará es guardar el contenido que se encuentre desde la primera coincidencia de carácter,
    //es decir, lo que esté después de la primera línea en este caso.
    const char *line_end = strchr(request, '\r'); //se busca con un retorno de carro
    if (line_end == NULL) {
        line_end = strchr(request, '\n'); //si no está guardado así, se verifica con un salto de línea
    }

    if (line_end != NULL) {
        //se resta para obtener el tamaño o cantidad de caracteres que hay entre el inicio de la petición y el primer
        //retorno de carro o salto de línea, es decir, se obtendrá solo la primera línea.
        size_t line_length = line_end - request;
        //se crea una variable para guardar la primera línea
        char line[LONG_BUFFER];
        //se copia el contenido de la primera línea del request en la variable line
        strncpy(line, request, line_length);
        line[line_length] = '\0';

        // Extraer el método y la URL, como ambas cosas son la primera y segunda palabra que se ven en la primera 
        //línea, solo se pide guardar directamente las primeras dos palabras y así se guarda el método y el url
        sscanf(line, "%s %s", req.method, req.url);
    }

    return req;
}

void http_response(int connfd, int code, const char *content_type, const char *body) {
    char headers[LONG_BUFFER];
    
    time_t now;
    struct tm *tm_info;
    char date_buffer[30]; //buffer para guardar la fecha en el formato que el header pide

    //obtener la fecha y hora actual
    time(&now);
    tm_info = gmtime(&now); //se usa gmtime para obtener la hora en formato GMT

    //se formatea la fecha en el formato RFC 1123
    strftime(date_buffer, sizeof(date_buffer), "%a, %d %b %Y %H:%M:%S GMT", tm_info);

    snprintf(headers, sizeof(headers),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Server: ServidorLabSO2\r\n"
        "Date: %s\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        code, get_http_status_message(code), content_type, strlen(body), date_buffer, body);
    
    send(connfd, headers, strlen(headers), 0);
    //send(connfd, body, strlen(body), 0);
}
