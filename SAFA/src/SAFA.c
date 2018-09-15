/*
 ============================================================================
 Name        : SAFA.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "safaHeader.h"


t_log* logger= NULL;
Socket socket_servidor;

pthread_t idHilo;


int main(void) {

    int buffer;


	logger = log_create("SafaLog.log", "SAFA",false, LOG_LEVEL_INFO);
	t_config *inicializador;

	c_inicial = malloc(sizeof(config_inicial));

	inicializador = config_create("safa.cfg");

	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		log_info(logger, "No se encuetra archivo de configuracion inicial %s", "INFO");
		exit(EXIT_FAILURE);
	}

	//leo archivo
	leer_configuracion(inicializador, c_inicial);
	log_info(logger, "Leo archivo %s", "INFO");

	//muestro consola valor leido de archivo como prueba
	prueba_leer_archivo_cfg(c_inicial);
	log_info(logger, "muestro valor por consola %s", "INFO");

	/* libero memoria de inicializacion  */
	config_destroy(inicializador);




	/* crear socket  INADDR_ANY */
	Socket socket_servidor = crear_socket(  "127.0.0.1" , c_inicial->puerto_safa);
	log_info(logger, "Creo socket %s", "INFO");
	puts("creo socket");
	//Asocio el servidor a un puerto
	asociar_puerto(socket_servidor);

	//Escucho Conexiones Entrantes
	escuchar(socket_servidor);
	puts("escucho socket");

	/*Por cada una de las conexiones que sean aceptadas, se lanza
		un Hilo encargado de atender la conexión*/
	while(1)
	{
		int socketCliente = Acepta_Conexion_Cliente(socket_servidor.socket);

		//Leo un Mensaje del Servidor
		if( Lee_Socket(socketCliente, (char *)&buffer, sizeof(int)) == -1 ) {
			puts("Error de lectura");
			exit(EXIT_FAILURE);
		}

		printf("se recibio el id: %d\n",buffer);

		//pthread_create (&idHilo, NULL, (void*)nueva_conexion, &socketCliente);
	}


	cerrar_socket(socket_servidor);

	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);
	log_info(logger, "Libero memoria de configuracion inicial %s", "INFO");

	//rompo logger
	log_destroy(logger);

	return EXIT_SUCCESS;
}



	void nueva_conexion (void *parametro) {
		int *sock = (int *) parametro;
		log_info(logger, "Conectado CPU");
		//cerrar_socket(*sock);
	}


	void leer_configuracion(t_config *inicializador , config_inicial *c_inicial ){

		//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
		c_inicial->puerto_safa = string_duplicate(config_get_string_value(inicializador, "PUERTO"));
		c_inicial->algoritmo = string_duplicate(config_get_string_value(inicializador, "ALGORITMO"));
		c_inicial->quantum= config_get_int_value(inicializador, "QUANTUM");
		c_inicial->multiprogramacion= config_get_int_value(inicializador, "MULTIPROGRAMACION");
		c_inicial->retardo= config_get_int_value(inicializador, "RETARDO_PLANIF");

	}

	void prueba_leer_archivo_cfg(config_inicial* c_inicial) {
		puts("lectura de archivo correcta");
		puts(c_inicial->puerto_safa);
		puts(c_inicial->algoritmo);
		printf("%d\n", c_inicial->quantum);
		printf("%d\n", c_inicial->multiprogramacion);
		printf("%d\n", c_inicial->retardo);
	}

	void liberarMemoriaConfig(config_inicial* c_inicial) {

		free(c_inicial->puerto_safa);
		free(c_inicial->algoritmo);
		free(c_inicial);
	}
