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
#include <qcommons/socket.h> //Libreria Sockets
#include <qcommons/console.h>

#include "config/config.h"

//CONSTANTES
#define TAMANIO_CANT_CLIENTES 3
#define FILE_CONFIG_DAM "dam.cfg"
#define DAM "DAM"

//Variables Globales
int socket_dam;

int socket_fm9;
int socket_safa;
int socket_mdj;

//Funciones
int inicializar();

void servidorDAM();
void aceptarConexiones();
void realizarConexiones();

int conectar_safa(dam_config);
int conectar_mdj(dam_config);
int conectar_fm9(dam_config);

void conexion_cpu (void *parametro);

void liberar_recursos(int tipo_salida);
void terminar_exitosamente();


#endif /* DAMHEADER_H_ */
