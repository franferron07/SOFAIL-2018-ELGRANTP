#ifndef STORAGE_STORAGE_H_
#define STORAGE_STORAGE_H_
#include "../config/config.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <commons/collections/list.h>

typedef struct {
	int id;
	int limite;
	int base;
	t_list* tabla_paginas;
} segmento_struct;

typedef struct {
	int id;
	int marco;
} pagina_struct;

//typedef struct {
//	PCB;
//	Pila;
//	Código;
//	Datos;
//} proceso_struct;

char** memoria;

t_list *tabla_procesos;
t_list *tabla_segmentos;
t_list *tabla_paginas;
t_list *tabla_paginas_invertidas;

void inicializar_memoria();
int obtener_primer_indice_libre(int cant_lineas);
int generar_id_segmento() ;
void crear_segmento_paginada(int memoria_requerida);
void crear_segmento(int memoria_requerida);

void iniciar_segmentacion_pura(int tamanio_memoria);
void iniciar_segmentacion_paginada(int tamanio_memoria);
void iniciar_paginas_invertidas(int tamanio_memoria);

#endif /* STORAGE_STORAGE_H_ */
