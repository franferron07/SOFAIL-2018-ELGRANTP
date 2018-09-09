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

int main(void) {


	t_config *inicializador;

	logger = log_create("SafaLog.log", "SAFA",false, LOG_LEVEL_INFO);



	c_inicial = malloc(sizeof(config_inicial));

	inicializador = config_create("safa.cfg");

	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
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








	//destruyo log
	log_destroy(logger);

	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);

	return EXIT_SUCCESS;
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
