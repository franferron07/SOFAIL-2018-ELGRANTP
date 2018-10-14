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
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <qcommons/socket_viejo.h> //Libreria Socket Cliente
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
Socket socket_safa;
Socket socket_mdj;
Socket socket_fm9;

//Funciones
void nueva_conexion (void *parametro);
void liberarMemoriaConfig(config_inicial* c_inicial); //libera memoria de la configuracion inicial
void leer_configuracion( t_config* , config_inicial*  ); //inicializa la configuracion inicial(lectura de archivo)
void prueba_leer_archivo_cfg(config_inicial* c_inicial); //prueba leer archivo configuracion

Socket conectar_safa(config_inicial* c_inicial);
Socket conectar_mdj(config_inicial* c_inicial);
Socket conectar_fm9(config_inicial* c_inicial);

#endif /* DAMHEADER_H_ */
