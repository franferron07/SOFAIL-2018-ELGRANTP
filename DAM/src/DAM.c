/*
 ============================================================================
 Name        : DAM.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "damHeader.h"

int main(void) {
	puts("DAM"); /* prints DAM */

	t_config *inicializador;

	c_inicial = malloc(sizeof(config_inicial));

	inicializador = config_create("dam.cfg");

	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		exit(EXIT_FAILURE);
	}

	//leo archivo
	leer_configuracion(inicializador, c_inicial);

	//muestro consola valor leido de archivo como prueba
	prueba_leer_archivo_cfg(c_inicial);

	/* libero memoria de inicializacion  */
	config_destroy(inicializador);

	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);

	return EXIT_SUCCESS;
}

void leer_configuracion(t_config *inicializador , config_inicial *c_inicial ){

	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	c_inicial->puerto_dam = string_duplicate(config_get_string_value(inicializador, "PUERTO"));
	c_inicial->ip_safa = string_duplicate(config_get_string_value(inicializador, "IP_SAFA"));
	c_inicial->puerto_safa = string_duplicate(config_get_string_value(inicializador, "PUERTO_SAFA"));
	c_inicial->ip_mdj = string_duplicate(config_get_string_value(inicializador, "IP_MDJ"));
	c_inicial->puerto_mdj = string_duplicate(config_get_string_value(inicializador, "PUERTO_MDJ"));
	c_inicial->ip_fm9 = string_duplicate(config_get_string_value(inicializador, "IP_FM9"));
	c_inicial->puerto_fm9 = string_duplicate(config_get_string_value(inicializador, "PUERTO_FM9"));
	c_inicial->transfer_size= config_get_int_value(inicializador, "TRANSFER_SIZE");


}

void prueba_leer_archivo_cfg(config_inicial* c_inicial) {
	puts("lectura de archivo correcta");
	puts(c_inicial->puerto_dam);
	puts(c_inicial->ip_safa);
	puts(c_inicial->puerto_safa);
	puts(c_inicial->ip_mdj);
	puts(c_inicial->puerto_mdj);
	puts(c_inicial->ip_fm9);
	puts(c_inicial->puerto_fm9);
	printf("%d", c_inicial->transfer_size);
}

void liberarMemoriaConfig(config_inicial* c_inicial) {

	free(c_inicial->puerto_dam);
	free(c_inicial->ip_safa);
	free(c_inicial->puerto_safa);
	free(c_inicial->ip_mdj);
	free(c_inicial->puerto_mdj);
	free(c_inicial->ip_fm9);
	free(c_inicial->puerto_fm9);
	free(c_inicial);
}
