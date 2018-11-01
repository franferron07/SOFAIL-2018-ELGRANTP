#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h> //Commons string
#include "../config/config.h"


typedef enum {
	ESCRIPTORIO_ABRIR = 1,
	ESCRIPTORIO_CONCENTRAR = 2,
	ESCRIPTORIO_ASIGNAR = 3,
	ESCRIPTORIO_WAIT = 4,
	ESCRIPTORIO_SIGNAL = 5,
	ESCRIPTORIO_FLUSH = 6,
	ESCRIPTORIO_CLOSE = 7,
	ESCRIPTORIO_CREAR = 8,
	ESCRIPTORIO_BORRAR = 9,
	ESCRIPTORIO_COMENTARIO = 10,
} escriptorio_instruccion;


typedef struct {
	escriptorio_instruccion nombre_instruccion;
	char** parametros;
} struct_instruccion;




struct_instruccion parsear_linea(char[]);

//otras
void liberarListaDeStrings(char** parametros); //sirve para liberar la memoria de una lista de strings con todas sus boludeces



#endif /* PARSER_PARSER_H_ */
