/*
 * socketServer.h
 *
 *  Created on: 15 abr. 2018
 *      Author: utnso
 */

#include "socket.h"
#define MAX_CLIENTES 100 //Máxima Cantidad de Clientes que atiene el Servidor
#define BACKLOG 10     // Cantidad de conexiones pendientes en cola

/*Asociar Puerto (Servidor) */
void asociar_puerto(Socket sock);

/*Servidor escucha conexiones entrantes */
void escuchar(Socket sock);

/**
 * Acepta un cliente al Servidor.
 * Devuelve el descriptor de la conexión con el cliente o -1 si ha habido error.
 */
int Acepta_Conexion_Cliente (int socketServidor);

/* Prototipos funciones auxiliares que usa el Servidor*/
int nuevoCliente (int servidor, int *clientes, int *nClientes);
int dameMaximo (int *tabla, int n);
void compactaClaves (int *tabla, int *n);


