/*
 * CPU.h
 *
 *  Created on: 8 sep. 2018
 *      Author: cypher
 */
#ifndef CPU_H_
#define CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string
#include <qcommons/socket.h> //Libreria Socket Cliente
#include <qcommons/socketServer.h> //Libreria Socket Servidor


typedef struct {
	char *ip_safa;
	char *puerto_safa;
	char *ip_diego;
	char *puerto_diego;
	char *ip_fm9;
	char *puerto_fm9;
	int retardo;
} config_inicial;

//Variables Globales
config_inicial *c_inicial = NULL;
t_config *inicializador=NULL;
t_log *logger= NULL;

Socket socket_safa;
Socket socket_dam;
Socket socket_fm9;

//Funciones
void inicializar_configuracion();
void cargar_configuracion( t_config* , config_inicial*  ); //inicializa la configuracion inicial(lectura de archivo)
void imprimir_configuracion(config_inicial* c_inicial); //prueba leer archivo configuracion
void liberar_memoria_configuracion(config_inicial* c_inicial); //libera memoria de la configuracion inicial



Socket conectar_safa(config_inicial* c_inicial);
Socket conectar_dam(config_inicial* c_inicial);
Socket conectar_fm9(config_inicial* c_inicial);


//Funciones de Parser
void ejecutar_linea(char[]);
bool _esAbrirArchivo(char* linea);
bool _esConcentrar(char* linea);
bool _esAsignarLinea(char* linea);
bool _esWait(char* linea);
bool _esSignal(char* linea);
bool _esFlush(char* linea);
bool _esClose(char* linea);
bool _esCrearArchivo(char* linea);
bool _esBorrarArchivo(char* linea);
/////////////////////////////////
//otras
void liberarListaDeStrings(char** operation);

#endif /* CPU_H_ */
