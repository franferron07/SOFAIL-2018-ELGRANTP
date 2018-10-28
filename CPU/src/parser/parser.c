#include "parser.h"

static bool _esAbrirArchivo(char* linea);
static bool _esConcentrar(char* linea);
static bool _esAsignarLinea(char* linea);
static bool _esWait(char* linea);
static bool _esSignal(char* linea);
static bool _esFlush(char* linea);
static bool _esCloseArchivo(char* linea);
static bool _esCrearArchivo(char* linea);
static bool _esBorrarArchivo(char* linea);
static bool _esComentario(char* linea);



struct_instruccion parsear_linea(char linea[]){
	struct_instruccion instruccion;

	if( _esAbrirArchivo(linea) ){
		strcpy(instruccion.nombre_instruccion,"escriptorio_abrir");
		instruccion.parametros = string_split(linea + strlen("abrir "), " ");
		log_info(cpu_log, "Es abrir archivo, Path: %s\n",*(instruccion.parametros));

	}else if(_esConcentrar(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_concentrar");
		instruccion.parametros = string_split(linea + strlen("concentrar"), " ");;
		printf("Instruccion Concentrar.\n\n");//tengo que poner 2 "\n" para tener el salto de linea bien(ni idea por que, error de la matrix?)

	}else if(_esAsignarLinea(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_asignar");
		instruccion.parametros = string_split(linea + strlen("asignar "), " ");
		printf("Es asignar linea, Path: %s, Linea: %s, Datos: %s\n",*(instruccion.parametros),
																	*(instruccion.parametros + 1),
																	*(instruccion.parametros + 2));

	}else if(_esWait(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_wait");
		instruccion.parametros = string_split(linea + strlen("wait "), " ");
		printf("Es operacion wait, Recurso: %s\n", *(instruccion.parametros));

	}else if(_esSignal(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_signal");
		instruccion.parametros = string_split(linea + strlen("signal "), " ");
		printf("Es operacion signal, Recurso: %s\n", *(instruccion.parametros));

	}else if(_esFlush(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_flush");
		instruccion.parametros = string_split(linea + strlen("flush "), " ");
		printf("Es flush archivo, Path: %s\n", *(instruccion.parametros));

	}else if(_esCloseArchivo(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_close");
		instruccion.parametros = string_split(linea + strlen("close "), " ");
		printf("Es close archivo, Path: %s\n",*(instruccion.parametros));

	}else if(_esCrearArchivo(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_crear");
		instruccion.parametros = string_split(linea + strlen("crear "), " ");
		printf("Es crear archivo, Path: %s, Lineas: %s\n",	*(instruccion.parametros),
															*(instruccion.parametros + 1));

	}else if(_esBorrarArchivo(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_borrar");
		instruccion.parametros = string_split(linea + strlen("borrar "), " ");
		printf("Es borrar archivo, Path: %s\n",	*(instruccion.parametros));

	}else if(_esComentario(linea)){
		strcpy(instruccion.nombre_instruccion,"escriptorio_comentario");
		*instruccion.parametros=NULL;
		printf("Comentario Encontrado: %s\n",linea);

	}
	return instruccion;
}


static bool _esBorrarArchivo(char* linea){
	return string_starts_with(linea, "borrar ");
}

static bool _esCrearArchivo(char* linea){
	return string_starts_with(linea, "crear ");
}

static bool _esCloseArchivo(char* linea){
	return string_starts_with(linea, "close ");
}

static bool _esFlush(char* linea){
	return string_starts_with(linea, "flush ");
}

static bool _esSignal(char* linea){
	return string_starts_with(linea, "signal ");
}

static bool _esWait(char* linea){
	return string_starts_with(linea, "wait ");
}

static bool _esAsignarLinea(char* linea){
	return string_starts_with(linea, "asignar ");
}

static bool _esConcentrar(char* linea){
	return string_starts_with(linea, "concentrar");
}

static bool _esAbrirArchivo(char* linea){
	return string_starts_with(linea, "abrir ");
}

static bool _esComentario(char* linea){
	return string_starts_with(linea, "#");
}


void liberarListaDeStrings(char** operation) {
	string_iterate_lines(operation, (void*) free);
	free(operation);
}

/*
void printCharArray(char *arr[]){
    for (int i=0; arr[i] != NULL; i++) {
        printf("Array item: [%s]",arr[i]);
    }
}
*/
