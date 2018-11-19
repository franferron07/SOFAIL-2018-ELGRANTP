#include "cpu.h"



cpu_struct crear_cpu(int socket) {
	cpu_struct cpu_nueva;

	cpu_nueva.ocupada = false;
	cpu_nueva.socket = socket;
	cpu_nueva.dtb_ejecutar = NULL;
	return cpu_nueva;
}




cpu_struct *obtener_cpu_libre() {

	cpu_struct *cpu_libre = NULL;
	cpu_libre = list_find( cpus , (void*)cpu_libre );

	if( cpu_libre == NULL ) return NULL;

	return cpu_libre;
}

bool cpu_libre(cpu_struct *cpu) {

	if(cpu->ocupada == true) return false;
	return true;
}


void reiniciar_cpu(cpu_struct cpu ){

	cpu.dtb_ejecutar= NULL;
	cpu.ocupada=0;
}


void inicializar_lista_cpus() {
	cpus = list_create();
}


void liberar_cpu(cpu_struct * cpu){
	free(cpu);
}

void liberar_recursos_cpu(){

	list_destroy_and_destroy_elements(cpus, (void*) liberar_cpu);
}


