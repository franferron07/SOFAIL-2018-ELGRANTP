/*
 * damHeader.h
 *
 *  Created on: 7 sep. 2018
 *      Author: utnso
 */

#ifndef DAMHEADER_H_
#define DAMHEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <qcommons/socket.h> //Libreria Socket Cliente
#include <qcommons/socketServer.h> //Libreria Socket Servidor

typedef struct {
	char *puerto_dam;
	char *ip_safa;
	char *puerto_safa;
	char *ip_mdj;
	char *puerto_mdj;
	char *ip_fm9;
	char *puerto_fm9;
	int transfer_size;
} config_inicial;

//Variables Globales
config_inicial *c_inicial = NULL;

//Funciones
void liberarMemoriaConfig(config_inicial* c_inicial); //libera memoria de la configuracion inicial
void leer_configuracion( t_config* , config_inicial*  ); //inicializa la configuracion inicial(lectura de archivo)
void prueba_leer_archivo_cfg(config_inicial* c_inicial); //prueba leer archivo configuracion

#endif /* DAMHEADER_H_ */
