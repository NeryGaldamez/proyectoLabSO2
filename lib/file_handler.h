#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

//ruta a partir de la que se buscarán los archivos solicitados
#define HTML_ROOT "./html"

char *get_file_contents(const char *url, int *http_code);

#endif