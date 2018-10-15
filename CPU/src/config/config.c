#include "config.h"

int cargar_archivo_config(char* path) {
	if (path == NULL) {
		log_error(cpu_log, "No se encontró el archivo de configuración");
		return -1;
	}
	t_config *config = config_create(path);

	if (config == NULL) {
		log_error(cpu_log, "Error al leer ruta del archivo de configuracion");
		return -1;
	}

	log_info(cpu_log, ".:: Cargando configuracion ::.");

	cpu.ip_safa = malloc(sizeof(char*));
	cpu.ip_diego = malloc(sizeof(char*));
	cpu.ip_fm9 = malloc(sizeof(char*));

	strcpy(cpu.ip_safa, config_get_string_value(config, "IP_SAFA"));
	cpu.puerto_safa = config_get_int_value(config, "PUERTO_SAFA");
	strcpy(cpu.ip_diego, config_get_string_value(config, "IP_DIEGO"));
	cpu.puerto_diego = config_get_int_value(config, "PUERTO_DIEGO");
	strcpy(cpu.ip_fm9, config_get_string_value(config, "IP_FM9"));
	cpu.puerto_fm9 = config_get_int_value(config, "PUERTO_FM9");
	cpu.retardo = config_get_int_value(config, "RETARDO");

	config_destroy(config);
	return 0;
}


int create_log(char* nombre) {
	cpu_log = log_create(nombre, "El-Gran-TP-CPU", true, LOG_LEVEL_TRACE);
	if (cpu_log == NULL) {
		printf("No se pudo crear el log. Abortando ejecución\n");
		return EXIT_FAILURE;
	}

	return 0;
}

void destruir_archivo_log(t_log *cpu_log){
	log_destroy(cpu_log);
}

void imprimir_config() {
	log_info(cpu_log, ".:: Imprimiendo configuracion ::.");
	log_info(cpu_log, "IP_SAFA: %s", cpu.ip_safa);
	log_info(cpu_log, "PUERTO_SAFA: %d", cpu.puerto_safa);
	log_info(cpu_log, "IP_DIEGO: %s", cpu.ip_diego);
	log_info(cpu_log, "PUERTO_DIEGO: %d", cpu.puerto_diego);
	log_info(cpu_log, "IP_FM9: %s", cpu.ip_fm9);
	log_info(cpu_log, "PUERTO_FM9: %d", cpu.puerto_fm9);
	log_info(cpu_log, "RETARDO: %d", cpu.retardo);
}

void liberar_recursos_configuracion() {
	free(cpu.ip_safa);
	free(cpu.ip_diego);
	free(cpu.ip_fm9);
}

