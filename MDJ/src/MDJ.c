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

#include <readline/readline.h>
#include <readline/history.h>
int i;
// char buffer[MAX_INPUT_BUFFER];							/* Buffer para leer de los socket */
 char *buffer=NULL;				/* Buffer para leer de los socket */
 numeroClientes = 0;
 char leyenda_temporal[MAX_INPUT_BUFFER];
 char* buffer_input_keyboard=NULL; //linea de entrada de teclado

 char* path_seleccionado[MAX_INPUT_BUFFER];
pthread_attr_t hilo_escucha;
pthread_attr_t hilo_consola_fifa;
//pthread_mutex_t mutex_recibo_mensaje;

int main(void) {
		mdj_init();
		mostrar_configuracion();


		puts("MDJ escuchando .."); /* prints MDJ */

//		pthread_create(&hilo_consola_fifa,NULL,consola_fifa(),NULL);

			pthread_create(&hilo_escucha, NULL,escuchar_mensajes_entrantes, NULL);


			pthread_join(&hilo_escucha, NULL);
//			pthread_join(&hilo_consola_fifa,NULL);

	 	mdj_finish_and_free();
	 	return 0;
	 }
void consola_fifa(){
	puts("press \"exit\" para salir de consola ");
	while(1){
		 buffer_input_keyboard = readline("fifa@mdj=> ");
//		 if(buffer_input_keyboard) add_history(buffer_input_keyboard);//agrega al historial , como la terminal
		 guardar_log_v2("fifa@mdj=> %s",buffer_input_keyboard);
		 if(!strncmp(buffer_input_keyboard, "exit", 4)) break; //pthread_exit(EXIT_FAILURE);//si hay algun hilo usando esta funcion sale , por "exit"{
		 ejecutar_linea_entrante();
		 free(buffer_input_keyboard);
	}
}
void  ejecutar_linea_entrante(){
	printf("ingreso \"%s\"  con %d letras \n", buffer_input_keyboard,strlen(buffer_input_keyboard));
	system(buffer_input_keyboard);
}
void mdj_finish_and_free(){
	 config_destroy_mdj(&mdj);
	 mostrar_y_guardar_log("MDJ terminando..");
	 log_info(logger, "Finish.cfg");
	 log_destroy(logger);
	 free(buffer);
}
void mostrar_y_guardar_log(char * s, ...){
	va_list resto;
	va_start(resto,s);
	vsprintf(leyenda_temporal,s, resto );
	printf(leyenda_temporal);
	log_info(logger,leyenda_temporal);
	va_end(resto);
}
void guardar_log(char* s ){
	log_info(logger,s);
}

void guardar_log_v2(char * s, ...){
	va_list resto;
	va_start(resto,s);
	vsprintf(leyenda_temporal,s, resto );
//	printf(leyenda_temporal);
	log_info(logger,leyenda_temporal);
	va_end(resto);
//	free(s);
}


void inicializando_socket(){
	mdj_socket=crear_socket(mdj.ip,mdj.puerto);
		 puts("conectando socket");
		 socketServidor = mdj_socket.socket;
		 	//Asocio el servidor a un puerto
		 	asociar_puerto(mdj_socket);
		 	//Escucho Conexiones Entrantes
		 	escuchar(mdj_socket);
}

void escuchar_mensajes_entrantes(){
	inicializando_socket();
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
//		 					int bytes_recibidos=Lee_Socket(  socketCliente[i], (char *)&buffer, sizeof(buffer));
		 					int recibi_caracteres= Lee_Socket(  socketCliente[i],buffer, MAX_INPUT_BUFFER);
		 					int cant_caracteres_recibidos=strlen(buffer);
		 					if (recibi_caracteres){
		 						mostrar_y_guardar_log("Cliente %d envía %s \n", i+1, buffer);
		 						mostrar_y_guardar_log("cant letras recibidos => %d \n ",cant_caracteres_recibidos);
//		 						mostrar_y_guardar_log("cant caracteres recibidos => %d \n ",cant_caracteres_recibidos);

		 					}
		 					else
		 					{
		 						/* Se indica que el cliente ha cerrado la conexión y se
		 						 * marca con -1 el descriptor para que compactaClaves() lo
		 						 * elimine */
		 						guardar_log_v2("Cliente %d ha cerrado la conexión \n", i+1);
//		 						perror(leyenda_temporal);
		 						 goto salto2;
		 						socketCliente[i] = -1;
		 						pthread_exit(NULL);//ver si corta el hilo
//		 						return 0;
		 					}
		 				}
		 			}

		 			/* Se comprueba si algún cliente nuevo desea conectarse y se le
		 			 * admite */
		 			if (FD_ISSET (socketServidor, &descriptoresLectura))
		 				nuevoCliente (socketServidor, socketCliente, &numeroClientes);
		 		}

	salto2://puts("");
	cerrar_socket(mdj_socket);
}


void mdj_init(){
	logger = log_create("MDJ.log", "MDJ",false, LOG_LEVEL_INFO);
	guardar_log("INICIO MDJ");

//	mdj=malloc(sizeof(mdj));
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("mdj.cfg");
	montar_configuracion(configuracion_cfg_temporal,&mdj);
	config_destroy(configuracion_cfg_temporal);
	buffer=malloc(MAX_INPUT_BUFFER);
}


t_config* cargar_en_memoria_cfg(char* dir){
	t_config* aux = malloc(sizeof(MDJ_CONFIG));
	aux=config_create(dir);
	if(aux==NULL){
//		free(mdj);
		mostrar_y_guardar_log("No se encuentra archivo MDJ.cfg");
		log_error(logger, leyenda_temporal);
	}
	//config_create() carga en memoria el archivo .cfg ,en memoria se lo encuentra con &configuracion_temporal
	return aux;
}

void montar_configuracion(t_config*  temporal,MDJ_CONFIG* configuracion){
	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	configuracion->puerto=string_duplicate(config_get_string_value(temporal,"PUERTO"));
	configuracion->punto_de_montaje=string_duplicate(config_get_string_value(temporal,"PUNTO_MONTAJE"));
	configuracion->retardo=config_get_int_value(temporal,"RETARDO");
	configuracion->ip=string_duplicate(config_get_string_value(temporal,"IP"));

}

void config_destroy_mdj(MDJ_CONFIG* mdj_configuracion_){
	free(mdj_configuracion_->puerto);
	free(mdj_configuracion_->punto_de_montaje);
	free(mdj_configuracion_->ip);
//	free(mdj_configuracion_);

}
void mostrar_configuracion(){
	printf("iniciando lectura de configuracion...\n");
	printf("PUNTO_DE_MONTAJE = %s \n",mdj.punto_de_montaje);
	printf("RETARDO = %d \n",mdj.retardo);
	printf("PUERTO MDJ = %s \n",mdj.puerto);
	printf("IP MDJ = %s \n",mdj.ip);
	printf("---fin lectura de configuracion --- .. \n\n");
}
