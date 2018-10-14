#ifndef SOCKETCOMMONS_H
#define SOCKETCOMMONS_H

#define PACKAGE_SIZE 1024

#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen, malloc
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

int obtener_socket_cliente(int* socket_cliente, const char* direccion_ip,
		const int puerto);
int configurar_socket_servidor(int* socket_servdor, const char* direccion_ip,
		const int puerto, const int cantidad_clientes);
int aceptar_conexion(int socket_servidor);

#endif
