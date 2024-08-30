#ifndef LOG_H
#define LOG_H

#define RUTA_LOG "./logs/"

void log_event(char *evento, char *mensaje);
void log_error(char *error);
void generarFechayHora();

#endif