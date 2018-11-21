/*
 * mdj.h
 *
 *  Created on: 9 sep. 2018
 *      Author: utnso
 */
#ifndef MDJ_H_
#define MDJ_H_
#include "config/config.h"
#include "file_system/util.h"
//#include "file_system/bitmap.h"

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

#include <readline/readline.h>
#include <readline/history.h>

#include <commons/txt.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>

#include <sys/types.h>//para posicionarme en directorios o carpetas
#include <dirent.h>

//#include "archivos/util.h"//cuantos archivos hay etc
#include <qcommons/utilitaria.h>

Socket mdj_socket;//socket mdj



fd_set descriptoresLectura;	/* Descriptores de interes para select() */
int maximo;							/* Número de descriptor más grande */
int socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
int numeroClientes;			/* Número clientes conectados */
int socketServidor;				/* Descriptor del socket servidor */

//variables globales

//--------------

//MDJ
void mostrar_configuracion_mdj();
void mdj_liberar_recursos();


//MDJ

void escuchar_mensajes_entrantes();

void  ejecutar_linea_entrante();

//bitmap
void persistirContenido(char * contenido);



bool estaLLenoElBloqueActual();///debe usarse con Bitmap.bin
bool estaOcupaco(char* path);
bool terminoDeMapearContenido();
void consola_fifa();
//bool bitmap_bloque_esta_ocupado(char* path_del_bloque);
void  persistirAlBloque(char* unBloquePath, char * contenido);







//METADATA
struct METADATA{
	int tamanio_de_bloque;//en bytes, ni dice eso el enunciado
	int cantidad_bloques;
	char* MAGIC_NUMBER;//es fifa , puede ser un charArray, charlable, no sirve , es la extension de los archivos d config escriptorio
}metadata;

void cargar_configuracion_metadata();
void mostrar_configuracion_metadata();

//METADATA


//BITMAP begin
char* bloqueActual_path=NULL;
 int bloqueActual_int=0;
char* bitmap_path_directorio;
FILE* bitmap_file;
t_bitarray* bitarray_=NULL;

void setear_bloque_ocupado_en_posicion(off_t pos);
bool testear_bloque_libre_en_posicion(int pos);
void setBloqueLleno();//agregar un 1 al bitmap.bin
void mostrar_bitarray();

void getBloqueLibre_path();
bool estaLibreElBloqueActual(FILE* bloqueActual, int tamanioDeBloque);
void configurar_bitmap(char bitmap_array[],int cantidadDeBytes);


//BITMAP end


#endif /* MDJ_H_ */
