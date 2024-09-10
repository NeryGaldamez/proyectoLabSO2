#ifndef SERVER_HANDLE_CLI_H
#define SERVER_HANDLE_CLI_H

#define LONG_BUFFER 9048 //2083

typedef struct {
    char method[8];  
    char url[128]; 
} http_req;

void srv_handle_client(int connfd);
http_req parse_request(const char *request);

#endif