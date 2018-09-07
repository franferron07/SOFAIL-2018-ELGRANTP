/*
 * socketServer.c
 *
 *  Created on: 15 abr. 2018
 *      Author: utnso
 */

#include "socketServer.h"

void asociar_puerto(Socket sock)
{
	int yes=1;
    if (setsockopt(sock.socket, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
    	cerrar_socket(sock);
        perror("setsockopt");
		exit(EXIT_FAILURE);
    }

    if (bind(sock.socket, sock.server_info->ai_addr, sock.server_info->ai_addrlen) == -1) {
    	cerrar_socket(sock);
        perror("server: bind");
		exit(EXIT_FAILURE);
    }
}

void escuchar(Socket sock)
{
    if (listen(sock.socket, BACKLOG) == -1) {
    	cerrar_socket(sock);
        perror("listen");
		exit(EXIT_FAILURE);
    }
    freeaddrinfo(sock.server_info);
}

/*
* Se le pasa un socket de servidor y acepta una conexion del cliente.
* devuelve el descriptor del socket del cliente o -1 si hay problemas.
*/
int Acepta_Conexion_Cliente (int socketServidor)
{
	socklen_t Longitud_Cliente;
	struct sockaddr_storage Cliente;
	int Hijo;

	/*
	* La llamada a la funcion accept requiere que el parametro
	* Longitud_Cliente contenga inicialmente el tamano de la
	* estructura Cliente que se le pase. A la vuelta de la
	* funcion, esta variable contiene la longitud de la informacion
	* util devuelta en Cliente
	*/
	Longitud_Cliente = sizeof (Cliente);
	Hijo = accept (socketServidor, (struct sockaddr *)&Cliente, &Longitud_Cliente);
	if (Hijo == -1)
		return -1;

	/*
	* Se devuelve el descriptor en el que esta "enchufado" el cliente.
	*/
	return Hijo;
}

/*
 * Crea un nuevo socket cliente.
 * Se le pasa el socket servidor y el array de clientes, con el número de
 * clientes ya conectados.
 */
int nuevoCliente (int servidor, int *clientes, int *nClientes)
{
	/* Acepta la conexión con el cliente, guardándola en el array */
	clientes[*nClientes] = Acepta_Conexion_Cliente (servidor);
	(*nClientes)++;

	/* Si se ha superado el maximo de clientes, se cierra la conexión,
	 * se deja todo como estaba y se vuelve. */
	if ((*nClientes) >= MAX_CLIENTES)
	{
		close (clientes[(*nClientes) -1]);
		(*nClientes)--;
		return -1;
	}

	return clientes[(*nClientes)-1];
}

/*
 * Función que devuelve el valor máximo en la tabla.
 * Supone que los valores válidos de la tabla son positivos y mayores que 0.
 * Devuelve 0 si n es 0 o la tabla es NULL */
int dameMaximo (int *tabla, int n)
{
	int i;
	int max;

	if ((tabla == NULL) || (n<1))
		return 0;

	max = tabla[0];
	for (i=0; i<n; i++)
		if (tabla[i] > max)
			max = tabla[i];

	return max;
}

/*
 * Busca en array todas las posiciones con -1 y las elimina, copiando encima
 * las posiciones siguientes.
 * Ejemplo, si la entrada es (3, -1, 2, -1, 4) con *n=5
 * a la salida tendremos (3, 2, 4) con *n=3
 */
void compactaClaves (int *tabla, int *n)
{
	int i,j;

	if ((tabla == NULL) || ((*n) == 0))
		return;

	j=0;
	for (i=0; i<(*n); i++)
	{
		if (tabla[i] != -1)
		{
			tabla[j] = tabla[i];
			j++;
		}
	}

	*n = j;
}

