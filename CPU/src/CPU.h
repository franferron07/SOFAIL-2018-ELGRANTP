/*
 * CPU.h
 *
 *  Created on: 8 sep. 2018
 *      Author: cypher
 */
#ifndef CPU_H_
#define CPU_H_

#include <commons/string.h> //Commons string
#include <stdbool.h>
#include <qcommons/console.h>
#include <qcommons/serialize.h>
#include <qcommons/socket.h> //Libreria Socket Cliente
#include <signal.h>

#include "config/config.h"

#define FILE_CONFIG_CPU "cpu.config"
#define EXTENSION_FILE_LOG_CPU ".log"
#define CPU "CPU"

int socket_safa, socket_diego, socket_fm9;



//Funciones
void conectarse_con_safa();
void conectarse_con_diego();
void conectarse_con_fm9();
int inicializar(char* nombre_archivo_log);
char* crear_nombre_file_log();
void liberar_recursos(int tipo_salida);
void exit_gracefully();



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
bool _esComentario(char* linea);
/////////////////////////////////
//otras
void liberarListaDeStrings(char** operation);

#endif /* CPU_H_ */
