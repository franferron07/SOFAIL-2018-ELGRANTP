/*
 ============================================================================
 Name        : MDJ.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "mdj.h"
int main(void) {
	puts("MDJ"); /* prints MDJ */
	mdj_configuracion_inicial = malloc(sizeof(mdj_configuracion));
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("mdj.cfg");

	montar_configuracion(configuracion_cfg_temporal,mdj_configuracion_inicial);

	config_destroy(configuracion_cfg_temporal);

	mostrar_configuracion(mdj_configuracion_inicial);

	config_destroy_mdj(mdj_configuracion_inicial);

	return EXIT_SUCCESS;
}
t_config* cargar_en_memoria_cfg(char* dir){
	t_config* aux = malloc(sizeof(mdj_configuracion));
	aux=config_create(dir);
	if(aux==NULL){
		free(mdj_configuracion_inicial);
		puts("no hay archivo mdj.cfg");
	}
	//config_create() carga en memoria el archivo .cfg ,en memoria se lo encuentra con &configuracion_temporal
	return aux;
}
void montar_configuracion(t_config*  temporal,mdj_configuracion* configuracion){
	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	configuracion->mdj_puerto=string_duplicate(config_get_string_value(temporal,"PUERTO"));
	configuracion->punto_de_montaje=string_duplicate(config_get_string_value(temporal,"PUNTO_MONTAJE"));
	configuracion->retardo=config_get_int_value(temporal,"RETARDO");
}

void config_destroy_mdj(mdj_configuracion* mdj_configuracion_){
	free(mdj_configuracion_->mdj_puerto);
	free(mdj_configuracion_->punto_de_montaje);
	free(mdj_configuracion_->retardo);
}
void mostrar_configuracion(mdj_configuracion* config){
	puts("iniciando lectura ..");
	printf("PUNTO_DE_MONTAJE = %s \n",config->punto_de_montaje);
	printf("RETARDO = %d \n",config->retardo);
	printf("PUERTO MDJ = %s \n",config->mdj_puerto);
	puts("---fin lectura---");
}
