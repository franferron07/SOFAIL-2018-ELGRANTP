/*
 * mdj.h
 *
 *  Created on: 9 sep. 2018
 *      Author: utnso
 */
#ifndef MDJ_H_
#define MDJ_H_
#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <qcommons/socket.h> //Libreria Socket Cliente
#include <qcommons/socketServer.h> //Libreria Socket Servidor
//----------------------------
typedef struct{
	char* mdj_puerto;
	int retardo;
	char* punto_de_montaje;
}mdj_configuracion;
//----------------------------






//variables globales
mdj_configuracion* mdj_configuracion_inicial=NULL;
//int mdj_id;

//funciones
void liberar_memoria_de_configuracion(mdj_configuracion* configuracion_inicial);
//freeshea el puntero de configuracion inicial
void montar_configuracion(t_config* ,mdj_configuracion*);
void mostrar_configuracion(mdj_configuracion* configuracion_inicial);
t_config* cargar_en_memoria_cfg(char*);
void config_destroy_mdj(mdj_configuracion* );


#endif /* MDJ_H_ */
