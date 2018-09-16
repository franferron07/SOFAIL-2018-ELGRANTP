/*
 ============================================================================
 Name        : MDJ.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "mdj.h"



int main(void) {

	int socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
	fd_set descriptoresLectura;	/* Descriptores de interes para select() */
	int numeroClientes = 0;			/* Número clientes conectados */
	int maximo;							/* Número de descriptor más grande */
	int buffer;							/* Buffer para leer de los socket */
	int socketServidor;				/* Descriptor del socket servidor */
	int i;
		puts("MDJ escuchando .."); /* prints MDJ */
	mdj_setear_configuracion_default();
	 Socket socket=crear_socket("127.0.0.1","8080");
	 socketServidor = socket.socket;
	 	//Asocio el servidor a un puerto
	 	asociar_puerto(socket);
	 	//Escucho Conexiones Entrantes
	 	escuchar(socket);

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
	 			for (i=0; i<numeroClientes; i++)
	 				FD_SET (socketCliente[i], &descriptoresLectura);

	 			/* Se el valor del descriptor más grande. Si no hay ningún cliente,
	 			 * devolverá 0 */
	 			maximo = dameMaximo (socketCliente, numeroClientes);

	 			if (maximo < socketServidor)
	 				maximo = socketServidor;

	 			/* Espera indefinida hasta que alguno de los descriptores tenga algo
	 			 * que decir: un nuevo cliente o un cliente ya conectado que envía un
	 			 * mensaje */
	 			select(maximo + 1, &descriptoresLectura, NULL, NULL, NULL);

	 			/* Se comprueba si algún cliente ya conectado ha enviado algo */
	 			for (i=0; i<numeroClientes; i++)
	 			{
	 				if (FD_ISSET (socketCliente[i], &descriptoresLectura))
	 				{
	 					/* Se lee lo enviado por el cliente y se escribe en pantalla */
	 					if ((Lee_Socket (socketCliente[i], (char *)&buffer, sizeof(int)) > 0))
	 						printf ("Cliente %d envía %d\n", i+1, buffer);
	 					else
	 					{
	 						/* Se indica que el cliente ha cerrado la conexión y se
	 						 * marca con -1 el descriptor para que compactaClaves() lo
	 						 * elimine */
	 						printf ("Cliente %d ha cerrado la conexión\n", i+1);
	 						socketCliente[i] = -1;
	 					}
	 				}
	 			}

	 			/* Se comprueba si algún cliente nuevo desea conectarse y se le
	 			 * admite */
	 			if (FD_ISSET (socketServidor, &descriptoresLectura))
	 				nuevoCliente (socketServidor, socketCliente, &numeroClientes);
	 		}
	 	 config_destroy_mdj(mdj_configuracion_inicial);
	 	 cerrar_socket(socket);

	 }

void mdj_setear_configuracion_default(){
	mdj_configuracion_inicial=malloc(sizeof(mdj_configuracion));
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("mdj.cfg");
	montar_configuracion(configuracion_cfg_temporal,mdj_configuracion_inicial);
	config_destroy(configuracion_cfg_temporal);
}


t_config* cargar_en_memoria_cfg(char* dir){
	t_config* aux = malloc(sizeof(mdj_configuracion));
	aux=config_create(dir);
	if(aux==NULL){
		free(mdj_configuracion_inicial);
		puts("Error");
	}
	//config_create() carga en memoria el archivo .cfg ,en memoria se lo encuentra con &configuracion_temporal
	return aux;
}

void montar_configuracion(t_config*  temporal,mdj_configuracion* configuracion){
	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	configuracion->mdj_puerto=string_duplicate(config_get_string_value(temporal,"PUERTO"));
	configuracion->punto_de_montaje=string_duplicate(config_get_string_value(temporal,"PUNTO_MONTAJE"));
	configuracion->retardo=config_get_int_value(temporal,"RETARDO");
}

void config_destroy_mdj(mdj_configuracion* mdj_configuracion_){
	free(mdj_configuracion_->mdj_puerto);
	free(mdj_configuracion_->punto_de_montaje);
//	free(mdj_configuracion_->retardo);
	free(mdj_configuracion_);

}
void mostrar_configuracion(mdj_configuracion* config){
	puts("iniciando lectura ..");
	printf("PUNTO_DE_MONTAJE = %s \n",config->punto_de_montaje);
	printf("RETARDO = %d \n",config->retardo);
	printf("PUERTO MDJ = %s \n",config->mdj_puerto);
	puts("---fin lectura---");
}
