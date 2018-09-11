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

t_config *inicializador=NULL;
t_log* logger= NULL;

int main(void) {
	puts("DAM"); /* prints DAM */

	logger = log_create("Dam.log", "DAM",false, LOG_LEVEL_INFO);

	log_info(logger, "INICIO DAM");

	c_inicial = malloc(sizeof(config_inicial));

	inicializador = config_create("dam.cfg");

	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		log_error(logger, "No se encuentra archivo dam.cfg");
		exit(EXIT_FAILURE);
	}

	//leo archivo
	leer_configuracion(inicializador, c_inicial);
	log_info(logger, "Leido archivo dam.cfg");

	//muestro consola valor leido de archivo como prueba
	prueba_leer_archivo_cfg(c_inicial);

	socket_safa = conectar_safa(c_inicial);
	socket_mdj = conectar_mdj(c_inicial);
	socket_fm9 = conectar_fm9(c_inicial);

	log_info(logger, "Realizada Conexiones safa/mdj/fm9");

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
	puts(c_inicial->puerto_dam);
	puts(c_inicial->ip_safa);
	puts(c_inicial->puerto_safa);
	puts(c_inicial->ip_mdj);
	puts(c_inicial->puerto_mdj);
	puts(c_inicial->ip_fm9);
	puts(c_inicial->puerto_fm9);
	printf("%d", c_inicial->transfer_size);
}

Socket conectar_safa(config_inicial* c_inicial){
	Socket socket;

	socket = crear_socket(c_inicial->ip_safa , c_inicial->puerto_safa);
	conectar(socket);

	return socket;
}

Socket conectar_mdj(config_inicial* c_inicial){
	Socket socket;

	socket = crear_socket(c_inicial->ip_mdj , c_inicial->puerto_mdj);
	conectar(socket);

	return socket;
}

Socket conectar_fm9(config_inicial* c_inicial){
	Socket socket;

	socket = crear_socket(c_inicial->ip_fm9 , c_inicial->puerto_fm9);
	conectar(socket);

	return socket;
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
