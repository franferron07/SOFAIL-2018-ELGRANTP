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


#define  MAX_INPUT_BUFFER 1000
#define loop while(1)
 char leyenda_temporal[MAX_INPUT_BUFFER];

//----------------------------
typedef struct{
	char* puerto;
	int retardo;
	char* punto_de_montaje;
	char * ip;

}MDJ_CONFIG;
//----------------------------

MDJ_CONFIG mdj;
Socket mdj_socket;//socket mdj



struct METADATA{
	int tamanio_de_bloque;//en bytes, ni dice eso el enunciado
	int cantidad_bloques;
	char* MAGIC_NUMBER;//es fifa , puede ser un charArray, charlable
}metadata;

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
void montar_configuracion(t_config* ,MDJ_CONFIG*);
void mostrar_configuracion();
t_config* cargar_en_memoria_cfg(char*);
void config_destroy_mdj(MDJ_CONFIG* );
//--------------
void mdj_inicializar();


void loggear_y_guardar_info(char * s, ...); //imprime como printf y guarda en log
void loggear_info(char * s, ...);// solo guarda log



void escuchar_mensajes_entrantes();
void mdj_liberar_recursos();
//void  ejecutar_linea_entrante();
void cargar_configuracion_mdj();
void cargar_configuracion_mdj();//hardcodeada, completar con config.h
void mostrar_configuracion_metadata();

bool esta_lLeno(FILE* bloque);///debe usarse con Bitmap.bin
bool estaOcupaco(char* path);
bool terminoDeMapearlaLinea();
void consola_fifa();
//bool bitmap_bloque_esta_ocupado(char* path_del_bloque);
void  mapearBloque(FILE* bloque, char * contenido);
void configurar_bitmap();

#endif /* MDJ_H_ */
