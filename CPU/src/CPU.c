/*
 ============================================================================
 Name        : CPU.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Proceso CPU
 ============================================================================
 */

#include "CPU.h"

void signal_catch(int signal);
int validar_parametros_consola(int cant_parametros);


int main(int argc, char *argv[]) {
	/*
	if (validar_parametros_consola(argc) != 0) {
		return EXIT_FAILURE;
	}*/

	if (inicializar("cpu_log") < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}




	imprimir_config();
/*
	conectarse_con_safa();
	conectarse_con_diego();
	conectarse_con_fm9();
*/
/*
	while(1){
		char linea[50];
		//				ejemplos de lineas:
		// 				abrir /equipos/Racing.txt
		//				concentrar
		//				asignar /equipos/Racing.txt 9 GustavoBou
		//				wait Conmebol
		//				signal Conmebol
		//				flush /equipos/Racing.txt
		//				close /equipos/Racing.txt
		//				crear /equipos/Racing.txt 11
		//				borrar /equipos/Racing.txt


		printf("Ingrese la sentencia: ");
		scanf(" %[^\n]s ",linea);
		ejecutar_linea(linea);
	}
*/

	/**
	 * ####Lectura de Escriptorio
	 * */
/*
	FILE * archivo;
	char * linea = NULL;
	size_t longitud = 0;
	ssize_t leido;
	archivo = fopen(argv[2], "r");
	if (archivo == NULL) {
		log_error(cpu_log, "No se puedo abrir el Escriptorio.");
		liberar_recursos(EXIT_FAILURE);
	}
	while ((leido = getline(&linea, &longitud, archivo)) != -1) {
		log_info(cpu_log, "%s", linea);
		ejecutar_linea(linea);
	}

	fclose(archivo);
	if (linea)
		free(linea);
	/**################*/



	liberar_recursos(EXIT_SUCCESS);
}


/***********************************************/
/***************LOGICA PARSER*******************/
/***********************************************/

void ejecutar_linea(char linea[]){

	if( _esAbrirArchivo(linea) ){
		puts("Es abrir archivo");
		char * path = (char *) malloc(30);
		strcpy(path,linea + strlen("abrir "));
		printf("Path encontrado: %s\n",path);


		free(path);
	}else if(_esConcentrar(linea)){
		printf("Instruccion Concentrar.\n");

	}else if(_esAsignarLinea(linea)){
		//"asignar /equipos/Racing.txt 9 GustavoBou"
		printf("Es asignar linea.\n");
		char **operation = string_split(linea + strlen("asignar "), " ");
		string_iterate_lines(operation, (void*)puts);


		liberarListaDeStrings(operation);
	}else if(_esWait(linea)){
		printf("Es operacion Wait.\n");
		char * recurso = (char *) malloc(30);
		strcpy(recurso,linea + strlen("wait "));
		printf("Recurso a la espera: %s\n",recurso);


		free(recurso);
	}else if(_esSignal(linea)){
		printf("Es operacion Signal.\n");
		char * recurso = (char *) malloc(30);
		strcpy(recurso,linea + strlen("signal "));
		printf("Recurso a liberar: %s\n",recurso);

		free(recurso);
	}else if(_esFlush(linea)){
		//Recibe como parámetro el path del archivo a guardar en MDJ.
		puts("Es Flush archivo");
		char * path = (char *) malloc(30);
		strcpy(path,linea + strlen("flush "));
		printf("Archivo a Flushear: %s\n",path);


		free(path);
	}else if(_esClose(linea)){
		puts("Es Cerrar archivo");
		char * path = (char *) malloc(30);
		strcpy(path,linea + strlen("flush "));
		printf("Archivo a cerrar: %s\n",path);


		free(path);
	}else if(_esCrearArchivo(linea)){
		//Recibe como parámetro el path y la cantidad de líneas que tendrá el archivo.
		//crear /equipos/Racing.txt 11
		puts("Es Crear archivo");
		char **operation = string_split(linea + strlen("crear "), " ");
		string_iterate_lines(operation, (void*)puts);


		liberarListaDeStrings(operation);
	}else if(_esBorrarArchivo(linea)){
		puts("Es Borrar archivo");
		char * path = (char *) malloc(30);
		strcpy(path,linea + strlen("borrar "));
		printf("Archivo a Borrar: %s\n",path);


		free(path);
	}else if(_esComentario(linea)){
		puts("Es Comentario");
	}
}


bool _esBorrarArchivo(char* linea){
	return string_starts_with(linea, "borrar ");
}

bool _esCrearArchivo(char* linea){
	return string_starts_with(linea, "crear ");
}

bool _esClose(char* linea){
	return string_starts_with(linea, "close ");
}

bool _esFlush(char* linea){
	return string_starts_with(linea, "flush ");
}

bool _esSignal(char* linea){
	return string_starts_with(linea, "signal ");
}

bool _esWait(char* linea){
	return string_starts_with(linea, "wait ");
}

bool _esAsignarLinea(char* linea){
	return string_starts_with(linea, "asignar ");
}

bool _esConcentrar(char* linea){
	return string_starts_with(linea, "concentrar");
}

bool _esAbrirArchivo(char* linea){
	return string_starts_with(linea, "abrir ");
}

bool _esComentario(char* linea){
	return string_starts_with(linea, "#");
}


void liberarListaDeStrings(char** operation) {
	string_iterate_lines(operation, (void*) free);
	free(operation);
}

/***********************************************/
/***********FIN LOGICA PARSER*******************/
/***********************************************/

int validar_parametros_consola(int cant_parametros) {
	if (cant_parametros < 2) {
		printf("Ingrese un nombre para el archivo de log!\n");
		return -1;
	}

	if (cant_parametros < 3) {
		printf("Ingrese una ruta a un script Escriptorio!\n");
		return -1;
	}

	return 0;
}

int inicializar(char* nombre_archivo_log) {
	char* name = crear_nombre_file_log(nombre_archivo_log);
	if (create_log(name) == EXIT_FAILURE)
		exit_gracefully(EXIT_FAILURE);

	print_header(CPU, cpu_log);

	if (cargar_archivo_config(FILE_CONFIG_CPU) < 0) {
		return -1;
	}

	return 0;
}

char* crear_nombre_file_log(char* nombre) {
	char* log_file_name = malloc(sizeof(char*));
	puts("entre perro1");
	puts(nombre);
	strcpy(log_file_name, nombre);
	puts("entre perro2");
	strcat(log_file_name, EXTENSION_FILE_LOG_CPU);
	return log_file_name;
}

void liberar_recursos(int tipo_salida) {
	liberar_recursos_configuracion();
	print_footer(CPU, cpu_log);
	log_destroy(cpu_log);
	exit_gracefully(tipo_salida);
}

void signal_catch(int signal) {
	printf("\nTratando seniales, senial: %d\n", signal);
	switch (signal) {
	case SIGINT:
	case SIGKILL:
	case SIGSTOP:
	case SIGTSTP:
		liberar_recursos(EXIT_SUCCESS);
	}
}

void exit_gracefully(int ret_val) {
	close(socket_safa);
	close(socket_diego);
	close(socket_fm9);
	exit(ret_val);
}
