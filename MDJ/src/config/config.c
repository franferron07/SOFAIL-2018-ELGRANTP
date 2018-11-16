/*
 * config.c
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */
#include "../config/config.h"

 void loggear_y_guardar_info(char * s, ...){
 	va_list resto;
 	va_start(resto,s);
 	vsprintf(leyenda_temporal,s, resto );
 	printf(leyenda_temporal);
 	log_info(mdj_log,leyenda_temporal);
 	va_end(resto);
 }

 void loggear_info(char * s, ...){
 	va_list resto;
 	va_start(resto,s);
 	vsprintf(leyenda_temporal,s, resto );
 //	printf(leyenda_temporal);
 	log_info(mdj_log,leyenda_temporal);
 	va_end(resto);

 }


 t_config* cargar_en_memoria_cfg(char* dir){//ok
 	t_config*aux=config_create(dir);
 	if(aux==NULL){
 		perror("No se encuentra archivo MDJ.cfg \n");
 		log_error(mdj_log, "No se encuentra archivo MDJ.cfg \n");
 	}
 	return aux;
 }
 void cargar_configuracion_mdj(){
 	mdj_log = log_create("MDJ.log", "MDJ",false, LOG_LEVEL_INFO);
 	loggear_info("INICIO MDJ");
 	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("MDJ.config");
 	puts("antes de  montar_configuracion");
 	montar_configuracion(configuracion_cfg_temporal);
 	config_destroy(configuracion_cfg_temporal);
 	puts("despues de montar config");
 }
 void montar_configuracion(t_config*  temporal){
 //	strcpy(mdj.puerto ,string_duplicate(config_get_string_value(temporal,"PUERTO")));
 	strcpy(mdj.puerto ,config_get_string_value(temporal,"PUERTO"));
 	strcpy(mdj.punto_de_montaje,config_get_string_value(temporal,"PUNTO_MONTAJE"));
 	mdj.retardo=config_get_int_value(temporal,"RETARDO");
 	strcpy(mdj.ip,config_get_string_value(temporal,"IP"));
 }
