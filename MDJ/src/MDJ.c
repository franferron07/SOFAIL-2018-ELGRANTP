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
#include <string.h>
#define  MAX_INPUT_BUFFER 1000

int i;
 char buffer[MAX_INPUT_BUFFER];							/* Buffer para leer de los socket */
 numeroClientes = 0;
 char leyenda_temporal[MAX_INPUT_BUFFER];
 char buffer_input[MAX_INPUT_BUFFER];

 char **palabras=NULL;
 char path[MAX_INPUT_BUFFER];

pthread_attr_t hilo_escucha;

int main(void) {
		mdj_init();
		mostrar_configuracion(mdj);
		puts("MDJ escuchando .."); /* prints MDJ */

//		inicializando_socket();
//	 	pthread_create(&hilo_escucha, NULL, escuchar_mensajes_entrantes(), NULL);
//	 	pthread_join(&hilo_escucha, NULL);
//		cerrar_socket(mdj_socket);

		gets(buffer_input);
		char** linea = string_split(buffer_input, " ");
		while(1){
			sleep(1);
			puts(*(linea++));
		}


	 	mdj_finish_and_free();
	 	exit(EXIT_SUCCESS);

	 }
void leer_input_linea_de_filesystem(){
	gets(buffer_input);//buffer_input almacena la linea de comando de FS(FileSystem)
	ejecutar_linea_fifa(buffer_input);


}
//void fifa_ejecutar_linea(char* linea){
////	palabras=string_split(linea, " ");
void ejecutar_linea_fifa(char linea[]){

		if( _es_cd_(linea) ){
			puts("listando ficheros de directorio actual");
			char * path = (char *) malloc(30);
			strcpy(path,linea + strlen("abrir "));
			printf("Path encontrado: %s\n",path);
			free(path);
		}else if(_es_cd_(linea)){
			printf("Instruccion Concentrar.\n");

		}else if(_es_ms5_(linea)){
			//"asignar /equipos/Racing.txt 9 GustavoBou"
			printf("Es asignar linea.\n");
			char **operation = string_split(linea + strlen("asignar "), " ");
			string_iterate_lines(operation, (void*)puts);

			liberarListaDeStrings(operation);
		}else if(_es_cat_(linea)){
			printf("Es operacion Wait.\n");
			char * recurso = (char *) malloc(30);
			strcpy(recurso,linea + strlen("wait "));
			printf("Recurso a la espera: %s\n",recurso);


			free(recurso);
		}else if(_esSignal(linea)){
			printf("Es operacion Signal.\n");
			char * recurso = (char *) malloc(30);
			strcpy(recurso,linea + strlen("signal "));
			printf("Recurso a liberar: %s\n",recurso);

			free(recurso);
		}else if(_esFlush(linea)){
			//Recibe como parámetro el path del archivo a guardar en MDJ.
			puts("Es Flush archivo");
			char * path = (char *) malloc(30);
			strcpy(path,linea + strlen("flush "));
			printf("Archivo a Flushear: %s\n",path);


			free(path);
		}else if(_esClose(linea)){
			puts("Es Cerrar archivo");
			char * path = (char *) malloc(30);
			strcpy(path,linea + strlen("flush "));
			printf("Archivo a cerrar: %s\n",path);


			free(path);
		}else if(_esCrearArchivo(linea)){
			//Recibe como parámetro el path y la cantidad de líneas que tendrá el archivo.
			//crear /equipos/Racing.txt 11
			puts("Es Crear archivo");
			char **operation = string_split(linea + strlen("crear "), " ");
			string_iterate_lines(operation, (void*)puts);


			liberarListaDeStrings(operation);
		}else if(_esBorrarArchivo(linea)){
			puts("Es Borrar archivo");
			char * path = (char *) malloc(30);
			strcpy(path,linea + strlen("borrar "));
			printf("Archivo a Borrar: %s\n",path);


			free(path);
		}
	}


bool _es_ls_(char* linea){
		return string_starts_with(linea, "ls ");
	}
bool _es_cd_(char* linea){
		return string_starts_with(linea, "cd ");
	}
bool _es_ms5_(char* linea){
		return string_starts_with(linea, "md5 ");
	}
bool _es_cat_(char* linea){
		return string_starts_with(linea, "cats ");
	}

void mdj_finish_and_free(){
	 config_destroy_mdj(mdj);

	 mostrar_y_guardar_log("MDJ terminando..");
	 log_info(logger, "Finish.cfg");
	 log_destroy(logger);

}
void mostrar_y_guardar_log(char * s, ...){
	va_list resto;
	va_start(resto,s);
	vsprintf(leyenda_temporal,s, resto );
	guardar_leyenda(leyenda_temporal);
//	puts("");
	printf(leyenda_temporal);
	va_end(resto);
//	free(s);
}
void guardar_leyenda(char * s){
	log_info(logger, s);
//	free(s);
}
void inicializando_socket(){
	mdj_socket=crear_socket(mdj->ip,mdj->puerto);
		 puts("conectando socket");
		 socketServidor = mdj_socket.socket;
		 	//Asocio el servidor a un puerto
		 	asociar_puerto(mdj_socket);
		 	//Escucho Conexiones Entrantes
		 	escuchar(mdj_socket);
}

void escuchar_mensajes_entrantes(){
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
		 						mostrar_y_guardar_log("Cliente %d envía %s \n", i+1, buffer);

		 					}
		 					else
		 					{
		 						/* Se indica que el cliente ha cerrado la conexión y se
		 						 * marca con -1 el descriptor para que compactaClaves() lo
		 						 * elimine */
		 						mostrar_y_guardar_log("Cliente %d ha cerrado la conexión \n", i+1);
		 						socketCliente[i] = -1;
		 					}
		 				}
		 			}

		 			/* Se comprueba si algún cliente nuevo desea conectarse y se le
		 			 * admite */
		 			if (FD_ISSET (socketServidor, &descriptoresLectura))
		 				nuevoCliente (socketServidor, socketCliente, &numeroClientes);
		 		}

}


void mdj_init(){
	logger = log_create("MDJ.log", "MDJ",false, LOG_LEVEL_INFO);
	guardar_leyenda("INICIO MDJ");

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
	configuracion->puerto=string_duplicate(config_get_string_value(temporal,"PUERTO"));
	configuracion->punto_de_montaje=string_duplicate(config_get_string_value(temporal,"PUNTO_MONTAJE"));
	configuracion->retardo=config_get_int_value(temporal,"RETARDO");
	configuracion->ip=string_duplicate(config_get_string_value(temporal,"IP"));

}

void config_destroy_mdj(MDJ* mdj_configuracion_){
	free(mdj_configuracion_->puerto);
	free(mdj_configuracion_->punto_de_montaje);
	free(mdj_configuracion_->ip);
	free(mdj_configuracion_);

}
void mostrar_configuracion(MDJ* config){
	mostrar_y_guardar_log("iniciando lectura de configuracion...\n");
	mostrar_y_guardar_log("PUNTO_DE_MONTAJE = %s \n",config->punto_de_montaje);
	mostrar_y_guardar_log("RETARDO = %d \n",config->retardo);
	mostrar_y_guardar_log("PUERTO MDJ = %s \n",config->puerto);
	mostrar_y_guardar_log("IP MDJ = %s \n",config->ip);
	mostrar_y_guardar_log("---fin lectura de configuracion --- .. \n\n");
}
