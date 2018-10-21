#ifndef CONSOLA_GESTORGDT_H_
#define CONSOLA_GESTORGDT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/string.h>
#include <qcommons/utilitaria.h>
#include "../config/config.h"
#include "../cpu/cpu.h"
#include "../dtb/dtb.h"


#define CONSOLA_TERMINAR -1
#define CONSOLA_CONTINUAR 0
#define CONSOLA_COMANDO_EJECUTAR 1
#define CONSOLA_COMANDO_STATUS 2
#define CONSOLA_COMANDO_FINALIZAR 3
#define CONSOLA_COMANDO_METRICAS 4
#define CONSOLA_COMANDO_DESCONOCIDO 5

#define TAMANIO_CLAVE (sizeof(tabla_referencia_comandos)/sizeof(t_comando_struct))

typedef struct {
	char *clave;
	int valor;
} t_comando_struct;

int consola_leer_comando();
int obtener_valor_por_clave(char *clave);
static const int TAMANIO_ENTRADA_STDIN = 50;


int generar_id_dtb();


/*
 * parametro: el dtb nuevo a agregar
 *
 * agregar el dtb inicializado en la lista de nuevos y en la lista de dtbs.
 * Se agrega para que el plp lo administre en algun momento
 */
void agregar_dtb_nuevo( dtb_struct dtb_nuevo );

/**
 * parametro
 * La ruta del script escriptorio que se desea ejecutar
 *
 * Permitirá el ingreso de un nuevo programa G.DT al sistema.
 * Pasada la ruta del script Escriptorio, el planificador creará su DTB asociado
 * (indicando en el mismo el flag de inicializado en 0) y colocará el proceso
 * en la cola de NEW para que el PLP lo administre cuando lo permita el grado de multiprogramación.
 * Cuando suceda esto, el PLP se comunicará con este submódulo para comenzar la ejecución dummy
 * de Iniciar Escriptorio definida en el Proceso CPU
 * */
void comando_ejecutar(char* ruta_sript);

/**
 * parametro
 * Ninguno o el ID correspondiente a un DT Block
 *
 * Se deberá mostrar el estado de cada cola, así como la información complementaria a las mismas.
 * En caso de tener un parámetro, deberá informar todos los datos almacenados en el DT Block
 * (tanto mínimos como agregados por el grupo).
 * */
void comando_status(char* id_dt_block);

/**
 * parametro
 * El ID correspondiente a un DT Block
 *
 * Obligará a un DTB a pasar a la cola de EXIT para poder destrabar la ejecución y dar
 * lugar a otro G.DT a operar sobre dicho equipo.
 * Si el G.DT se encuentra en la cola EXEC se deberá esperar a terminar la operación actual,
 * para luego moverlo a la cola EXIT.
 * */
void comando_finalizar(char* id_dt_block);

/**
 * parametro
 * Opcional: El ID correspondiente a un DT Block
 *
 * Detalla las siguientes métricas:
 * 1. Cant. de sentencias ejecutadas que esperó un DTB en la cola NEW
 * 2. Cant.de sentencias ejecutadas prom. del sistema que usaron a “El Diego”
 * 3. Cant. de sentencias ejecutadas prom. del sistema para que un DTB termine en la cola EXIT
 * 4. Porcentaje de las sentencias ejecutadas promedio que fueron a “El Diego”
 * 5. Tiempo de Respuesta promedio del Sistema
 * */
void comando_metricas(char* id_dt_block);

#endif /* CONSOLA_GESTORGDT_H_ */
