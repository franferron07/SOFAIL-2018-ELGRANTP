/*
 ============================================================================
 Name        : socket.c
 Author      : DennisRitchie
 Version     :
 Copyright   : UTN.SO
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "socket.h"

Socket crear_socket(char * ip, char * port) {

	Socket sock;

	memset(&sock.hints, 0, sizeof(sock.hints));
	sock.hints.ai_family = AF_UNSPEC; // Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
	sock.hints.ai_socktype = SOCK_STREAM;  // Indica que usaremos el protocolo TCP

	getaddrinfo(ip, port, &sock.hints, &sock.server_info);

	sock.socket = socket(sock.server_info->ai_family, sock.server_info->ai_socktype,
			sock.server_info->ai_protocol);

	//error
	if (sock.socket == -1) {
		cerrar_socket(sock);
		perror("server: socket");
		exit(EXIT_FAILURE);
	}

	return sock;
}

void conectar(Socket sock) {
	if (connect(sock.socket, sock.server_info->ai_addr, sock.server_info->ai_addrlen) == -1) {
		cerrar_socket(sock);
		perror("client: connect");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(sock.server_info);
}

void cerrar_socket(Socket sock)
{
	freeaddrinfo(sock.server_info);
	close(sock.socket);
}

/*
* Lee datos del socket. Supone que se le pasa un buffer con hueco
*	suficiente para los datos. Devuelve el numero de bytes leidos o
* 0 si se cierra fichero o -1 si hay error.
*/
int Lee_Socket (int fd, char *Datos, int Longitud)
{
	int Leido = 0;

	/*
	* Comprobacion de que los parametros de entrada son correctos
	*/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	* Leemos datos del socket
	*/
	while (Leido < Longitud)
	{
	Leido = recv(fd,Datos,Longitud,MSG_WAITALL);
			/*
			* Si recv devuelve 0, es que se ha cerrado el socket. Devolvemos
			* los caracteres leidos hasta ese momento
			*/
			if (Leido == 0)
				return Leido;
			if (Leido == -1)
			{
				/*
				* En caso de error, la variable errno nos indica el tipo
				* de error.
				* El error EINTR se produce si ha habido alguna
				* interrupcion del sistema antes de leer ningun dato. No
				* es un error realmente.
				* El error EGAIN significa que el socket no esta disponible
				* de momento, que lo intentemos dentro de un rato.
				* Ambos errores se tratan con una espera de 100 microsegundos
				* y se vuelve a intentar.
				* El resto de los posibles errores provocan que salgamos de
				* la funcion con error.
				*/
				switch (errno)
				{
					case EINTR:
					case EAGAIN:
						usleep (100);
						break;
					default:
						return -1;
				}
			}
		}
	/*
	* Se devuelve el total de los caracteres leidos
	*/
	return Leido;
}

/*
* Escribe dato en el socket cliente. Devuelve numero de bytes escritos,
* o -1 si hay error.
*/
int Escribe_Socket (int fd, char *Datos, int Longitud)
{
	int Escrito = 0;
	int Aux = 0;

	/*
	* Comprobacion de los parametros de entrada
	*/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	* Bucle hasta que hayamos escrito todos los caracteres que nos han
	* indicado.
	*/
	while (Escrito < Longitud)
	{
		Aux = send(fd, Datos + Escrito, Longitud - Escrito, 0);

		if (Aux > 0)
		{
			/*
			* Si hemos conseguido escribir caracteres, se actualiza la
			* variable Escrito
			*/
			Escrito = Escrito + Aux;
		}
		else
		{
			/*
			* Si se ha cerrado el socket, devolvemos el numero de caracteres
			* leidos.
			* Si ha habido error, devolvemos -1
			*/
			if (Aux == 0)
				return Escrito;
			else
				return -1;
		}
	}

	/*
	* Devolvemos el total de caracteres leidos
	*/
	return Escrito;
}




