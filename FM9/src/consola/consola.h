#ifndef CONSOLA_CONSOLA_H_
#define CONSOLA_CONSOLA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/string.h>
#include <qcommons/utilitaria.h>
#include "../config/config.h"

#define CONSOLA_TERMINAR -1
#define CONSOLA_CONTINUAR 0
#define CONSOLA_COMANDO_DUMP 1
#define CONSOLA_COMANDO_DESCONOCIDO 2

#define TAMANIO_CLAVE (sizeof(tabla_referencia_comandos)/sizeof(t_comando_struct))

typedef struct {
	char *clave;
	int valor;
} t_comando_struct;

int consola_leer_comando();
int obtener_valor_por_clave(char *clave);
static const int TAMANIO_ENTRADA_STDIN = 50;

/**
 * parametro
 * Opcional - ID de DTB
 *
 * El comando Dump constará del logueo de todos los datos asociados almacenados para un DTB indicado.
 * De esta manera, en base a un ID de DTB pasado, se deberá imprimir por pantalla y en log tanto
 * la totalidad de los datos administrativos guardados como los datos en memoria
 * real que contienen estos últimos.
 * */
void comando_dump(char* id_dt_block);

#endif /* CONSOLA_CONSOLA_H_ */
