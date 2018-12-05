/*
 * multiplexacion.c
 *
 *  Created on: 27 nov. 2018
 *      Author: utnso
 */
#include "multiplexacion.h"
/*
 * Programa principal.
 * Ejemplo de un Servidor con Multiplexación Ip: 127.0.0.1, Puerto: 8080//omitir esto
 * Crea un socket servidor y se mete en un select() a la espera de clientes.
 * Cuando un cliente se conecta, le atiende y lo añade al select() y se
 * queda constamente en un while infinito para actuar como server
 */


void escuchaMensajes(){
	printf(" MDJ escuchando... .\n");
	buffer_mensajes_recibidos=malloc(MAX_INPUT_BUFFER);
	numeroClientes= 0;
	Socket socket = crear_socket(mdj.ip,mdj.puerto);
		socketServidor = socket.socket;
		//Asocio el servidor a un puerto
		asociar_puerto(socket);
		//Escucho Conexiones Entrantes
		escuchar(socket);

		int bytesRecibidos;
		int i_esimo_cliente;
		while (1)

			{
				/* Cuando un cliente cierre la conexión, se pondrá un -1 en su descriptor
				 * de socket dentro del array socketCliente. La función compactaClaves()
				 * eliminará dichos -1 de la tabla, haciéndola más pequeña.
				 *
				 * Se eliminan todos los clientes que hayan cerrado la conexión */
				compactaClaves (socketCliente, &numeroClientes);

				/* Se inicializa descriptoresLectura */
				FD_ZERO (&descriptoresLectura);

				/* Se añade para select() el socket servidor */
				FD_SET (socketServidor, &descriptoresLectura);

				/* Se añaden para select() los sockets con los clientes ya conectados */
				for (i_esimo_cliente=0; i_esimo_cliente<numeroClientes; i_esimo_cliente++)
					FD_SET (socketCliente[i_esimo_cliente], &descriptoresLectura);

				/* Se el valor del descriptor más grande. Si no hay ningún cliente,
				 * devolverá 0 */
				maximo = dameMaximo (socketCliente, numeroClientes);

				if (maximo < socketServidor)
					maximo = socketServidor;

				/* Espera indefinida hasta que alguno de los descriptores tenga algo
				 * que decir: un nuevo cliente o un cliente ya conectado que envía un
				 * mensaje */
				select (maximo + 1, &descriptoresLectura, NULL, NULL, NULL);

				/* Se comprueba si algún cliente ya conectado ha enviado algo */
				for (i_esimo_cliente=0; i_esimo_cliente<numeroClientes; i_esimo_cliente++)
				{
					if (FD_ISSET (socketCliente[i_esimo_cliente], &descriptoresLectura))
					{
						/* Se lee lo enviado por el cliente y se escribe en pantalla */
	//					buffer=malloc(1090);
						 bytesRecibidos=Lee_Socket (socketCliente[i_esimo_cliente],buffer_mensajes_recibidos,MAX_INPUT_BUFFER);
						if ((bytesRecibidos > 0)){
//						 if (((strlen(buffer)) > 0)){
							printf ("Cliente %d envía %s con %d bytes\n", i_esimo_cliente+1, buffer_mensajes_recibidos,strlen(buffer_mensajes_recibidos));
	//						free(buffer);
						}

						else
						{
							/* Se indica que el cliente ha cerrado la conexión y se
							 * marca con -1 el descriptor para que compactaClaves() lo
							 * elimine */
							printf ("Cliente %d ha cerrado la conexión\n", i_esimo_cliente+1);
							socketCliente[i_esimo_cliente] = -1;
	//						free(buffer);
						}
					}
				}

				/* Se comprueba si algún cliente nuevo desea conectarse y se le
				 * admite */
				if (FD_ISSET (socketServidor, &descriptoresLectura))
					nuevoCliente (socketServidor, socketCliente, &numeroClientes);
			}
		free(buffer_mensajes_recibidos);
}
