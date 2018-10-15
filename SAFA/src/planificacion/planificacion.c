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

void aplicarRR(bool es_virtual) {
	virtual = es_virtual;

	pthread_mutex_lock(&mutex_planificador);

	if (list_size(dtb_listos) <= 0) {
		log_info(safa_log,
				"No se encontraron CPUs en estado listo para ejecutar");
		pthread_mutex_unlock(&mutex_planificador);
		return;
	}

	if (!virtual) {
		if (dtb_ejecutando == NULL) {

		}
	} else { //Es virtual

		if (dtb_ejecutando == NULL) {

		} else {

		}
	}

	pthread_mutex_unlock(&mutex_planificador);
}

void aplicarPropio() {
	pthread_mutex_lock(&mutex_planificador);

	if (list_size(dtb_listos) <= 0) {
		pthread_mutex_unlock(&mutex_planificador);
		return;
	}

	if (dtb_ejecutando == NULL) {

	}

	pthread_mutex_unlock(&mutex_planificador);
}
