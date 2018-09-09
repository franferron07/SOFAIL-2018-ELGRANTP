/*
 * CPU.h
 *
 *  Created on: 8 sep. 2018
 *      Author: cypher
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <qcommons/socket.h> //Libreria Socket Cliente

typedef struct {
	char *ip_safa;
	char *puerto_safa;
	char *ip_diego;
	char *puerto_diego;
	int retardo;
} config_inicial;

//Variables Globales
config_inicial *c_inicial = NULL;
int identificadorCPU;
Socket socket_safa;
Socket socket_dam;

//Funciones
void liberarMemoriaConfig(config_inicial* c_inicial); //libera memoria de la configuracion inicial
void leer_configuracion( t_config* , config_inicial*  ); //inicializa la configuracion inicial(lectura de archivo)
void prueba_leer_archivo_cfg(config_inicial* c_inicial); //prueba leer archivo configuracion

Socket conectar_safa(config_inicial* c_inicial);



#endif /* CPU_H_ */
