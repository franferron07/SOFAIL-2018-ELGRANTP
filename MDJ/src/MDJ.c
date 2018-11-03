/*
 ============================================================================
 Name        : MDJ.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "MDJ.h"
int i;
// char buffer[MAX_INPUT_BUFFER];							/* Buffer para leer de los socket */
 char *buffer=NULL;				/* Buffer para leer de los socket */
 numeroClientes = 0;
 char* buffer_input_keyboard=NULL; //linea de entrada de teclado

 char* path_seleccionado[MAX_INPUT_BUFFER];
pthread_attr_t hilo_escucha;
pthread_attr_t hilo_consola_fifa;
//pthread_mutex_t mutex_recibo_mensaje;

char* aMapearAlBloque=NULL;
FILE* bloqueActual_file=NULL;
//char* bloqueActual_nombre=NULL;
char bloqueActual_path[200]; //direccion del bloque actual
t_bitarray* bitarray;
int main(void) {
//		cargar_configuracion_mdj();
//		mostrar_configuracion();

//		puts("MDJ escuchando .."); /* prints MDJ */
//		pthread_create(&hilo_consola_fifa,NULL,consola_fifa(),NULL);
//			pthread_create(&hilo_escucha, NULL,escuchar_mensajes_entrantes, NULL);
////			pthread_join(&hilo_escucha, NULL);
//			pthread_join(&hilo_consola_fifa,NULL);
//	 	mdj_finish_and_free();
//	 	return 0;
	bitarray=bitarray_create("bitmap.bin",metadata.cantidad_bloques);
	consola_fifa();
	mdj_liberar_recursos();
	return 0;
}
void consola_fifa(){
	cargar_configuracion_metadata();
	mostrar_configuracion_metadata();
	puts("press \"exit\" para salir de consola ");
		loop{
			buffer_input_keyboard=readline("fifa@mdj=>  ");
			if(!strncmp(buffer_input_keyboard, "exit", 4)) break;
			realloc(buffer_input_keyboard,sizeof(buffer_input_keyboard)+1);

			ejecutar_linea_entrante();
			free(buffer_input_keyboard);
		}
}

void  ejecutar_linea_entrante(){
	printf("ingreso \"%s\"  con %d letras \n", buffer_input_keyboard,strlen(buffer_input_keyboard));
//	system(buffer_input_keyboard);
	aMapearAlBloque=malloc(metadata.tamanio_de_bloque);
	memmove(aMapearAlBloque,buffer_input_keyboard,espacioRestanteAlBloque());
////	FILE* file=txt_open_for_append("bloque1.bin");
	FILE* file=txt_open_for_append("1.bin");
	mapearBloque(file,aMapearAlBloque);
	puts(aMapearAlBloque);
	for(FILE* unBloque=getBloqueLibre();terminoDeMapearlaLinea();unBloque=getBloqueLibre()){
		mapearBloque(unBloque,aMapearAlBloque);
	}
	free(aMapearAlBloque);
}
unsigned espacioRestanteAlBloque(char * path){
	int espacio_restante=(metadata.tamanio_de_bloque - cantidadDeBytesEnFile(path));
	if(espacio_restante<0){
		setBloqueLleno(path);
		return 0;
	}
	return espacio_restante;
}
void setBloqueLleno(char* path){//agregar un 1 al bitmap.bin

}
void  mapearBloque(FILE* bloque, char * contenido){
//	if(estaOcupado(bloque)){
//		puts("bloque ocupado");
//		return ;
//	}
	txt_write_in_file(bloque,contenido);
	txt_close_file(bloque);
}
//FILE* getBloqueLibre(){
//	return
//}
bool esta_lLeno(FILE* bloque){///debe usarse con Bitmap.bin
	return cantidadDeBytesEnFile(bloque)>=metadata.tamanio_de_bloque;
}
//bool estaOcupado(char* path){ //debe usarse con Bitmap.bin
//	return true;
//}
bool terminoDeMapearlaLinea(){
	bool estaOcupado=bitmap_bloque_esta_ocupado(bloqueActual_path);
	return estaOcupado;
}
bool bitmap_bloque_esta_ocupado(char* path_del_bloque){
	int n = bitmap_posicion_del_bloque(path_del_bloque);
	return bitarray_test_bit(bitarray,n);
}
int cantidadDeBytesEnFile(char *pathFile){
	  FILE *fich;
	  int ftam=-1;
	  fich=fopen(pathFile, "r");
	  if (fich)
	    {
	      fseek(fich, 0L, SEEK_END);
	      ftam=ftell(fich);
	      fclose(fich);
	    }
	  else
	    printf("error al saber santidad de bytes de archivo , ERRNO: %d - %s\n", errno, strerror(errno));
	  return ftam;
	}
