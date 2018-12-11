#include "dtb.h"




void inicializar_listas_dtb() {
	dtb_nuevos = list_create();
	dtb_listos = list_create();
	dtb_ejecutando = list_create();
	dtb_bloqueados = list_create();
	dtb_terminados = list_create();
	dtbs = list_create();

	dtb_listos_mayor_prioridad= list_create();

	recursos = list_create();

}


dtb_struct crear_dtb(int id_dtb, char *path) {

	dtb_struct dtb_nuevo;
	dtb_nuevo.id_dtb = id_dtb;
	dtb_nuevo.escriptorio = strdup(path);
	dtb_nuevo.inicializado = 1;
	dtb_nuevo.quantum = safa.quantum;
	dtb_nuevo.estado = NUEVO;
	/*dtb_nuevo.sentencias_espera=0;
		dtb_nuevo.sentencias_dam=0;
		dtb_nuevo.sentencias_totales=0;*/

	//dtb_nuevo.direcciones = NULL;

	return dtb_nuevo;
}



void inicializar_dummy(dtb_struct* dtb) {

	dtb_dummy.id_dtb = dtb->id_dtb;
	dtb_dummy.escriptorio = strdup(dtb->escriptorio);
}


void reiniciar_dummy(){

	free(dtb_dummy.escriptorio);
	dtb_dummy.id_dtb = -1;
}




dtb_struct *obtener_dtb_a_ejecutar_dummy(){

	dtb_struct *dtb_libre = NULL;

	dtb_libre = list_find( dtb_nuevos , (void*)dtb_estado_nuevo_cargado );

	return dtb_libre;
}


dtb_struct *obtener_dtb_a_listos(){

	dtb_struct *dtb_libre = NULL;

	dtb_libre = list_find( dtb_nuevos , (void*)dtb_estado_nuevo_cargado);

	return dtb_libre;
}


bool dtb_estado_nuevo_cargado(dtb_struct *dtb) {

	if(dtb->estado == NUEVO || dtb->estado == CARGALISTA ) return true;
	return false;
}



void agregar_direccion_a_dtb( dtb_struct *dtb , direccion_struct *direccion ){

	list_add(dtb->direcciones , direccion);
}


void eliminar_path_dtb( dtb_struct *dtb   , char* path ){

	bool dtb_tiene_path(direccion_struct *direccion) {
		return string_equals_ignore_case( direccion->path , path  ) ;
	}

	direccion_struct *dir = list_remove_by_condition(dtb->direcciones,(void*)dtb_tiene_path);
	free(dir->direccion);
	free( dir->path );
}



void aumentar_sentencias_totales( dtb_struct *dtb , int quantum ){

	dtb->sentencias_totales = dtb->sentencias_totales + quantum;
}


void aumentar_sentencias_espera( int quantum_ejecutado ){

	void aumentar_espera(dtb_struct *dtb) {

		dtb->sentencias_espera = dtb->sentencias_espera +  quantum_ejecutado;
	}

	list_iterate( dtb_nuevos , (void*)aumentar_espera );
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

	list_destroy_and_destroy_elements(dtb_listos_mayor_prioridad, (void*) liberar_dtb);
}


