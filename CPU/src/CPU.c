#include "CPU.h"

void signal_catch(int signal);
int validar_parametros_consola(int cant_parametros);

int main(int argc, char *argv[]) {
	if (validar_parametros_consola(argc) != 0) {
		return EXIT_FAILURE;
	}

	if (inicializar(argv[1]) < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();
/*
	conectarse_con_safa();
	conectarse_con_diego();
	conectarse_con_fm9();
*/
	/**
	 * ####Lectura de Escriptorio
	 * */
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
		//log_info(cpu_log, "%s", linea);
		instruccion = parsear_linea(linea);
		ejecutar_instruccion(instruccion);
	}

	fclose(archivo);
	if (linea)
		free(linea);
	/**################*/


	liberar_recursos(EXIT_SUCCESS);
}

void ejecutar_instruccion(struct_instruccion instruccion){
	/*TODO: recibo la instruccion y lista de parametros
	 *necesito ejecutar la instruccion tomando como referencia el nombre de la funcion y los distintos parametros
	 *funcion por parametro
	 *puedo reescribir la funcion tipo o crear _ejecutar_instruccion
	 *whatever buenas noches
	 *ejm ejecutar(*(void)operacion, argumentos);
	 */

	liberar_instruccion(instruccion);
}

/*
 * TODO: en estas funciones va a estar la logica del anexo 1
 * */
void escriptorio_abrir(char** parametros){}
void escriptorio_concentrar(char** parametros){}
void escriptorio_asignar(char** parametros){}
void escriptorio_wait(char** parametros){}
void escriptorio_signal(char** parametros){}
void escriptorio_flush(char** parametros){}
void escriptorio_close(char** parametros){}
void escriptorio_crear(char** parametros){}
void escriptorio_borrar(char** parametros){}
void escriptorio_comentario(char** parametros){}

void liberar_instruccion(struct_instruccion instruccion){}


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
	char* log_file_name = malloc(10);
	strcpy(log_file_name, nombre);
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