void cargar_configuracion_metadata(){//hardcodeada, completar con config.h y  Metadata.bin
	(&metadata)->cantidad_bloques=64;
	(&metadata)->tamanio_de_bloque=50;
}
void mostrar_configuracion_metadata(){
	puts("leyendo metadata");
	printf("tamanio bloque %d \n", metadata.tamanio_de_bloque);
	printf("cantidad_bloques %d \n", metadata.cantidad_bloques);
	puts("fin lectura ");

}
void mdj_liberar_recursos(){
	 config_destroy_mdj(&mdj);
	 loggear_y_guardar_info("MDJ terminando..");
	 log_info(mdj_log, "Finish.cfg");
	 log_destroy(mdj_log);
	 free(buffer);
}
void loggear_y_guardar_info(char * s, ...){
	va_list resto;
	va_start(resto,s);
	vsprintf(leyenda_temporal,s, resto );
	printf(leyenda_temporal);
	log_info(mdj_log,leyenda_temporal);
	va_end(resto);
}

void loggear_info(char * s, ...){
	va_list resto;
	va_start(resto,s);
	vsprintf(leyenda_temporal,s, resto );
//	printf(leyenda_temporal);
	log_info(mdj_log,leyenda_temporal);
	va_end(resto);

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
		 						loggear_y_guardar_info("Cliente %d envía %s \n", i+1, buffer);
		 						loggear_y_guardar_info("cant letras recibidos => %d \n ",cant_caracteres_recibidos);
//		 						mostrar_y_guardar_log("cant caracteres recibidos => %d \n ",cant_caracteres_recibidos);

		 					}
		 					else
		 					{
		 						/* Se indica que el cliente ha cerrado la conexión y se
		 						 * marca con -1 el descriptor para que compactaClaves() lo
		 						 * elimine */
		 						loggear_info("Cliente %d ha cerrado la conexión \n", i+1);
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


void cargar_configuracion_mdj(){
	mdj_log = log_create("MDJ.log", "MDJ",false, LOG_LEVEL_INFO);
	loggear_info("INICIO MDJ");
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("mdj.cfg");
	montar_configuracion(configuracion_cfg_temporal,&mdj);
	config_destroy(configuracion_cfg_temporal);
	buffer=malloc(MAX_INPUT_BUFFER);
}


t_config* cargar_en_memoria_cfg(char* dir){
	t_config*aux=config_create(dir);
	if(aux==NULL){
//		free(mdj);
		loggear_y_guardar_info("No se encuentra archivo MDJ.cfg");
		log_error(mdj_log, leyenda_temporal);
	}
	return aux;
}

void montar_configuracion(t_config*  temporal,MDJ_CONFIG* configuracion){
	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	strcpy(configuracion->puerto,string_duplicate(config_get_string_value(temporal,"PUERTO")));
	strcpy(configuracion->punto_de_montaje,string_duplicate(config_get_string_value(temporal,"PUNTO_MONTAJE")));
	configuracion->retardo=config_get_int_value(temporal,"RETARDO");
	strcpy(configuracion->ip,string_duplicate(config_get_string_value(temporal,"IP")));

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
