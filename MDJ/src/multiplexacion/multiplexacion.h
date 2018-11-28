/*
 * multiplexacion.h
 *
 *  Created on: 27 nov. 2018
 *      Author: utnso
 */

#ifndef MULTIPLEXACION_MULTIPLEXACION_H_
#define MULTIPLEXACION_MULTIPLEXACION_H_
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <qcommons/socketServer.h> //Libreria Socket Servidor

#include <stdbool.h>
#include <unistd.h>


#include <qcommons/socket_viejo.h> //Libreria Socket Cliente

int socketServidor;				/* Descriptor del socket servidor */
int socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
int numeroClientes ;			/* Número clientes conectados */
fd_set descriptoresLectura;	/* Descriptores de interes para select() */
//char buffer[1000];							/* Buffer para leer de los socket */
int maximo;							/* Número de descriptor más grande */


///* char* buffer : Buffer para leer de los socket */
void escuchaMensajes(char* ip, char* puerto, char* buffer,int max_input_buffer);


#endif /* MULTIPLEXACION_MULTIPLEXACION_H_ */
