#include "storage.h"

int id_segmento = 0;

int generar_id_segmento() {
	return id_segmento++;
}

void inicializar_memoria() {
	int cant_lineas = fm9.tamanio / fm9.max_linea;
	memoria = malloc((cant_lineas * fm9.max_linea));
	int i;
	for (i = 0; i < cant_lineas; i++) {
		memoria[i] = malloc(fm9.max_linea);
		strcpy(memoria[i], "null");
	}
}

void iniciar_segmentacion_pura(int tamanio_memoria) {
	tabla_segmentos = list_create();
}

void iniciar_segmentacion_paginada(int tamanio_memoria) {

}

void iniciar_paginas_invertidas(int tamanio_memoria) {

}

int obtener_primer_indice_libre(int cant_lineas) {
	int i;
	int total_lineas = fm9.tamanio / fm9.max_linea;
	for (i = 0; i < total_lineas; i++) {
		if (!strcmp(memoria[i], "null")) {
			int aux;
			bool cumple = true;
			//evaluo valores entre el inicio y el final (cant_lineas-1)
			for (aux = i + 1; aux < cant_lineas + i; aux++) {
				if (aux >= total_lineas) {
					return -1;
				}
				if (strcmp(memoria[aux], "null") != 0) {
					cumple = false;
					break;
				}
			}
			if (cumple) {
				return i;
			}
		}
	}
	return -1;
}

void crear_segmento(int memoria_requerida) {
	segmento_struct* segmento_nuevo = malloc(sizeof(segmento_struct));
	segmento_nuevo->id = id_segmento;
	segmento_nuevo->base = obtener_primer_indice_libre(memoria_requerida);
	segmento_nuevo->limite = memoria_requerida;
	list_add(tabla_segmentos, segmento_nuevo);
	generar_id_segmento();
}

void crear_segmento_paginada(int memoria_requerida) {
	int tamanio_necesario_paginas = 0;
	int multiplo_requerido = memoria_requerida;
	if (memoria_requerida % fm9.tam_pagina != 0) {
		multiplo_requerido = ceil(fm9.tam_pagina / fm9.max_linea);
	}
	int tamanio_total = memoria_requerida + tamanio_necesario_paginas;
	segmento_struct* segmento_nuevo = malloc(sizeof(segmento_struct));
	segmento_nuevo->id = id_segmento;
	segmento_nuevo->limite = tamanio_total - 1;
	segmento_nuevo->base = obtener_primer_indice_libre(tamanio_total);
	segmento_nuevo->tabla_paginas = list_create();
	int i;
	for (i = 0; i < multiplo_requerido; i++) {
		pagina_struct * nueva_pagina = malloc(sizeof(pagina_struct));
		nueva_pagina->id = i;
		nueva_pagina->marco = i + 1;
		list_add(segmento_nuevo->tabla_paginas, nueva_pagina);
		list_add(tabla_segmentos, segmento_nuevo);
	}
	generar_id_segmento();
}

