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





	/* libero memoria de inicializacion  */
	config_destroy(inicializador);
	/* libero loggger de logging */
	log_destroy(logger);
	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);


	return EXIT_SUCCESS;
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
