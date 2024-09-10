#include <stdlib.h>
#include <stdio.h>
#include <string.h> //bzeros
#include <unistd.h> //exit
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>
#include "lib/server_functions.h"
#include "lib/log.h"
#include "lib/server_handle_cli.h"

#define PORT 8888

int main(){
    int listenfd = srv_init(PORT);
    int connfd;

    while (1){
        connfd = srv_accept_client(listenfd);
        //no se llama al server handle porque lo ejecuto dentro de la función srv_accept_client
        srv_handle_client(connfd);
        close(connfd);
    }
    
    close(listenfd);

    return 0;
}