/*
 * damHeader.h
 *
 *  Created on: 7 sep. 2018
 *      Author: utnso
 */

#ifndef SAFAHEADER_H_
#define SAFAHEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <commons/log.h> //Commons log
#include <qcommons/socket.h> //Socket Cliente
#include <qcommons/socketServer.h> //Socket Servidor
#include <pthread.h> //hilos
#include <readline/readline.h> //biblioteca del readline para consola
#include<commons/collections/queue.h> //Colas

typedef struct {
	char *puerto_safa;
	char *algoritmo;
	int quantum;
	int multiprogramacion;
	int retardo;
} config_inicial;


//Variables Globales
config_inicial *c_inicial = NULL;


//Funciones
void liberarMemoriaConfig(config_inicial* c_inicial); //libera memoria de la configuracion inicial
void leer_configuracion( t_config* , config_inicial*  ); //inicializa la configuracion inicial(lectura de archivo)
void prueba_leer_archivo_cfg(config_inicial* c_inicial); //prueba leer archivo configuracion
void conexion_dam(void*) ; //hilo que maneja conexion con el dam.
void conexion_cpu(void*); //hilo que maneja conexion con cpu
void consolaSafa(); /* Consola Safa */

#endif /* SAFAHEADER_H_ */
