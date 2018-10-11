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


int main(void) {
	puts("CPU"); /* prints CPU */

	logger = log_create("CPU.log", "CPU",false, LOG_LEVEL_INFO);
	log_info(logger, "INICIO CPU");

	inicializar_configuracion();

	/*
	socket_dam = conectar_dam(c_inicial);
	socket_safa = conectar_safa(c_inicial);
	socket_fm9 = conectar_fm9(c_inicial);
	log_info(logger, "Realizada Conexiones dam/safa/fm9");
	*/
	while(1){
		char linea[50];
		/*ejemplos de lineas
		 * 				"abrir /equipos/Racing.txt"
						"concentrar"
						"asignar /equipos/Racing.txt 9 GustavoBou"
						"wait Conmebol"
						"signal Conmebol"
						"flush /equipos/Racing.txt"
						"close /equipos/Racing.txt"
						"crear /equipos/Racing.txt 11"
						"borrar /equipos/Racing.txt"
		*/

		printf("Ingrese la sentencia: ");
		scanf(" %[^\n]s ",linea);
		ejecutar_linea_fifa(linea);

	}



	/* libero loggger de logging */
	liberar_memoria_cpu();
	return EXIT_SUCCESS;
}



void ejecutar_linea_fifa(char linea[]){
	//printf("Lei linea: %s\n",linea);

	if( _es_cd_(linea) ){
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

bool _es_cd_(char* linea){
	return string_starts_with(linea, "abrir ");
}


void liberarListaDeStrings(char** operation) {
	string_iterate_lines(operation, (void*) free);
	free(operation);
}


Socket conectar_dam(config_inicial* c_inicial){
	Socket socket;

	socket = crear_socket(c_inicial->ip_diego , c_inicial->puerto_diego);
	conectar(socket);

	return socket;
}

Socket conectar_safa(config_inicial* c_inicial){
	Socket socket;

	int test = 0;

	socket = crear_socket(c_inicial->ip_safa , c_inicial->puerto_safa);
	conectar(socket);

	Escribe_Socket (socket.socket, (char *)&test , sizeof(int) );

	return socket;
}

Socket conectar_fm9(config_inicial* c_inicial){
	Socket socket;

	socket = crear_socket(c_inicial->ip_fm9 , c_inicial->puerto_fm9);
	conectar(socket);

	return socket;
}


void inicializar_configuracion() {
	//instancio el inicializador y reservo memoria para c_inicial
	c_inicial = malloc(sizeof(config_inicial));
	inicializador = config_create("cpu.cfg");
	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		log_error(logger, "No se encuentra archivo CPU.cfg");
		exit(EXIT_FAILURE);
	}
	//leo archivo
	cargar_configuracion(inicializador, c_inicial);
	log_info(logger, "Leido archivo cpu.cfg");
	//muestro consola valor leido de archivo como prueba
	imprimir_configuracion(c_inicial);
	/* libero memoria de inicializacion  */
	config_destroy(inicializador);
}

void cargar_configuracion(t_config *inicializador , config_inicial *c_inicial ){

	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	c_inicial->ip_safa = string_duplicate(config_get_string_value(inicializador, "IP_SAFA"));
	c_inicial->puerto_safa = string_duplicate(config_get_string_value(inicializador, "PUERTO_SAFA"));
	c_inicial->ip_diego = string_duplicate(config_get_string_value(inicializador, "IP_DIEGO"));
	c_inicial->puerto_diego = string_duplicate(config_get_string_value(inicializador, "PUERTO_DIEGO"));
	c_inicial->ip_fm9 = string_duplicate(config_get_string_value(inicializador, "IP_FM9"));
	c_inicial->puerto_fm9 = string_duplicate(config_get_string_value(inicializador, "PUERTO_FM9"));
	c_inicial->retardo= config_get_int_value(inicializador, "RETARDO");

}

void imprimir_configuracion(config_inicial* c_inicial) {
	puts("lectura de archivo correcta");
	printf("IP_SAFA: %s \n",c_inicial->ip_safa);
	printf("PUERTO_SAFA: %s \n",c_inicial->puerto_safa);
	printf("IP_DIEGO: %s \n",c_inicial->ip_diego);
	printf("PUERTO_DIEGO: %s \n" ,c_inicial->puerto_diego);
	printf("IP_FM9: %s \n",c_inicial->ip_fm9);
	printf("PUERTO_FM9: %s \n" ,c_inicial->puerto_fm9);
	printf("RETARDO %d \n", c_inicial->retardo);
}

void liberar_memoria_configuracion(config_inicial* c_inicial) {

	free(c_inicial->ip_safa);
	free(c_inicial->puerto_safa);
	free(c_inicial->ip_diego);
	free(c_inicial->puerto_diego);
	free(c_inicial->ip_fm9);
	free(c_inicial->puerto_fm9);
	free(c_inicial);
}

void liberar_memoria_cpu() {
	/* libero loggger de logging */
	log_destroy(logger);
	/* libero struct config_inicial  */
	liberar_memoria_configuracion(c_inicial);
}
