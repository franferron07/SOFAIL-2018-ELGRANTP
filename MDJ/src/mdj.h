/*
 * mdj.h
 *
 *  Created on: 9 sep. 2018
 *      Author: utnso
 */
#ifndef MDJ_H_
#define MDJ_H_
#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <qcommons/socketServer.h> //Libreria Socket Servidor

#include <commons/log.h>
#include <stdbool.h>
#include <unistd.h>


#include <stdarg.h>
#include <pthread.h>
#include <qcommons/socket_viejo.h> //Libreria Socket Cliente

//----------------------------
typedef struct{
	char* puerto;
	int retardo;
	char* punto_de_montaje;
	char * ip;
}MDJ;
//----------------------------


t_log *logger= NULL;
fd_set descriptoresLectura;	/* Descriptores de interes para select() */
int maximo;							/* Número de descriptor más grande */
int socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
int numeroClientes;			/* Número clientes conectados */
int socketServidor;				/* Descriptor del socket servidor */

Socket mdj_socket; //socket MDJ

//variables globales
MDJ* mdj=NULL;

Socket mdj_socket;//socket mdj

//funciones
void liberar_memoria_de_configuracion(MDJ* configuracion_inicial);
//freeshea el puntero de configuracion inicial
void montar_configuracion(t_config* ,MDJ*);
void mostrar_configuracion();
t_config* cargar_en_memoria_cfg(char*);
void config_destroy_mdj(MDJ* );
//--------------
void mdj_inicializar();


void mostrar_y_guardar_log(char * s, ...); //imprime como printf y guarda en log
void guardar_log(char * ); // solo guarda log

#endif /* MDJ_H_ */
