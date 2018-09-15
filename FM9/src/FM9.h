/*
 * FM9.h
 *
 *  Created on: 15 sep. 2018
 *      Author: cypher
 */

#ifndef FM9_H_
#define FM9_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <qcommons/socket.h> //Libreria Socket Cliente
#include <qcommons/socketServer.h> //Libreria Socket Servidor

typedef struct {
	char *puerto_escucha;
	char *modo_ejecucion;
	int  tam_memoria;
	int  tam_linea;
	int  tam_pagina;
} config_inicial;

//Variables Globales
config_inicial *c_inicial = NULL;
t_config *inicializador=NULL;
t_log *logger= NULL;
pthread_t idHilo;



//Funciones
void nueva_conexion (void *parametro);
void liberarMemoriaConfig(config_inicial* c_inicial); //libera memoria de la configuracion inicial
void leer_configuracion( t_config* , config_inicial*  ); //inicializa la configuracion inicial(lectura de archivo)
void prueba_leer_archivo_cfg(config_inicial* c_inicial); //prueba leer archivo configuracion




#endif /* FM9_H_ */
