/*
 * CPU.h
 *
 *  Created on: 8 sep. 2018
 *      Author: cypher
 */
#ifndef CPU_H_
#define CPU_H_

#include <qcommons/console.h>
#include <stdbool.h>
#include <commons/string.h> //Commons string
#include <qcommons/socket_viejo.h> //Libreria Socket Cliente
#include <qcommons/socketServer.h> //Libreria Socket Servidor

#include "config/config.h"

#define FILE_CONFIG_CPU "cpu.cfg"
#define CPU "CPU"


//Variables Globales
cpu_config *c_inicial = NULL;
t_config *inicializador=NULL;
t_log *logger= NULL;

Socket socket_safa;
Socket socket_dam;
Socket socket_fm9;
int quantum;

//Funciones
void liberar_memoria_cpu();
int inicializar();
void liberar_recursos(int tipo_salida);
void terminar_exitosamente();


Socket conectar_safa(cpu_config* c_inicial);
Socket conectar_dam(cpu_config* c_inicial);
Socket conectar_fm9(cpu_config* c_inicial);


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
