#ifndef SRC_COMMONS_COMMONS_H_
#define SRC_COMMONS_COMMONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <commons/collections/list.h>

typedef struct {
	char *path;
	char *direccion;
} direccion_struct;

typedef struct {
	int id_dtb;
	char *escriptorio;
	int pc;
	int inicializado;
	int quantum;
	direccion_struct** direcciones;
} dtb_struct;

dtb_struct dtb_dummy;

typedef struct {
	int socket;
	bool ocupada;
	dtb_struct* dtb_ejecutar;
} cpu_struct;

typedef enum {
	DTB_LISTO, DTB_NUEVO, DTB_EJECUTANDO, DTB_BLOQUEADO, DTB_TERMINADO
} dtb_status_enum;


sem_t sem_nuevo_vacio; //control cola de nuevos vacia
sem_t sem_listo_vacio; //controla cola de listos vacia
sem_t sem_listo_max; //controlar multiprogramacion

pthread_mutex_t sem_nuevo_mutex; //mutex en lista de nuevos
pthread_mutex_t sem_dtb_dummy_mutex; //mutex para dummy
pthread_mutex_t sem_listo_mutex; //mutex para lista de listos
pthread_mutex_t sem_cpu_mutex; //mutex para lista de cpus . SE PODRIA QUITAR
pthread_mutex_t mutex_consola;
pthread_mutex_t mutex_planificador;

#endif /* SRC_COMMONS_COMMONS_H_ */
