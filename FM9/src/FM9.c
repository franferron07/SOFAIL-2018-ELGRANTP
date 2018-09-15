/*
 ============================================================================
 Name        : FM9.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "FM9.h"


int main(void) {
	puts("FM9"); /* prints FM9 */
	logger = log_create("FM9.log", "FM9",false, LOG_LEVEL_INFO);
	log_info(logger, "INICIO FM9");
	//instancio el inicializador y reservo memoria
	c_inicial = malloc(sizeof(config_inicial));
	inicializador = config_create("fm9.cfg");
	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		log_error(logger, "No se encuentra archivo fm9.cfg");
		exit(EXIT_FAILURE);
	}

	//leo archivo
	leer_configuracion(inicializador, c_inicial);
	log_info(logger, "Leido archivo fm9.cfg");
	//muestro consola valor leido de archivo como prueba
	prueba_leer_archivo_cfg(c_inicial);

	//Servidor con Hilos
	log_info(logger, "Servidor con Hilos");
	Socket socket = crear_socket("127.0.0.1",c_inicial->puerto_escucha);
	//Asocio el servidor a un puerto
	asociar_puerto(socket);
	//Escucho Conexiones Entrantes
	escuchar(socket);

	/*Por cada una de las conexiones que sean aceptadas, se lanza
	un Hilo encargado de atender la conexión*/
	while(1)
	{

		int socketCliente = Acepta_Conexion_Cliente(socket.socket);
		pthread_create (&idHilo, NULL, (void*)nueva_conexion, &socketCliente);
	}




	/* libero memoria de inicializacion  */
	config_destroy(inicializador);
	/* libero loggger de logging */
	log_destroy(logger);
	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);
	return EXIT_SUCCESS;
}


void nueva_conexion (void *parametro) {
    int *sock = (int *) parametro;
    puts("Nueva conexion perrooo!!");
    log_info(logger, "Nueva conexion perrooo!!");
    //cerrar_socket(*sock);
}

void leer_configuracion(t_config *inicializador , config_inicial *c_inicial ){
	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	c_inicial->puerto_escucha = string_duplicate(config_get_string_value(inicializador, "PUERTO"));
	c_inicial->modo_ejecucion = string_duplicate(config_get_string_value(inicializador, "MODO"));
	c_inicial->tam_memoria = config_get_int_value(inicializador, "TAMANIO");
	c_inicial->tam_linea = config_get_int_value(inicializador, "MAX_LINEA");
	c_inicial->tam_pagina = config_get_int_value(inicializador, "TAM_PAGINA");
}

void prueba_leer_archivo_cfg(config_inicial* c_inicial) {
	puts("lectura de archivo correcta");
	printf("PUERTO: %s \n",c_inicial->puerto_escucha);
	printf("MODO: %s \n",c_inicial->modo_ejecucion);
	printf("TAMANIO: %d \n",c_inicial->tam_memoria);
	printf("MAX_LINEA: %d \n" ,c_inicial->tam_linea);
	printf("TAM_PAGINA: %d \n",c_inicial->tam_pagina);
}

void liberarMemoriaConfig(config_inicial* c_inicial) {
	free(c_inicial->puerto_escucha);
	free(c_inicial->modo_ejecucion);
	free(c_inicial);
}
