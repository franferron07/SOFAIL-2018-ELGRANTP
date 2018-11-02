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

		/******* SI TENGO CPU DISPOSNIBLE *********/
		if( cpu_ejecutar != NULL ){

			log_info(safa_log, "Se encontro CPU para ejecutar");
			sem_wait( &sem_listo_vacio );
			pthread_mutex_lock( &sem_listo_mutex );
			dtb_ejecutar = aplicar_algoritmo_planificacion();
			log_info(safa_log, "Se encontro dtb a ejecutar: %s",dtb_ejecutar->id_dtb);
			pthread_mutex_unlock( &sem_listo_mutex );


			/* TODO agregar a lista de ejecucion el dtb. Y SACAR DE LA LISTA LOS DTBS */

			/***** INDICO A CPU EL DTB A EJECUTAR *****/
			cpu_ejecutar->ocupada = true;
			cpu_ejecutar->dtb_ejecutar = dtb_ejecutar;

		}


	}

}



void ejecutar_planificacion_largo_plazo() {

	dtb_struct *dtb;

	while (1) {

		////TOMO PRIMER DTB EN NUEVO
		if( !list_is_empty( dtb_nuevos )  )
		{
			pthread_mutex_lock(&sem_nuevo_mutex);
			dtb = list_get(dtb_nuevos ,0);
			pthread_mutex_unlock(&sem_nuevo_mutex);

			//INICIALIZO DUMMY
			pthread_mutex_lock(&sem_dtb_dummy_mutex);
			inicializar_dummy(dtb);
			log_info(safa_log, "DTB dummy inicializado");
			sem_wait(&sem_listo_max);
			//AGREGO DUMMY A LISTOS SI MULTIPROGRAMACION LO PERMITE
			pthread_mutex_lock(&sem_listo_mutex);
			list_add(dtb_listos, &dtb_dummy);
			pthread_mutex_unlock(&sem_listo_mutex);
			sem_post(&sem_listo_vacio);
		}

	}
}


void inicializar_dummy(dtb_struct* dtb) {

	dtb_dummy.id_dtb = dtb->id_dtb;
	dtb_dummy.escriptorio = strdup(dtb->escriptorio);
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


	if (list_size(dtb_listos) <= 0) {
		log_info(safa_log,
				"No se encontraron DTBS en estado listo para ejecutar");

		return NULL;
	}

	return list_get(dtb_listos , 0);


}


dtb_struct* aplicarVRR() {


	if (list_size(dtb_listos) <= 0) {
		log_info(safa_log,
				"No se encontraron DTBS en estado listo para ejecutar");

		return NULL;
	}

	/* VERIFICO LISTA DE MAYOR PRIORIDAD  */
	if(list_size(dtb_listos_mayor_prioridad) <= 0){

		return list_get(dtb_listos , 0);
	}

	return list_get( dtb_listos_mayor_prioridad , 0 );
}


dtb_struct* aplicarPropio() {


	if (list_size(dtb_listos) <= 0) {

		return NULL;
	}

	if (dtb_ejecutando == NULL) {

	}

}
