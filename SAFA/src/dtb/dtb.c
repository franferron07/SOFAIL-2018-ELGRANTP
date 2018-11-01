#include "dtb.h"




void inicializar_listas_dtb() {
	dtb_nuevos = list_create();
	dtb_listos = list_create();
	dtb_ejecutando = list_create();
	dtb_bloqueados = list_create();
	dtb_terminados = list_create();
	dtbs = list_create();
}


dtb_struct crear_dtb(int id_dtb, char *path) {

	dtb_struct dtb_nuevo;
	dtb_nuevo.id_dtb = id_dtb;
	strcpy(dtb_nuevo.escriptorio, path);
	dtb_nuevo.inicializado = 1;
	dtb_nuevo.quantum = safa.quantum;
	dtb_nuevo.sentencias_espera=0;
	dtb_nuevo.sentencias_dam=0;
	dtb_nuevo.sentencias_totales=0;

	//dtb_nuevo.direcciones = NULL;

	return dtb_nuevo;
}

dtb_struct *obtener_proximo_dtb(int algoritmo) {
	return NULL;
}

void liberar_dtb(dtb_struct * dtb) {
	free(dtb);
}

void liberar_recursos_dtb() {
	list_destroy_and_destroy_elements(dtb_nuevos, (void*) liberar_dtb);
	list_destroy_and_destroy_elements(dtb_listos, (void*) liberar_dtb);
	list_destroy_and_destroy_elements(dtb_ejecutando, (void*) liberar_dtb);
	list_destroy_and_destroy_elements(dtb_bloqueados, (void*) liberar_dtb);
	list_destroy_and_destroy_elements(dtb_terminados, (void*) liberar_dtb);
	list_destroy_and_destroy_elements(dtbs, (void*) liberar_dtb);

}


