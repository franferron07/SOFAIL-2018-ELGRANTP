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
#include <stdarg.h>
#include <pthread.h>
#define  MAX_INPUT_BUFFER 1000

int i;
 char buffer[MAX_INPUT_BUFFER];							/* Buffer para leer de los socket */
 numeroClientes = 0;
 char leyenda_temporal[MAX_INPUT_BUFFER];

pthread_attr_t hilo_escucha;

int main(void) {

		puts("MDJ escuchando .."); /* prints MDJ */

		 logger = log_create("MDJ.log", "MDJ",false, LOG_LEVEL_INFO);
		log_info(logger, "INICIO MDJ");
	mdj_setear_configuracion_default();
	 Socket socket=crear_socket(mdj->ip,mdj->mdj_puerto);
//	 char s1[strlen("ip : 127.0.0.1 y puerto : 8080  conectando socket")+1];
	 mostrar_y_guardar_leyenda("ip : %s y puerto : %s  ",mdj->ip,mdj->mdj_puerto);

	 log_info(logger, leyenda_temporal);
	 puts("conectando socket");
	 socketServidor = socket.socket;
	 	//Asocio el servidor a un puerto
	 	asociar_puerto(socket);
	 	//Escucho Conexiones Entrantes
	 	escuchar(socket);

//	 	escuchar_mensajes();

	 	pthread_create(&hilo_escucha, NULL, escuchar_mensajes(), NULL);
	 	pthread_join(&hilo_escucha, NULL);


	 	 config_destroy_mdj(mdj);
	 	 cerrar_socket(socket);

	 	puts("MDJ terminando .."); /* prints MDJ */

	 	log_info(logger, "Finish.cfg");
	 	log_destroy(logger);

	 	exit(EXIT_SUCCESS);

	 }
void mostrar_y_guardar_leyenda(char * s, ...){
	va_list resto;
	va_start(resto,s);
	vsprintf(leyenda_temporal,s, resto );
	guardar_leyenda(leyenda_temporal);
//	puts("");
	puts(leyenda_temporal);
	va_end(resto);
//	free(s);
}
void guardar_leyenda(char * s){
	log_info(logger, s);
//	free(s);
}

void escuchar_mensajes(){
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
		 					if ((Lee_Socket (socketCliente[i], (char *)&buffer, sizeof(buffer)))){
		 						mostrar_y_guardar_leyenda("Cliente %d envía %s\n", i+1, buffer);

		 					}
		 					else
		 					{
		 						/* Se indica que el cliente ha cerrado la conexión y se
		 						 * marca con -1 el descriptor para que compactaClaves() lo
		 						 * elimine */
		 						printf ("Cliente %d ha cerrado la conexión\n", i+1);
		 						socketCliente[i] = -1;
		 						log_info(logger, "INICIO MDJ");
		 					}
		 				}
		 			}

		 			/* Se comprueba si algún cliente nuevo desea conectarse y se le
		 			 * admite */
		 			if (FD_ISSET (socketServidor, &descriptoresLectura))
		 				nuevoCliente (socketServidor, socketCliente, &numeroClientes);
		 		}

}


void mdj_setear_configuracion_default(){
	mdj=malloc(sizeof(MDJ));
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("mdj.cfg");
	montar_configuracion(configuracion_cfg_temporal,mdj);
	config_destroy(configuracion_cfg_temporal);
}


t_config* cargar_en_memoria_cfg(char* dir){
	t_config* aux = malloc(sizeof(MDJ));
	aux=config_create(dir);
	if(aux==NULL){
		free(mdj);
		puts("Error");

		log_error(logger, "No se encuentra archivo MDJ.cfg");
	}
	//config_create() carga en memoria el archivo .cfg ,en memoria se lo encuentra con &configuracion_temporal
	return aux;
}

void montar_configuracion(t_config*  temporal,MDJ* configuracion){
	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	configuracion->mdj_puerto=string_duplicate(config_get_string_value(temporal,"PUERTO"));
	configuracion->punto_de_montaje=string_duplicate(config_get_string_value(temporal,"PUNTO_MONTAJE"));
	configuracion->retardo=config_get_int_value(temporal,"RETARDO");
	configuracion->ip=string_duplicate(config_get_string_value(temporal,"IP"));

}

void config_destroy_mdj(MDJ* mdj_configuracion_){
	free(mdj_configuracion_->mdj_puerto);
	free(mdj_configuracion_->punto_de_montaje);
//	free(mdj_configuracion_->retardo);
	free(mdj_configuracion_);

}
void mostrar_configuracion(MDJ* config){
	puts("iniciando lectura ..");
	printf("PUNTO_DE_MONTAJE = %s \n",config->punto_de_montaje);
	printf("RETARDO = %d \n",config->retardo);
	printf("PUERTO MDJ = %s \n",config->mdj_puerto);
	puts("---fin lectura---");
}
