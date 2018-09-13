/*
 ============================================================================
 Name        : CPU.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Proceso CPU
 ============================================================================
 */

#include "CPU.h"

int main(void) {
	puts("CPU"); /* prints CPU */

	logger = log_create("CPU.log", "CPU",false, LOG_LEVEL_INFO);

	log_info(logger, "INICIO CPU");

	//instancio el inicializador y reservo memoria para c_inicial
	c_inicial = malloc(sizeof(config_inicial));
	inicializador = config_create("cpu.cfg");
	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		log_error(logger, "No se encuentra archivo CPU.cfg");
		exit(EXIT_FAILURE);
	}

	//leo archivo
	leer_configuracion(inicializador, c_inicial);
	log_info(logger, "Leido archivo cpu.cfg");
	//muestro consola valor leido de archivo como prueba
	prueba_leer_archivo_cfg(c_inicial);



	//socket_dam = conectar_dam(c_inicial);
	socket_safa = conectar_safa(c_inicial);
	//log_info(logger, "Realizada Conexiones dam/safa");


	/* libero memoria de inicializacion  */
	config_destroy(inicializador);
	/* libero loggger de logging */
	log_destroy(logger);
	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);

	return EXIT_SUCCESS;
}

Socket conectar_dam(config_inicial* c_inicial){
	Socket socket;

	socket = crear_socket(c_inicial->ip_diego , c_inicial->puerto_diego);
	conectar(socket);

	return socket;
}


Socket conectar_safa(config_inicial* c_inicial){
	Socket socket;

	socket = crear_socket(c_inicial->ip_safa , c_inicial->puerto_safa);
	conectar(socket);

	return socket;
}


void leer_configuracion(t_config *inicializador , config_inicial *c_inicial ){

	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	c_inicial->ip_safa = string_duplicate(config_get_string_value(inicializador, "IP_SAFA"));
	c_inicial->puerto_safa = string_duplicate(config_get_string_value(inicializador, "PUERTO_SAFA"));
	c_inicial->ip_diego = string_duplicate(config_get_string_value(inicializador, "IP_DIEGO"));
	c_inicial->puerto_diego = string_duplicate(config_get_string_value(inicializador, "PUERTO_DIEGO"));
	c_inicial->retardo= config_get_int_value(inicializador, "RETARDO");

}

void prueba_leer_archivo_cfg(config_inicial* c_inicial) {
	puts("lectura de archivo correcta");
	printf("IP_SAFA: %s \n",c_inicial->ip_safa);
	printf("PUERTO_SAFA: %s \n",c_inicial->puerto_safa);
	printf("IP_DIEGO: %s \n",c_inicial->ip_diego);
	printf("PUERTO_DIEGO: %s \n" ,c_inicial->puerto_diego);
	printf("RETARDO %d \n", c_inicial->retardo);
}

void liberarMemoriaConfig(config_inicial* c_inicial) {

	free(c_inicial->ip_safa);
	free(c_inicial->puerto_safa);
	free(c_inicial->ip_diego);
	free(c_inicial->puerto_diego);
	free(c_inicial);
}
