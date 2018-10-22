#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h> //Commons string
#include "../config/config.h"


typedef struct {
	char* nombre_instruccion;
	char** parametros;
} struct_instruccion;




struct_instruccion parsear_linea(char[]);

//otras
void liberarListaDeStrings(char** parametros); //sirve para liberar la memoria de una lista de strings con todas sus boludeces



#endif /* PARSER_PARSER_H_ */
