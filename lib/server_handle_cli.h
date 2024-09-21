#ifndef SERVER_HANDLE_CLI_H
#define SERVER_HANDLE_CLI_H

#define LONG_BUFFER 9048 //2083

typedef struct {
    char method[8];  
    char url[128]; 
} http_req;

void srv_handle_client(int connfd);
http_req parse_request(const char *request);

//se define una nueva estructura para guardar el codigo, el tipo de contenido y el body
typedef struct {
        int codigo;
        const char *content_type;
        char *datos;
} HttpResponse;

void http_response(int connfd, int code, const char *content_type, const char *body);

#endif