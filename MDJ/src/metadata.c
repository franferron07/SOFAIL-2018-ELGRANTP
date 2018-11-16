/*
 * metadata.c
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */

#include "metadata.h"

void cargar_configuracion_metadata(){//hardcodeada, completar con config.h y  Metadata.bin
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("Metadata.bin");
//	(&metadata)->cantidad_bloques=config_get_int_value(configuracion_cfg_temporal,"CANTIDAD_BLOQUES");
	metadata.cantidad_bloques=config_get_int_value(configuracion_cfg_temporal,"CANTIDAD_BLOQUES");
	metadata.tamanio_de_bloque=config_get_int_value(configuracion_cfg_temporal,"TAMANIO_BLOQUES");
	config_destroy(configuracion_cfg_temporal);
}
void mostrar_configuracion_metadata(){
	puts("leyendo metadata");
	printf("tamanio bloque %d \n", metadata.tamanio_de_bloque);
	printf("cantidad_bloques %d \n", metadata.cantidad_bloques);
	puts("fin lectura metadata ");
}
