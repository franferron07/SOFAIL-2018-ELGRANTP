#ifndef SAFA_H_
#define SAFA_H_

#include <qcommons/console.h>
#include <qcommons/serialize.h>
#include <qcommons/socket.h>
#include <qcommons/protocolos.h>
#include <semaphore.h>
#include <pthread.h>

#include "config/config.h"
#include "consola/gestorGDT.h"
#include "planificacion/planificacion.h"
#include "commons/commons.h"

#define FILE_CONFIG_SAFA "safa.config"
#define SAFA "SAFA"
#define TAMANIO_CANT_CLIENTES 3

int socket_safa;

pthread_t hilo_cliente;
pthread_t hilo_consola;
pthread_t hilo_principal;
pthread_t hilo_planificacion;

int inicializar();
void escuchar_consola();
void iniciar_safa();
void crear_servidor();
void atender_conexiones();
void *administrar_servidor(void *puntero_fd);
void ejecutar_planificacion();
int generar_id_dtb();
void liberar_recursos(int tipo_salida);
void terminar_exitosamente(int valor_retornado);

pthread_t hilo_dam;
pthread_t hilo_plp;
pthread_t hilo_pcp;

int id_dtb = 0;

//safa estado corrupto
int conecto_cpu = 0;
int conecto_dam = 0;

//FUNCIONES
void conexion_dam(void*); //hilo que maneja conexion con el dam.
void conexion_cpu(void*); //hilo que maneja conexion con cpu

void plp(); //Hilo Planificador largo plazo
void pcp(); //Hilo Planificador corto plazo

#endif /* SAFA_H_ */
