#ifndef CUSTOM_UTILITARIA_H_
#define CUSTOM_UTILITARIA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/string.h>

void _finalizar_cadena(char *cadena);

bool _esta_vacia(char* cadena);

bool _es_cadena_valida(char* cadena);

char* _obtener_iesimo_parametro(char** entrada, int i);

const char* get_file_extension(const char* filename);

const char* get_filename(const char* path);

#endif /* CUSTOM_UTILITARIA_H_ */
