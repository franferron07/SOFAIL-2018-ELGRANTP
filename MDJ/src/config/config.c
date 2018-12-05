/*
 * config.c
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */
#include "config.h"




void cargar_configuracion_mdj(const char* pathConfig){
	t_config*aux=config_create(pathConfig);
	 	if(aux==NULL){
	 		perror("No se encuentra archivo MDJ.CONFIG \n");
	 		log_error(mdj_log, "No se encuentra archivo MDJ.cfg \n");
	 	}
	 	else{
	 		strcpy( mdj.puerto,config_get_string_value(aux,"PUERTO"));
	 		strcpy( mdj.punto_de_montaje,config_get_string_value(aux,"PUNTO_MONTAJE"));
	 		strcpy( mdj.ip,config_get_string_value(aux,"IP"));
	 		mdj.retardo=config_get_int_value(aux,"RETARDO");
	 		config_destroy(aux);
	 	}
 }


// void montar_configuracion(t_config*  temporal,MDJ* configuracion){
// 	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
//
// }
//
// void config_destroy_mdj(MDJ* mdj_configuracion_){
// 	free(mdj_configuracion_->puerto);
// 	free(mdj_configuracion_->punto_de_montaje);
// 	free(mdj_configuracion_->ip);
// 	free(mdj_configuracion_);
//
// }
 void mostrar_configuracion_mdj(){
 	printf("iniciando lectura de configuracion...\n");
 	printf("PUNTO_DE_MONTAJE = %s \n",mdj.punto_de_montaje);
 	printf("RETARDO = %d \n",mdj.retardo);
 	printf("PUERTO MDJ = %s \n",mdj.puerto);
 	printf("IP MDJ = %s \n",mdj.ip);
 	printf("---fin lectura de configuracion --- .. \n\n");
 }
