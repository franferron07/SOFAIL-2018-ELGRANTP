/*
 * config.h
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_
#include <commons/config.h>
#include <commons/log.h>
#include <stdarg.h>
#include <commons/string.h>
#define  MAX_INPUT_BUFFER 1000
 char leyenda_temporal[MAX_INPUT_BUFFER];
 //----------------------------
 typedef struct{
 	char puerto[100];
 	int retardo;
 	char punto_de_montaje[100];
 	char  ip[100];
 }MDJ_CONFIG;
 //----------------------------
 MDJ_CONFIG mdj;

 t_log *mdj_log;
 void loggear_y_guardar_info(char * s, ...); //imprime como printf y guarda en log
 void loggear_info(char * s, ...);

 t_config* cargar_en_memoria_cfg(char*);
 void montar_configuracion(t_config* );

 void cargar_configuracion_mdj();//hardcodeada, completar con config.h


#endif /* CONFIG_CONFIG_H_ */
