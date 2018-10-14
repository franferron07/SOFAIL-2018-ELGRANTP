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

int main(int argc, char *argv[]) {
	if (inicializar() < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();



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
		ejecutar_linea(linea);

	}


/*
	int buffer_recv;

	while(1){
		Lee_Socket(socket_safa, (char*)buffer_recv,sizeof(int));

		if(buffer_recv == 20)
		{
		    log_info(logger, "estoy recibiendo quantum");
		    Lee_Socket(socket_safa, (char*)buffer_recv,sizeof(int));
		    quantum = buffer_recv;

		}
		else
		{
		    log_error(logger, "No se Pudo Conectar a SAFA");
		}



	}
*/


	/* libero loggger de logging */
	liberar_memoria_cpu();
	return EXIT_SUCCESS;
}



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


void liberarListaDeStrings(char** operation) {
	string_iterate_lines(operation, (void*) free);
	free(operation);
}


Socket conectar_dam(cpu_config* c_inicial){
	Socket socket;

	socket = crear_socket((char *)c_inicial->ip_diego ,(char *)c_inicial->puerto_diego);
	conectar(socket);

	return socket;
}

Socket conectar_safa(cpu_config* c_inicial){
	Socket socket;

	int test = 0;

	socket = crear_socket((char *)c_inicial->ip_safa , (char *)c_inicial->puerto_safa);
	conectar(socket);

	Escribe_Socket (socket.socket, (char *)&test , sizeof(int) );

	return socket;
}

Socket conectar_fm9(cpu_config* c_inicial){
	Socket socket;

	socket = crear_socket((char *)c_inicial->ip_fm9 , (char *)c_inicial->puerto_fm9);
	conectar(socket);

	return socket;
}

int inicializar() {
	if (crear_log() == EXIT_FAILURE)
		terminar_exitosamente(EXIT_FAILURE);

	print_header(CPU, cpu_log);

	if (cargar_archivo_config(FILE_CONFIG_CPU) < 0) {
		return -1;
	}

	return 0;
}


void liberar_recursos(int tipo_salida) {
	print_footer(CPU, cpu_log);
	destruir_archivo_log(cpu_log);
	terminar_exitosamente(tipo_salida);
}


void liberar_memoria_cpu() {
	/* libero loggger de logging */
	log_destroy(logger);
}

void terminar_exitosamente(int ret_val) {
	exit(ret_val);
}
