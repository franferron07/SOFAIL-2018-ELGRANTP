#include "config.h"

int cargar_archivo_config(char* path) {
	if (path == NULL) {
		log_error(dam_log, "No se encontró el archivo de configuración");
		return -1;
	}
	t_config *config = config_create(path);

	if (config == NULL) {
		log_error(dam_log, "Error al leer ruta del archivo de configuracion");
		return -1;
	}

	log_info(dam_log, ".:: Cargando configuracion ::.");

	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	dam.puerto_dam = config_get_int_value(config, "PUERTO");
	dam.ip_safa = string_duplicate(config_get_string_value(config, "IP_SAFA"));
	dam.puerto_safa = config_get_int_value(config, "PUERTO_SAFA");
	dam.ip_mdj = string_duplicate(config_get_string_value(config, "IP_MDJ"));
	dam.puerto_mdj = config_get_int_value(config, "PUERTO_MDJ");
	dam.ip_fm9 = string_duplicate(config_get_string_value(config, "IP_FM9"));
	dam.puerto_fm9 = config_get_int_value(config, "PUERTO_FM9");
	dam.transfer_size= config_get_int_value(config, "TRANSFER_SIZE");

	config_destroy(config);
	return 0;
}

int crear_log() {
	dam_log = log_create("dam.log", "El-Gran-TP-DAM", true, LOG_LEVEL_TRACE);
	if (dam_log == NULL) {
		printf("No se pudo crear el log. Abortando ejecución\n");
		return EXIT_FAILURE;
	}

	return 0;
}

void destruir_archivo_log(t_log *dam_log){
	log_destroy(dam_log);
}
void imprimir_config() {
	log_info(dam_log, ".:: Imprimiendo configuracion ::.");
	log_info(dam_log, "PUERTO: %d", dam.puerto_dam);
	log_info(dam_log, "IP SAFA: %s", dam.ip_safa);
	log_info(dam_log, "PUERTO SAFA: %d", dam.puerto_safa);
	log_info(dam_log, "IP FM9: %s", dam.ip_fm9);
	log_info(dam_log, "PUERTO FM9: %d", dam.puerto_fm9);
	log_info(dam_log, "IP MDJ: %s", dam.ip_mdj);
	log_info(dam_log, "PUERTO MDJ: %d", dam.puerto_mdj);
	log_info(dam_log, "TRANSFER SIZE: %d", dam.transfer_size);
}
