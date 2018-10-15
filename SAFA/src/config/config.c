#include "config.h"

int cargar_archivo_config(char* path) {
	if (path == NULL) {
		log_error(safa_log, "No se encontró el archivo de configuración");
		return -1;
	}
	t_config *config = config_create(path);

	if (config == NULL) {
		log_error(safa_log, "Error al leer ruta del archivo de configuracion");
		return -1;
	}

	log_info(safa_log, ".:: Cargando configuracion ::.");

	safa.puerto = config_get_int_value(config, "PUERTO");
	safa.quantum = config_get_int_value(config, "QUANTUM");
	safa.algoritmo = config_get_int_value(config, "ALGORITMO");
	safa.multiprogramacion = config_get_int_value(config, "MULTIPROGRAMACION");
	safa.retardo_planif = config_get_int_value(config, "RETARDO_PLANIF");

	config_destroy(config);
	return 0;
}

int crear_log() {
	safa_log = log_create("safa.log", "El-Gran-TP-SAFA", true, LOG_LEVEL_TRACE);
	if (safa_log == NULL) {
		printf("No se pudo crear el log. Abortando ejecución\n");
		return EXIT_FAILURE;
	}

	return 0;
}

void destruir_archivo_log(t_log *safa_log){
	log_destroy(safa_log);
}

void imprimir_config() {
	log_info(safa_log, ".:: Imprimiendo configuracion ::.");
	log_info(safa_log, "PUERTO: %d", safa.puerto);
	log_info(safa_log, "ALGORITMO: %s", obtener_nombre_algoritmo(safa));
	log_info(safa_log, "QUANTUM: %d", safa.quantum);
	log_info(safa_log, "MULTIPROGRAMACION: %d", safa.multiprogramacion);
	log_info(safa_log, "RETARDO_PLANIF: %d", safa.retardo_planif);
}

char* obtener_nombre_algoritmo(safa_config safa) {
	switch (safa.algoritmo) {
	case RR:
		return RR_STRING;
	case VRR:
		return VRR_STRING;
	case PROPIO:
		return PROPIO_STRING;
	default:
		return "";
	}
}
