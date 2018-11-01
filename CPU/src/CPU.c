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
	 **/
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


	puts("");
	/*
	dtb_struct dtb_a_enviar;
	dtb_a_enviar.id_dtb = 15;
	dtb_a_enviar.escriptorio=strdup("/direccion/putoelquele/aguanteallboys/ahreloco/gato/policia");
	dtb_a_enviar.program_counter = 7;
	dtb_a_enviar.inicializado = 1;
	dtb_a_enviar.quantum = 4;
	//dtb_a_enviar.direcciones = NULL;



	printf("id_dtb: %d\n",dtb_a_enviar.id_dtb);
	printf("longitud escriptorio: %d\n",strlen(dtb_a_enviar.escriptorio));
	printf("escriptorio: %s\n",dtb_a_enviar.escriptorio);
	printf("program_counter: %d\n",dtb_a_enviar.program_counter);
	printf("inicializado: %d\n",dtb_a_enviar.inicializado);
	printf("quantum: %d\n",dtb_a_enviar.quantum);


	void * dtb_serializado = serializar_dtb(&dtb_a_enviar);


	dtb_struct* dtb_deserializado = deserializar_dtb(dtb_serializado);

	puts("//////DTB DESERIALIZADO//////");
	printf("id_dtb: %d\n",dtb_deserializado->id_dtb);
	printf("longitud escriptorio: %d\n",strlen(dtb_deserializado->escriptorio));
	printf("escriptorio: %s\n",dtb_deserializado->escriptorio);
	printf("program_counter: %d\n",dtb_deserializado->program_counter);
	printf("inicializado: %d\n",dtb_deserializado->inicializado);
	printf("quantum: %d\n",dtb_deserializado->quantum);
	puts("///FIN DTB DESERIALIZADO/////");
	*/

	liberar_recursos(EXIT_SUCCESS);
}

void ejecutar_instruccion(struct_instruccion instruccion){
	switch (instruccion.nombre_instruccion) {
		case ESCRIPTORIO_ABRIR:
			escriptorio_abrir(instruccion.parametros);
			break;
		case ESCRIPTORIO_CONCENTRAR:
			escriptorio_concentrar(instruccion.parametros);
			break;
		case ESCRIPTORIO_ASIGNAR:
			escriptorio_asignar(instruccion.parametros);
			break;
		case ESCRIPTORIO_WAIT:
			escriptorio_wait(instruccion.parametros);
			break;
		case ESCRIPTORIO_SIGNAL:
			escriptorio_signal(instruccion.parametros);
			break;
		case ESCRIPTORIO_FLUSH:
			escriptorio_flush(instruccion.parametros);
			break;
		case ESCRIPTORIO_CLOSE:
			escriptorio_close(instruccion.parametros);
			break;
		case ESCRIPTORIO_CREAR:
			escriptorio_crear(instruccion.parametros);
			break;
		case ESCRIPTORIO_BORRAR:
			escriptorio_borrar(instruccion.parametros);
			break;
		case ESCRIPTORIO_COMENTARIO:
			escriptorio_comentario(instruccion.parametros);
			break;
		default:
			break;
	}
	liberar_instruccion(instruccion);
}

/*
 * TODO: en estas funciones va a estar la logica del anexo 1
 * */
unsigned escriptorio_abrir(char** parametros){
	char * path = parametros[0];
	if(se_encuentra_archivo_en_gdt(path)){
		printf("el path que encontre perro es %s",path);

		//INFO:SDA
		return 0;
	}
	return 0;
}

bool se_encuentra_archivo_en_gdt(path){
	return true;
}





unsigned escriptorio_concentrar(char** parametros){return 0;}
unsigned escriptorio_asignar(char** parametros){return 0;}
unsigned escriptorio_wait(char** parametros){return 0;}
unsigned escriptorio_signal(char** parametros){return 0;}
unsigned escriptorio_flush(char** parametros){return 0;}
unsigned escriptorio_close(char** parametros){return 0;}
unsigned escriptorio_crear(char** parametros){return 0;}
unsigned escriptorio_borrar(char** parametros){return 0;}
unsigned escriptorio_comentario(char** parametros){return 0;}

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
	free(name);

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
