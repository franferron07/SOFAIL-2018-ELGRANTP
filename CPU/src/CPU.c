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

//	conectarse_con_safa();
/*
	conectarse_con_diego();
	conectarse_con_fm9();
*/
	/**
	 * ####Lectura de Escriptorio
	 *
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
*/

	puts("");

	dtb_struct dtb_a_enviar;
	dtb_a_enviar.id_dtb = 15;
	dtb_a_enviar.escriptorio=strdup("/direccion/memoria/carpeta/escriptorios/");
	dtb_a_enviar.program_counter = 7;
	dtb_a_enviar.inicializado = 1;
	dtb_a_enviar.quantum = 4;
	dtb_a_enviar.direcciones = list_create();
	list_add(dtb_a_enviar.direcciones, strdup("/sistemas/algoritmos/khg74jk/"));
	list_add(dtb_a_enviar.direcciones, strdup("/sistemas/paradigmas/piu39gh/"));
	list_add(dtb_a_enviar.direcciones, strdup("/sistemas/operativos/mry58lq/"));


	printf("id_dtb: %d\n",dtb_a_enviar.id_dtb);
	printf("longitud escriptorio: %d\n",strlen(dtb_a_enviar.escriptorio));
	printf("escriptorio: %s\n",dtb_a_enviar.escriptorio);
	printf("program_counter: %d\n",dtb_a_enviar.program_counter);
	printf("inicializado: %d\n",dtb_a_enviar.inicializado);
	printf("quantum: %d\n",dtb_a_enviar.quantum);
	puts("Direcciones: ");
	list_iterate(dtb_a_enviar.direcciones, (void *)puts);



/*
 	//prueba de deserializacion de DTB
	int tamanio_buffer;
	void * dtb_serializado = serializar_dtb(&dtb_a_enviar, &tamanio_buffer);
	printf("DTB tamanio %d\n",tamanio_buffer);

	//int result = send(socket_safa, dtb_serializado, tamanio_buffer, 0);
	//printf("result:  %d",result);
	dtb_struct* dtb_deserializado = deserializar_dtb(dtb_serializado);

	exit(0);
	puts("//////DTB DESERIALIZADO//////");
	printf("id_dtb: %d\n",dtb_deserializado->id_dtb);
	printf("longitud escriptorio: %d\n",strlen(dtb_deserializado->escriptorio));
	printf("escriptorio: %s\n",dtb_deserializado->escriptorio);
	printf("program_counter: %d\n",dtb_deserializado->program_counter);
	printf("inicializado: %d\n",dtb_deserializado->inicializado);
	printf("quantum: %d\n",dtb_deserializado->quantum);
	puts("Direcciones: ");
	//list_iterate(dtb_deserializado->direcciones, (void *)puts);
	puts("///FIN DTB DESERIALIZADO/////");
*/

	//liberar_recursos(EXIT_SUCCESS);
}

void  conectarse_con_safa(){
	log_info(cpu_log, "Conectandome a SAFA.");
	obtener_socket_cliente(&socket_safa,cpu.ip_safa,cpu.puerto_safa);
	ejecutar_handshake(socket_safa,"CPU",CPU,cpu_log);
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
	(dtb_ejecutado.quantum)-=1;
	//TODO: contabilizar el quantum ejecutado
	//hacer confirmacion(send) de ejecucion de 1 unidad de quantum

	liberar_instruccion(instruccion);
}

/*
 * TODO: en estas funciones va a estar la logica del anexo 1
 * */
unsigned escriptorio_abrir(char** parametros){

	char * path = parametros[0];
	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		return 0;
	}else{
		DAM_abrir(dtb_ejecutado.id_dtb,path);
		desalojar_dtb();
		SAFA_avisar_espera_de_carga(path);
	}

	return 0;
}

void desalojar_dtb(){
	/*TODO: borrar dtb
	*indicar a SAFA que CPU esta esperando que DAM cargue en FM9 el archivo del path
	*/

}

void SAFA_avisar_espera_de_carga(char *path){
	//TODO: SAFA_avisar_espera_de_carga
}

void DAM_abrir(uint8_t id_dtb,char * path){
	//TODO: enviar solicitud a diego para traer path desde mdj
}

bool se_encuentra_archivo_en_gdt(char *path){
	bool coincide_el_path(char * direccion){
		return string_equals_ignore_case(direccion, path);
	}
	return list_find(dtb_ejecutado.direcciones, (void*)coincide_el_path);
}



unsigned escriptorio_concentrar(char** parametros){
	sleep(cpu.retardo);
	return 0;
}



unsigned escriptorio_asignar(char** parametros){
	char * path = parametros[0];
	char * linea = parametros[1];
	char * datos = parametros[2];

	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		FM9_actualizar(path,linea,datos);
		return 0;
	}else{
		//TODO: enviar a safa 	//20001: El archivo no se encuentra abierto.
		abortar_gdt(dtb_ejecutado.id_dtb);
	}


	return 0;
}

void FM9_actualizar(char *path,char *linea,char *datos){
	//TODO hacer un send de los datos a actualizar a FM9
	//hacer receive del resultado de la operacion desde FM9
	//20002: Fallo de segmento/memoria.
	//20003: Espacio insuficiente en FM9.
	//hacer el send a SAFA
}

void abortar_gdt(int8_t id_dtb){
	//TODO: enviar a safa el id del DTB para que
	//se aborte de forma gratuita y segura
}

unsigned escriptorio_wait(char** parametros){return 0;}
unsigned escriptorio_signal(char** parametros){return 0;}

unsigned escriptorio_flush(char** parametros){
	char * path = parametros[0];

	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		DAM_flush(path);
		SAFA_espera_flush(dtb_ejecutado.id_dtb);
		return 0;
	}else{
		//TODO: enviar a safa 	//20001: El archivo no se encuentra abierto.
		abortar_gdt(dtb_ejecutado.id_dtb);
	}
	return 0;
}

void DAM_flush(char *path){
	//TODO: DAM_flush
}

void SAFA_espera_flush(uint8_t id_gdt){
	//TODO: SAFA_espera_flush
}

unsigned escriptorio_close(char** parametros){
	char * path = parametros[0];
	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		FM9_solicitar_liberar_memoria(path);
		SAFA_borrar_referencia(path);
		return 0;
	}else{
		//TODO: enviar a safa 	//20001: El archivo no se encuentra abierto.
		abortar_gdt(dtb_ejecutado.id_dtb);
	}


	return 0;
}

void FM9_solicitar_liberar_memoria(char *path){
	//TODO: FM9_solicitar_liberar_memoria
}

void SAFA_borrar_referencia(char *path){
	//TODO: SAFA_borrar_referencia
}

unsigned escriptorio_crear(char** parametros){
	char * path = parametros[0];
	char * lineas = parametros[1];

	DAM_solicitar_crear_archivo(path,lineas);
	desalojar_dtb();

	return 0;
}

void DAM_solicitar_crear_archivo(char *path,char *lineas){
	//TODO:DAM_solicitar_crear_archivo
}

unsigned escriptorio_borrar(char** parametros){
	char * path = parametros[0];

	DAM_solicitar_borrar(path);
	desalojar_dtb();
	return 0;
}

void DAM_solicitar_borrar(char* path){
	//TODO: DAM_solicitar_borrar
}

unsigned escriptorio_comentario(char** parametros){return 0;}//no hace nada

void liberar_instruccion(struct_instruccion instruccion){
	string_iterate_lines(instruccion.parametros,(void *)free);
}


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

	print_header(CPU_NAME, cpu_log);
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
	print_footer(CPU_NAME, cpu_log);
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
