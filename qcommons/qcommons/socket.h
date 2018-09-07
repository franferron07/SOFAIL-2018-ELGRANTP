/*
 * socket.h
 *
 *  Created on: 15 abr. 2018
 *      Author: utnso
 */

/* Mini Libreria para el Manejo Feliz de Sockets */
#ifndef SOCKETHEADER_H_
#define SOCKETHEADER_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	int socket;
	struct addrinfo hints;
	struct addrinfo *server_info;
} Socket;



#endif /* SOCKETHEADER_H_ */

/*Crea un socket devuelve una estructura feliz Socket
que contiene toda la info necesaria para los demas funciones */
Socket crear_socket(char* ip, char* port);

/*Permite conectarse*/
void conectar(Socket sock);

/*Cierra socket*/
void cerrar_socket(Socket sock);

/** Lee Datos de tamaño Longitud de un socket (fd=Socket).
 * Devuelve el numero de bytes leidos o -1 si ha habido error */
int Lee_Socket (int fd, char *Datos, int Longitud);

/** Envia Datos de tamaño Longitud por el socket (fd=Socket).
 * Devuelve el número de bytes escritos o -1 si ha habido error. */
int Escribe_Socket (int fd, char *Datos, int Longitud);

