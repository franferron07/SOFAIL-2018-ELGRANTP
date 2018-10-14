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
#include <qcommons/socketServer.h> //Socket Servidor
#include <pthread.h> //hilos
#include <readline/readline.h> //biblioteca del readline para consola
#include<commons/collections/queue.h> //Colas
#include <qcommons/socket_viejo.h> //Socket Cliente
#include <semaphore.h> //Semaforos

typedef struct {
	char *puerto_safa;
	char *algoritmo;
	int quantum;
	int multiprogramacion;
	int retardo;
} config_inicial;


//STRUCT direcciones
typedef struct{
	char *path;
	char *direccion;
} struct_direccion ;


//STRUCT DTB
typedef struct{
	int idDtb;
	char *escriptorio;
	int pc;
	int inicializado;
	int quantum;
	struct_direccion** direcciones;
} struct_dtb ;



//STRUCT CPU
typedef struct{
	int socket;
	int ocupada;
	struct_dtb* dtb_ejecutar;
	sem_t sem_mutex_ejecucion_cpu;
}struct_cpu;



//GLOBALES

config_inicial *c_inicial = NULL;

t_log* logger= NULL;
Socket socket_servidor;

pthread_t hilo_dam;
pthread_t hilo_consola;
pthread_t hilo_plp;
pthread_t hilo_pcp;


//safa estado corrupto
int conecto_cpu = 0 ;
int conecto_dam = 0;


//ids para asignar a los dtbs
int id_total = 1;


//SEMAFOROS
sem_t sem_nuevo_mutex; //mutex en cola de nuevos
sem_t sem_nuevo_vacio; //control cola de nuevos vacia
sem_t sem_dtb_dummy_mutex;//mutex para dummy

sem_t sem_listo_mutex; //mutex para cola de listos
sem_t sem_listo_vacio;//controla cola de listos vacia

sem_t sem_listo_max;//controlar multiprogramacion

sem_t sem_cpu_mutex;//mutex para cola de cpus


//ESTADOS
t_queue *nuevos=NULL;
t_queue *listos=NULL;
t_queue *ejecucion=NULL;
t_queue *bloqueados=NULL;
t_queue *terminados=NULL;


//CPUS
t_queue *cpus=NULL; //lista de struct_cpu. Se tiene todas las cpus conectadas.




struct_dtb dtb_dummy;




//FUNCIONES

void liberarMemoriaConfig(config_inicial* c_inicial); //libera memoria de la configuracion inicial
void leer_configuracion( t_config* , config_inicial*  ); //inicializa la configuracion inicial(lectura de archivo)
void prueba_leer_archivo_cfg(config_inicial* c_inicial); //prueba leer archivo configuracion
void conexion_dam(void*) ; //hilo que maneja conexion con el dam.


//CPU
void conexion_cpu(void*); //hilo que maneja conexion con cpu
struct_cpu* crear_struct_cpu( int socket );

//CONSOLA
void consolaSafa(); /* Consola Safa */
void ejecutar_path_plp( char* );
void status_dtb(long);
void status_colas();
void finalizar_dtb(long );
void metricas( long );


//PLP
void plp(); //Hilo Planificador largo plazo


//PCP
void pcp(); //Hilo Planificador corto plazo
struct_dtb *obtener_proximo_dtb( char *algoritmo );
struct_cpu *obtener_cpu_libre();


struct_dtb crear_dtb( char* );



#endif /* SAFAHEADER_H_ */
