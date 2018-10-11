

#include "mdj.h"

#define  MAX_INPUT_BUFFER 1000

char buffer_mensaje_cliente[MAX_INPUT_BUFFER];							/* Buffer para leer de los socket */
numeroClientes = 0;
char buffer_leyenda_temporal[MAX_INPUT_BUFFER];
char buffer_input[MAX_INPUT_BUFFER];

pthread_attr_t hilo_escucha;

int main(void) {

		mostrar_y_guardar_leyenda("MDJ escuchando .."); /* prints MDJ */
		mdj_inicializar();
	 mostrar_y_guardar_leyenda("ip : %s y puerto : %s  ",mdj->ip,mdj->puerto);

	guardar_leyenda("abriendo socket");


//	 	escuchar_mensajes();

	 	pthread_create(&hilo_escucha, NULL, escuchar_mensajes_entrantes(), NULL);
	 	pthread_join(&hilo_escucha, NULL);


	 	mdj_finalizar_y_liberar_memoria();

	 	exit(EXIT_SUCCESS);

	 }
void mdj_inicializar(){

	logger = log_create("MDJ.log", "MDJ",false, LOG_LEVEL_INFO);
	guardar_leyenda("INICIO MDJ");
	mdj=malloc(sizeof(MDJ));
	t_config *configuracion_cfg_temporal=cargar_en_memoria_cfg("mdj.cfg");
	montar_configuracion(configuracion_cfg_temporal,mdj);
	config_destroy(configuracion_cfg_temporal);
	inicializar_socket();
}
void mdj_finalizar_y_liberar_memoria(){
	 config_destroy_mdj(mdj);
	 cerrar_socket(mdj_socket);
	log_info(logger, "Finish.cfg");
	log_destroy(logger);
	puts("MDJ terminando .."); /* prints MDJ */
}
void inicializar_socket(){
	 mdj_socket=crear_socket(mdj->ip,mdj->puerto);
	 socketServidor = mdj_socket.socket;
	 //Asocio el servidor a un puerto
	 asociar_puerto(mdj_socket);
	 //Escucho Conexiones Entrantes
	 escuchar(mdj_socket);
}


void mostrar_y_guardar_leyenda(char * s, ...){
	va_list resto;
	va_start(resto,s);
	vsprintf(buffer_leyenda_temporal,s, resto );
	guardar_leyenda(buffer_leyenda_temporal);
//	puts("");
	puts(buffer_leyenda_temporal);
	va_end(resto);
//	free(s);
}
void guardar_leyenda(char * s){
	log_info(logger, s);
//	free(s);
}

void escuchar_mensajes_entrantes(){
	int i;
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
		 					if ((Lee_Socket (socketCliente[i], (char *)&buffer_mensaje_cliente, sizeof(buffer_mensaje_cliente)))){
		 						mostrar_y_guardar_leyenda("Cliente %d envía %s", i+1, buffer_mensaje_cliente);

		 					}
		 					else
		 					{
		 						/* Se indica que el cliente ha cerrado la conexión y se
		 						 * marca con -1 el descriptor para que compactaClaves() lo
		 						 * elimine */
		 						mostrar_y_guardar_leyenda("Cliente %d ha cerrado la conexión ", i+1);
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
//	free(mdj_configuracion_->retardo);
	free(mdj_configuracion_);

}
void mostrar_configuracion(MDJ* config){
	puts("iniciando lectura ..");
	printf("PUNTO_DE_MONTAJE = %s \n",config->punto_de_montaje);
	printf("RETARDO = %d \n",config->retardo);
	printf("PUERTO MDJ = %s \n",config->puerto);
	puts("---fin lectura---");
}
