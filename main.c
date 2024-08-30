#include <stdlib.h>
#include <stdio.h>
#include <string.h> //bzeros
#include <unistd.h> //exit
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>
#include "lib/server_functions.h"
#include "lib/log.h"

#define PORT 13

int main(){
    int listenfd = srv_init(PORT);
    int connfd;

    while (1){
        connfd = srv_accept_client(listenfd);
    }
    
    close(listenfd);

    return 0;
}