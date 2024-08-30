#include <stdlib.h>
#include <stdio.h>
#include <string.h> //bzeros
#include <unistd.h> //exit
#include <errno.h>
#include <arpa/inet.h>
#include "server_functions.h"
#include "log.h"


int srv_init(int puerto){
     
    int listenfd;
    SAI servaddr;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        //fprintf(stderr, "Error en creación de socket.\nCodigo: %i\n", errno);
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Error en creación de socket. Codigo: %i", errno);
        log_error(mensaje);
        exit(101);
    } 

    const int enable = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        //fprintf(stderr, "setsockopt(SO_REUSEADDR) failed");
        log_error("setsockopt(SO_REUSEADDR) failed");
    }
        

    //bzero borra datos de un espacio de memoria, escribiendo n 0s
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //binds a todas las interfaces
    servaddr.sin_port =  htons(puerto);

    //establece la direccion o identificacion del socket IP:PUERTO
    if( bind(listenfd, (SA*) &servaddr, sizeof(servaddr)) == -1) {
        //printf("Error code: %d\n", errno);
        //fprintf(stderr, "Error en bind.\n");  

        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Error code: %d. Error en bind", errno);
        log_error(mensaje);
 
        exit(112);
    }

    //poner el servidor en modo oyente
    if( listen(listenfd, LISTENQ) == -1) {
        //fprintf(stderr, "Error en listen.\n");
        log_error("Error en listen");
        exit(113);
    }

    //fprintf(stdout, "Servidor inicializado en modo de escucha...\n");

    log_event("Inicializado", "Servidor inicializado en modo de escucha...");

    return listenfd;
}

int srv_accept_client(int listenfd){
    int connfd;
    SAI clientaddr;

    socklen_t clientaddrsize = sizeof(clientaddr);
    while ( (connfd = accept(listenfd, (struct sockaddr*) &clientaddr, &clientaddrsize)) > 0 ) { //si no importa saber quien es el cliente, los ultimos dos parametros pueden ser null
                
        //printf("Se obtuvo una conexión desde %s\n", inet_ntoa(clientaddr.sin_addr));

        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje),"Se obtuvo una conexión desde %s", inet_ntoa(clientaddr.sin_addr));
        log_event("Conexión", mensaje);

        close(connfd);        
    }

    if(connfd == -1 ) {
        //fprintf(stderr, "Error en accept");
        log_error("Error en accept");
        exit(114);
    }

    return connfd;
}
