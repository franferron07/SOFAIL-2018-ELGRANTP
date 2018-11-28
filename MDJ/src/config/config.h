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
 void cargar_configuracion_mdj(const char* pathConfig);
 void mostrar_configuracion_mdj();



#endif /* CONFIG_CONFIG_H_ */
