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

char* aMapearAlBloque=NULL;
FILE* bloqueActual_file=NULL;
//char* bloqueActual_nombre=NULL;
char bloqueActual_path[250]; //direccion del bloque actual
unsigned int bloqueActual_int=0;
t_bitarray* bitarray;
char* bitmap_path_directorio=NULL;
FILE* bitmap_file=NULL;
int main(void) {
//	cargar_configuracion_mdj();
//
//	mostrar_configuracion_mdj();
//	buffer=malloc(MAX_INPUT_BUFFER);
//	puts("cargar config mdj");
//
//	configurar_bitmap();
//	consola_fifa();
//
//	bitarray_destroy(bitarray);
//	mdj_liberar_recursos();
//	puts("fin");
//	char* aux="";
//	char* s =recortarPrimerosCaracteres(aux,2);
//	printf("strlen destino es  %d  y contenido %s  y restante .%s.\n ",strlen(s),s,aux);
	consola_fifa();
//	bitarray_destroy(bitarray);
//	free(s);
	return 0;
}


void configurar_bitmap(){
	char bitmap_array[metadata.cantidad_bloques/8];
	for(int i =0;i<metadata.cantidad_bloques/8;i++)bitmap_array[i]=0;
	bitarray = bitarray_create_with_mode(bitmap_array, sizeof(bitmap_array), LSB_FIRST);
	bitmap_file=txt_open_for_append("Bitmap.bin");
	txt_write_in_file(bitmap_file,bitarray->bitarray);//hacerlo con mmap()
	txt_close_file(bitmap_file);
//	bitarray_destroy(bitarray);
}
void setear_bloque_ocupado_en_posicion(int pos){
	bitarray_set_bit(bitarray,(off_t)(pos));
}
bool testear_bloque_libre_en_posicion(int pos){
	return bitarray_test_bit(bitarray,(off_t)(pos));
}
void mostrar_bitarray(){
	for(int k =0;k<metadata.cantidad_bloques;k++)printf("test bit posicion, despues de seteo %d en pos %d \n", bitarray_test_bit(bitarray,k),k);
}
void consola_fifa(){
	cargar_configuracion_metadata();
	mostrar_configuracion_metadata();
	configurar_bitmap();
	puts("press \"exit\" para salir de consola ");
	loop{
			buffer_input_keyboard=readline("fifa@mdj=>  ");
			if(!strncmp(buffer_input_keyboard, "exit", 4)) break;
			ejecutar_linea_entrante();
			free(buffer_input_keyboard);
	}
}
void  ejecutar_linea_entrante(){
//	system(buffer_input_keyboard);
	for(bloqueActual_file=getBloqueLibre_file();quedaContenidoParaMapear();bloqueActual_file=getBloqueLibre_file()){
		mapearBloque(bloqueActual_file,buffer_input_keyboard);
		setear_bloque_ocupado_en_posicion(bloqueActual_int);
	}
}

bool quedaContenidoParaMapear(){return strlen(buffer_input_keyboard)>0;}
 int  espacioRestanteAlBloque(){
	return metadata.tamanio_de_bloque-cantidadDeCaracteres_file(bloqueActual_file);
}
void setBloqueLleno(){//agregar un 1 al bitmap.bin
	bitarray_set_bit(bitarray,bloqueActual_int);
}
//int minimo(int unNum,int otroNum){return unNum>otroNum?unNum:otroNum;}

