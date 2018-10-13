#include "config.h"

int cargar_archivo_config(char* path) {
	if (path == NULL) {
		log_error(fm9_log, "No se encontró el archivo de configuración");
		return -1;
	}
	t_config *config = config_create(path);

	if (config == NULL) {
		log_error(fm9_log, "Error al leer ruta del archivo de configuracion");
		return -1;
	}

	log_info(fm9_log, ".:: Cargando configuracion ::.");

	fm9.puerto = config_get_int_value(config, "PUERTO");
	fm9.modo = config_get_int_value(config, "MODO");
	fm9.tamanio = config_get_int_value(config, "TAMANIO");
	fm9.max_linea = config_get_int_value(config, "MAX_LINEA");
	fm9.tam_pagina = config_get_int_value(config, "TAM_PAGINA");

	config_destroy(config);
	return 0;
}

int crear_log() {
	fm9_log = log_create("fm9.log", "El-Gran-TP-FM9", true, LOG_LEVEL_TRACE);
	if (fm9_log == NULL) {
		printf("No se pudo crear el log. Abortando ejecución\n");
		return EXIT_FAILURE;
	}

	return 0;
}

void destruir_archivo_log(t_log *fm9_log){
	log_destroy(fm9_log);
}

void imprimir_config() {
	log_info(fm9_log, ".:: Imprimiendo configuracion ::.");
	log_info(fm9_log, "PUERTO: %d", fm9.puerto);
	log_info(fm9_log, "MODO: %s", obtener_nombre_modo(fm9));
	log_info(fm9_log, "TAMANIO: %d", fm9.tamanio);
	log_info(fm9_log, "MAX_LINEA: %d", fm9.max_linea);
	log_info(fm9_log, "TAM_PAGINA: %d", fm9.tam_pagina);
}

char* obtener_nombre_modo(fm9_config fm9) {
	switch (fm9.modo) {
	case SEG:
		return SEG_STRING;
	case TPI:
		return TPI_STRING;
	case SPA:
		return SPA_STRING;
	default:
		return "";
	}
}
