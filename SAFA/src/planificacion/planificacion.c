#include "planificacion.h"

bool virtual = false;
int quantum = 0;
/**
 * 0: RR; 1: VRR; 2: PROPIO
 */
int algoritmo = 0;

void set_quantum(int p_quantum) {
	quantum = p_quantum;
}

void set_algoritmo(int p_algoritmo) {
	algoritmo = p_algoritmo;
}



void ejecutar_planificacion() {

	cpu_struct *cpu_ejecutar= NULL;
	dtb_struct *dtb_ejecutar = NULL;

	while (true) {



		cpu_ejecutar = obtener_cpu_libre();

		/************ SI TENGO CPU DISPOSNIBLE *****************/
		if( cpu_ejecutar != NULL ){

			log_info(safa_log, "Se encontro CPU para ejecutar");

			/******* SI NO HAY DTBS DISPONIBLES ME QUEDO ESPERANDO *******/
			while( list_size(dtb_listos) <= 0  ){

			}

			/******** BUSCO DTB A EJECUTAR ********/
			pthread_mutex_lock( &sem_listo_mutex );
			dtb_ejecutar = aplicar_algoritmo_planificacion();
			dtb_ejecutar->estado = EJECUTANDO;
			log_info(safa_log, "Se encontro dtb a ejecutar: %s",dtb_ejecutar->id_dtb);
			pthread_mutex_unlock( &sem_listo_mutex );

			/******** INDICO A CPU EL DTB A EJECUTAR *******/
			cpu_ejecutar->ocupada = true;
			cpu_ejecutar->dtb_ejecutar = dtb_ejecutar;
			log_info(safa_log, "Indico a cpu el dtb a ejecutar");

		}


	}

}



void ejecutar_planificacion_largo_plazo(){


	dtb_struct *dtb = NULL;

	while(1){

		/**** si hay dtb para ejecutar(en estado nuevo) *******/
		dtb = obtener_dtb_a_ejecutar_dummy();
		if( dtb != NULL ){

			log_info(safa_log, "Se toma DTB para inicializar procesos dummy: %d",dtb->id_dtb);

			/***** ESPERAMOS A QUE DUMMY ESTE DISPONIBLE PARA REALIZAR PASAJE A LISTO *******/
			while( dtb_dummy.id_dtb != -1 ){

			}
			log_info(safa_log, "El dummy esta disponible");

			inicializar_dummy(dtb);
			dtb->estado = CARGANDODUMMY;
			log_info(safa_log, "DTB dummy inicializado");


			/**************** AGREGO DUMMY A LISTOS SI MULTIPROGRAMACION LO PERMITE **************************/
			sem_wait(&sem_listo_max);
			pthread_mutex_lock(&sem_listo_mutex);

			list_add(dtb_listos, &dtb_dummy);
			log_info(safa_log, "DUMMY pasado a listos");

			pthread_mutex_unlock(&sem_listo_mutex);

		}


	}


}



dtb_struct* aplicar_algoritmo_planificacion() {
	switch (safa.algoritmo) {
	case RR:
		return aplicarRR();
		break;
	case VRR:
		return aplicarVRR();
		break;
	case PROPIO:
		return aplicarPropio();
		break;
	}

	return NULL;
}

dtb_struct* aplicarRR() {

	dtb_struct* dtbAEjecutar = (dtb_struct*) list_remove(dtb_listos, 0);
	list_add(dtb_ejecutando , dtbAEjecutar);

	return dtbAEjecutar;

}


dtb_struct* aplicarVRR() {

	dtb_struct* dtbAEjecutar = NULL;

	/* VERIFICO LISTA DE MAYOR PRIORIDAD  */
	if(list_size(dtb_listos_mayor_prioridad) <= 0){

		dtbAEjecutar = (dtb_struct*) list_remove(dtb_listos_mayor_prioridad, 0);
		list_add(dtb_ejecutando , dtbAEjecutar);
	}
	else{

		dtbAEjecutar = (dtb_struct*) list_remove(dtb_listos, 0);
		list_add(dtb_ejecutando , dtbAEjecutar);
	}

	return dtbAEjecutar;
}


dtb_struct* aplicarPropio() {

	dtb_struct* dtbAEjecutar = NULL;

	return dtbAEjecutar;

}




dtb_struct* quitar_dtb_lista_id( t_list *lista  ,int id ){

	bool dtb_tiene_id(dtb_struct *dtb) {
		return dtb->id_dtb == id;
	}

	return list_remove_by_condition(lista,(void*)dtb_tiene_id);

}


dtb_struct* buscar_dtb_id( t_list *lista  ,int id ){

	bool dtb_tiene_id(dtb_struct *dtb) {
		return dtb->id_dtb == id;
	}

	return list_find(lista,(void*)dtb_tiene_id);

}



recurso_struct* buscar_recurso( char*nombre_recurso ){

	recurso_struct *recurso = NULL;

	return recurso;
}

recurso_struct*  crear_recurso( char*nombre_recurso ){

	recurso_struct *recurso = NULL;

	return recurso;
}

void asignar_recurso( recurso_struct *recurso ){

}

void liberar_recurso( recurso_struct *recurso ){

}