void  mapearBloque(FILE* bloque, char * contenido){
	aMapearAlBloque=recortarPrimerosCaracteres(contenido,minimo(metadata.tamanio_de_bloque,espacioRestanteAlBloque()));
	printf("se va a mapear al bloque %s y el restante es %s \n",aMapearAlBloque,buffer_input_keyboard);
	txt_write_in_file(bloque,aMapearAlBloque);
	txt_close_file(bloque);
	free(aMapearAlBloque);
}
FILE* getBloqueLibre_file(){
	int i;
	for( i =0;testear_bloque_libre_en_posicion(i);i++);//hasta un bloque lbre
	char* path_del_bloque_libre = malloc(1000);
	sprintf(path_del_bloque_libre,"%d.bin",i);//rehacer path con punto de ontaje y carpeta segun dam
	bloqueActual_file = fopen(path_del_bloque_libre,"w");//txt_open_for_append(path_bloque); SI LO ABRO COMO "W" SE BORRA EL CONTENIDO
	free(path_del_bloque_libre);
	return bloqueActual_file;
}

bool estaLLenoElBloqueActual(){
	return cantidadDeCaracteres_file(bloqueActual_file)==metadata.tamanio_de_bloque;
}
bool estaLibreElBloqueActual(){return cantidadDeCaracteres_file(bloqueActual_file)<metadata.tamanio_de_bloque;}
int cantidadDeCaracteres_file(FILE* bloque){
	int i ;
	for(i=0;getc(bloque)!=EOF;i++);
	return i;
}
int cantidadDeCaracteres_path(char* path ){
	FILE * f = fopen(path,"r");
	int n = cantidadDeCaracteres_file(f);
	fclose(f);
	return n ;
}
bool terminoDeMapearContenido(){
	bool hayCaracteresParaMapear=strlen(buffer_input_keyboard)>0;
	return bitarray_test_bit(bitarray,bloqueActual_int)&&hayCaracteresParaMapear;
}

void cargar_configuracion_metadata(){//hardcodeada, completar con config.h y  Metadata.bin
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("Metadata.bin");
//	(&metadata)->cantidad_bloques=config_get_int_value(configuracion_cfg_temporal,"CANTIDAD_BLOQUES");
	metadata.cantidad_bloques=config_get_int_value(configuracion_cfg_temporal,"CANTIDAD_BLOQUES");
	metadata.tamanio_de_bloque=config_get_int_value(configuracion_cfg_temporal,"TAMANIO_BLOQUES");
	config_destroy(configuracion_cfg_temporal);
}
void mostrar_configuracion_metadata(){
	puts("leyendo metadata");
	printf("tamanio bloque %d \n", metadata.tamanio_de_bloque);
	printf("cantidad_bloques %d \n", metadata.cantidad_bloques);
	puts("fin lectura metadata ");

}
void mdj_liberar_recursos(){
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
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("MDJ.cfg");
	puts("antes de  montar_configuracion");
	montar_configuracion(configuracion_cfg_temporal);
	config_destroy(configuracion_cfg_temporal);
	puts("despues de montar config");
}


t_config* cargar_en_memoria_cfg(char* dir){//ok
	t_config*aux=config_create(dir);
	if(aux==NULL){
		perror("No se encuentra archivo MDJ.cfg \n");
		log_error(mdj_log, "No se encuentra archivo MDJ.cfg \n");
	}
	return aux;
}

void montar_configuracion(t_config*  temporal){
//	strcpy(mdj.puerto ,string_duplicate(config_get_string_value(temporal,"PUERTO")));
	strcpy(mdj.puerto ,config_get_string_value(temporal,"PUERTO"));
	strcpy(mdj.punto_de_montaje,config_get_string_value(temporal,"PUNTO_MONTAJE"));
	mdj.retardo=config_get_int_value(temporal,"RETARDO");
	strcpy(mdj.ip,config_get_string_value(temporal,"IP"));

}
void mostrar_configuracion_mdj(){
	printf("iniciando lectura de configuracion...\n");
	printf("PUNTO_DE_MONTAJE = %s \n",mdj.punto_de_montaje);
	printf("RETARDO = %d \n",mdj.retardo);
	printf("PUERTO MDJ = %s \n",mdj.puerto);
	printf("IP MDJ = %s \n",mdj.ip);
	printf("---fin lectura de configuracion --- .. \n\n");
}
