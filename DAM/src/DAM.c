/*
 ============================================================================
 Name        : DAM.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "DAM.h"

pthread_t idHilo;

int main(void) {

	if (inicializar() < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();

	//Realizar Conexiones hacia SAFA, FM9, MDJ
	realizarConexiones();

	//Servidor con Hilos
	servidorDAM();

	//Aceptar Conexiones de CPU y lanzar un Hilo por cada CPU
	aceptarConexiones();

	liberar_recursos(EXIT_SUCCESS);

	return EXIT_SUCCESS;
}

int inicializar() {
	if (crear_log() == EXIT_FAILURE)
		terminar_exitosamente(EXIT_FAILURE);

	print_header(DAM, dam_log);

	if (cargar_archivo_config(FILE_CONFIG_DAM) < 0) {
		return -1;
	}

	return 0;
}

void liberar_recursos(int tipo_salida) {
	liberar_recursos_configuracion();
	print_footer(DAM, dam_log);
	destruir_archivo_log(dam_log);
	terminar_exitosamente(tipo_salida);
}

void terminar_exitosamente(int ret_val) {
	if (socket_dam != 0)
		close(socket_dam);
	exit(ret_val);
}

void servidorDAM() {
	//Servidor con Hilos
	log_info(dam_log, "Servidor con Hilos");
	if (configurar_socket_servidor(&socket_dam, "127.0.0.1", dam.puerto_dam,
			TAMANIO_CANT_CLIENTES) < 0) {
		log_error(dam_log, "No se pudo iniciar el servidor");
		terminar_exitosamente(EXIT_FAILURE);
	}
}

void aceptarConexiones() {
	log_info(dam_log, "Esperando por conexiones entrantes...");
	/*Por cada una de las conexiones que sean aceptadas, se lanza
	 un Hilo encargado de atender la conexión*/
	while (1) {
		int socket_cliente = aceptar_conexion(socket_dam);
		log_info(dam_log, "Se agrego una nueva conexión, socket: %d",
				socket_cliente);
		if (socket_cliente <= 0) {
			log_error(dam_log, "Error al aceptar nueva conexión");
		}
		pthread_create(&idHilo, NULL, (void*) conexion_cpu, &socket_cliente);
	}
}

void realizarConexiones() {
	conectar_safa(dam);
	conectar_mdj(dam);
	conectar_fm9(dam);
	if (socket_safa <= 0 || socket_mdj <= 0 || socket_fm9 <= 0) {
		log_error(dam_log, "Error al conectarse a safa/mdj/fm9");
		terminar_exitosamente(EXIT_FAILURE);
	}
	else
	{
	log_info(dam_log, "Realizada Conexiones safa/mdj/fm9");
	}
}


void conexion_cpu (void *parametro) {
    //int *sock = (int *) parametro;
    log_info(dam_log, "Conectado CPU");
    //cerrar_socket(*sock);
}

void conectar_safa(dam_config dam){

	obtener_socket_cliente(&socket_safa,dam.ip_safa,dam.puerto_safa);
}

void conectar_mdj(dam_config dam){

	obtener_socket_cliente(&socket_mdj,dam.ip_mdj,dam.puerto_mdj);
}

void conectar_fm9(dam_config dam){
	obtener_socket_cliente(&socket_safa,dam.ip_fm9,dam.puerto_fm9);
}
