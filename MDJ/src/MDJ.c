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
 int numeroClientes = 0;
 char* buffer_input_keyboard=NULL; //linea de entrada de teclado

 char* path_seleccionado[MAX_INPUT_BUFFER];
pthread_attr_t hilo_escucha;
pthread_attr_t hilo_consola_fifa;

char* aMapearAlBloque=NULL;

////variables para seteo de bloques
//bloqueActual_file=NULL;
//bitmap_path_directorio=NULL;
//bitmap_file=NULL;
//bitarray=NULL;
int main(void) {
cargar_configuracion_mdj();
mostrar_configuracion_mdj();
cargar_configuracion_metadata();
mostrar_configuracion_metadata();

char bits[metadata.cantidad_bloques/8];
configurar_bitmap(bits,metadata.cantidad_bloques/8);
//FILE* f = fopen("holas/lol.bin","w+");
//fprintf(f," no esta");
//txt_close_file(f);
mostrar_bitarray();
bloqueActual_path=malloc(300);
consola_fifa();
free(bloqueActual_path);



puts("fin");
	return 0;
}

//BITMAP begin
void configurar_bitmap(char bitmap_array[], int cantidadDeBytes){
//	static char  bitmap_array[cantidadDeBytes];
	for(int k =0;k<cantidadDeBytes;k++)bitmap_array[k]=0b00000000;
//	char* bitmap_string=malloc(cantidadDeBytes);
//	memset(bitmap_string,'0',cantidadDeBytes);//mejor con array
	printf("strlen es  %d con valor %s \n",cantidadDeBytes,bitmap_array);
	bitarray_ = bitarray_create_with_mode(bitmap_array, cantidadDeBytes, LSB_FIRST);
	bitmap_file=fopen("Bitmap.bin","w+");
	txt_write_in_file(bitmap_file,bitarray_->bitarray);//hacerlo con mmap()
//	memmove(bitmap_file,bitarray_->bitarray,bitarray_->size);
	txt_close_file(bitmap_file);
}

void setear_bloque_ocupado_en_posicion(off_t pos){
	bitarray_set_bit(bitarray_,pos);
}
bool testear_bloque_libre_en_posicion(int pos){
	return bitarray_test_bit(bitarray_,(off_t)(pos));
}
void mostrar_bitarray(){
	for(int k =0;k<(bitarray_get_max_bit(bitarray_));k++)printf("test bit posicion, despues de seteo %d en pos %d \n", bitarray_test_bit(bitarray_,k),k);
}
void setBloqueActuaLleno(){//agregar un 1 al bitmap.bin
	bitarray_set_bit(bitarray_,bloqueActual_int);
}



char* getBloqueLibre_file(){
	int j;
	for( j =0;testear_bloque_libre_en_posicion(j);j++);//hasta un bloque lbre
	char* path_del_bloque_libre = malloc(1000);
	sprintf(path_del_bloque_libre,"%d.bin",j);//rehacer path con punto de ontaje y carpeta segun dam
//	bloqueActual_path = fopen(path_del_bloque_libre,"w+");//txt_open_for_append(path_bloque); SI LO ABRO COMO "W" SE BORRA EL CONTENIDO
	strcpy(bloqueActual_path,path_del_bloque_libre);
	free(path_del_bloque_libre);
	return bloqueActual_path;
}
bool estaLibreElBloqueActual(FILE* bloqueActual, int tamanioDeBloque){
	return cantidadDeCaracteres_file(bloqueActual)<tamanioDeBloque;
}

//BITMAP end


void consola_fifa(){
	puts("press \"exit\" para salir de consola ");
	loop{
			buffer_input_keyboard=readline("fifa@mdj=>  ");
			if(!strncmp(buffer_input_keyboard, "exit", 4)) break;
			ejecutar_linea_entrante(buffer_input_keyboard);
			free(buffer_input_keyboard);
	}
}
void  ejecutar_linea_entrante(char* buffer_entrante){
//	system(buffer_entrante);
	persistirContenido(buffer_entrante);
}
void persistirContenido(char * contenido){
	for(bloqueActual_path=getBloqueLibre_file();quedaContenidoParaMapear(contenido);bloqueActual_path=getBloqueLibre_file()){
			persistirAlBloque(bloqueActual_path,contenido);
			if(!quedaContenidoParaMapear(contenido) || estaLLenoElBloqueActual())setear_bloque_ocupado_en_posicion(bloqueActual_int);
		}
			setear_bloque_ocupado_en_posicion(bloqueActual_int);
		free(aMapearAlBloque);
}


 int  espacioRestanteAlBloque(){
	return metadata.tamanio_de_bloque-cantidadDeCaracteres_path(bloqueActual_path);
}


void  persistirAlBloque(char* unBloquePath, char * contenido){
	aMapearAlBloque=recortarPrimerosCaracteres(contenido,minimo(metadata.tamanio_de_bloque,espacioRestanteAlBloque()));
	printf("se va a mapear al bloque %s y el restante es %s \n",aMapearAlBloque,contenido);
//	 sprintf(bloqueActual_path,"%d.bin",bloqueActual_int);

	FILE* bloqueActual =txt_open_for_append(unBloquePath);
	txt_write_in_file(bloqueActual,aMapearAlBloque);
	txt_close_file(bloqueActual);

}
bool estaLLenoElBloqueActual(){
	return cantidadDeCaracteres_path(bloqueActual_path)==metadata.tamanio_de_bloque;
}

bool terminoDeMapearContenido(){
	bool hayCaracteresParaMapear=strlen(buffer_input_keyboard)>0;
	return bitarray_test_bit(bitarray_,bloqueActual_int)&&hayCaracteresParaMapear;
}


void mostrar_configuracion_mdj(){
 	printf("iniciando lectura de configuracion...\n");
 	printf("PUNTO_DE_MONTAJE = %s \n",mdj.punto_de_montaje);
 	printf("RETARDO = %d \n",mdj.retardo);
 	printf("PUERTO MDJ = %s \n",mdj.puerto);
 	printf("IP MDJ = %s \n",mdj.ip);
 	printf("---fin lectura de configuracion --- .. \n\n");
 }

void mdj_liberar_recursos(){
	 loggear_y_guardar_info("MDJ terminando..");
	 log_info(mdj_log, "Finish.cfg");
	 log_destroy(mdj_log);
	 free(buffer);
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




//METADATA
void cargar_configuracion_metadata(){
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("Metadata.bin");
	if(configuracion_cfg_temporal==NULL)perror("no cargo bien  Metadata.bin\n");
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
//METADATA






