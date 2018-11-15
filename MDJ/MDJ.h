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

#include <string.h>


#include <readline/readline.h>
#include <readline/history.h>

#include <commons/txt.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>

#include <sys/types.h>//para posicionarme en directorios o carpetas
#include <dirent.h>

//#include "archivos/util.h"//cuantos archivos hay etc
#include <qcommons/utilitaria.h>
#include "util/util.h"
#include "file_system/bitmap.h"
#include "file_system/metadata.h"
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
Socket mdj_socket;//socket mdj




t_log *mdj_log= NULL;

fd_set descriptoresLectura;	/* Descriptores de interes para select() */
int maximo;							/* Número de descriptor más grande */
int socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
int numeroClientes;			/* Número clientes conectados */
int socketServidor;				/* Descriptor del socket servidor */

//Socket mdj_socket; //socket MDJ

//variables globales


//funciones
void liberar_memoria_de_configuracion(MDJ_CONFIG* configuracion_inicial);
//freeshea el puntero de configuracion inicial
void montar_configuracion(t_config* );
void mostrar_configuracion_mdj();
t_config* cargar_en_memoria_cfg(char*);
//--------------


void loggear_y_guardar_info(char * s, ...); //imprime como printf y guarda en log
void loggear_info(char * s, ...);


void escuchar_mensajes_entrantes();
void mdj_liberar_recursos();
//void  ejecutar_linea_entrante();
void cargar_configuracion_mdj();//hardcodeada, completar con config.h


bool terminoDeMapearContenido();
void consola_fifa();
//bool bitmap_bloque_esta_ocupado(char* path_del_bloque);
void  mapearBloque(FILE* bloque, char * contenido);//escribe contenido en bloque de manera consecutiva hasta ocupar segun metadata

char* recortarPrimerosCaracteres(char* s, int primerosCaracteres);

bool quedaContenidoParaMapear();

#endif /* MDJ_H_ */
