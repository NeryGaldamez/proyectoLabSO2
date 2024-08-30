#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include <time.h>

char fecha_hoy[15];
char timestamp[25];

void generarFechayHora(){
    time_t t = time(NULL);
    struct tm fecha_hora = *localtime(&t);
    //printf("Fecha y hora de hoy: %d-%02d-%02d %02d:%02d:%02d\n", fecha_hora.tm_year + 1900, fecha_hora.tm_mon + 1, fecha_hora.tm_mday, fecha_hora.tm_hour, fecha_hora.tm_min, fecha_hora.tm_sec);

    // FGuardar la fecha de hoy en la variable
    snprintf(fecha_hoy, sizeof(fecha_hoy), "%d-%02d-%02d", fecha_hora.tm_year + 1900, fecha_hora.tm_mon + 1, fecha_hora.tm_mday);

    // Guardar en timestamp en la variable
    snprintf(timestamp, sizeof(timestamp), "%d-%02d-%02d %02d:%02d:%02d", 
             fecha_hora.tm_year + 1900, fecha_hora.tm_mon + 1, fecha_hora.tm_mday, 
             fecha_hora.tm_hour, fecha_hora.tm_min, fecha_hora.tm_sec);
}

void log_event(char *evento, char *mensaje){
    char nombre_archivo[256];
    char contenido[1024];

    generarFechayHora();

    //printf("%s", timestamp);

    snprintf(contenido, sizeof(contenido), "[%s] Evento/%s: %s.\n", timestamp, evento, mensaje);

    fprintf(stdout, "%s", contenido);

    snprintf(nombre_archivo, sizeof(nombre_archivo), "%s%s.log", RUTA_LOG, fecha_hoy);

    FILE *archivo = fopen(nombre_archivo, "a");
    if (archivo == NULL) {
        fprintf(stderr, "Error abriendo el archivo\n");
        return;
    }

    fwrite(contenido, 1, strlen(contenido), archivo);

    fclose(archivo);
}

void log_error(char *mensaje){
    char nombre_archivo[256];
    char contenido[1024];

    generarFechayHora();

    snprintf(contenido, sizeof(contenido), "[%s] Evento/Error: %s.\n", timestamp, mensaje);

    fprintf(stderr, "%s", contenido);

    snprintf(nombre_archivo, sizeof(nombre_archivo), "%s%s-errors.log", RUTA_LOG, fecha_hoy);

    FILE *archivo = fopen(nombre_archivo, "a");
    if (archivo == NULL) {
        fprintf(stderr, "Error abriendo el archivo\n");
        return;
    }

    fwrite(contenido, 1, strlen(contenido), archivo);

    fclose(archivo);
}

/*
int main(){
    log_event("prueba", "se hizo una prueba");
    log_error("se accedio al servidor incorrectamente");

}
*/