/*
 ============================================================================
 Name        : SAFA.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "safaHeader.h"
#include <semaphore.h>

//GLOBALES

config_inicial *c_inicial = NULL;

t_log* logger= NULL;
Socket socket_servidor;

pthread_t hilo_dam;
pthread_t hilo_consola;
pthread_t hilo_plp;
pthread_t hilo_pcp;


//comienzo safa en un estado corrupto
int conecto_cpu = 0 ;
int conecto_dam = 0;




//ids para asignar a los dtbs
int id_total = 1;


//SEMAFOROS
pthread_mutex_t mutex_sem_nuevo;
sem_t sem_nuevo_vacio;
sem_t mutex_dtb_dummy;

sem_t mutex_listo;
sem_t sem_listo_vacio;



//ESTADOS
t_queue *nuevos=NULL;
t_queue *listos=NULL;
t_queue *ejecucion=NULL;
t_queue *bloqueados=NULL;
t_queue *terminados=NULL;


//creo dtb_dummy
struct_dtb dtb_dummy;


int main(void) {


	dtb_dummy.inicializado=0;


    //CREO COLA DE ESTADOS
    nuevos=queue_create();
    listos=queue_create();
    ejecucion=queue_create();
    bloqueados=queue_create();
    terminados=queue_create();


    //inicio semaforos
    pthread_mutex_init( &mutex_sem_nuevo , NULL );
    sem_init(&sem_nuevo_vacio, 0, 0);
    sem_init(&mutex_dtb_dummy, 0, 1);
    sem_init( &mutex_listo , 0 , 1 );
    sem_init( &sem_listo_vacio , 0 , 0 );


    int buffer;

	logger = log_create("SafaLog.log", "SAFA",false, LOG_LEVEL_INFO);
	t_config *inicializador;

	c_inicial = malloc(sizeof(config_inicial));

	inicializador = config_create("safa.cfg");

	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		log_info(logger, "No se encuetra archivo de configuracion inicial %s", "INFO");
		exit(EXIT_FAILURE);
	}

	//leo archivo
	leer_configuracion(inicializador, c_inicial);
	log_info(logger, "Leo archivo %s", "INFO");

	//muestro consola valor leido de archivo como prueba
	prueba_leer_archivo_cfg(c_inicial);
	log_info(logger, "muestro valor por consola %s", "INFO");

	// libero memoria de inicializacion  *
	config_destroy(inicializador);

	// CONSOLA
	pthread_create(&hilo_consola , NULL , (void*)consolaSafa  , NULL );

	//PLP
	pthread_create(&hilo_plp , NULL , (void*)plp  , NULL );

	//PCP
	pthread_create(&hilo_pcp , NULL , (void*)pcp  , NULL );

	/*
	// crear socket  //
	Socket socket_servidor = crear_socket(  "127.0.0.1" , c_inicial->puerto_safa);
	log_info(logger, "Creo socket %s", "INFO");
	//Asocio el servidor a un puerto
	asociar_puerto(socket_servidor);

	//Escucho Conexiones Entrantes
	escuchar(socket_servidor);




	//lanzo hilo por cada una de las conexiones aceptadas
	while(1)
	{
		int socketCliente = Acepta_Conexion_Cliente(socket_servidor.socket);

		//Leo un Mensaje del Servidor
		if( Lee_Socket(socketCliente, (char *)&buffer, sizeof(int)) == -1 ) {
			puts("Error de lectura");
			log_info(logger, "Error de lectura %s", "INFO");
			exit(EXIT_FAILURE);
		}

		//abro hilo para dma
		if( buffer == 60 ){

			log_info(logger, "Conexion de DAM %s", "INFO");
			int rc = pthread_create (&hilo_dam, NULL, (void*)conexion_dam, &socketCliente);
			if(rc){
				puts("Error al crear thread DAM");
				log_info(logger, "Error al crear thread DAM %s", "INFO");
				exit(EXIT_FAILURE);
			}

			log_info(logger, "thread DAM creado %s", "INFO");


			//chequeo si es el primer cpu que se conecta
			if( conecto_dam == 0 )
			{
				conecto_dam = 1;
				//verifico si cpu ya conecto para ponerlo en un estado operativo.
				if( conecto_cpu == 1 ){
					//CONSOLA//
					pthread_create(&hilo_consola , NULL , (void*)consolaSafa  , NULL );
				}

			}



		}

		//abro hilo cpu
		if( buffer == 40 ){

			//verificar si tengo que tener algun maximo de cpus conectadas.

			pthread_t hilo_cpu;
			int rc = pthread_create (&hilo_cpu, NULL, (void*)conexion_cpu, &socketCliente);
			if(rc){
				puts("Error al crear thread CPU");
				log_info(logger, "Error al crear thread CPU %s", "INFO");
				exit(EXIT_FAILURE);
			}

			log_info(logger, "thread CPU creado %s", "INFO");


			//chequeo si es el primer cpu que se conecta
			if( conecto_cpu == 0 )
			{
				conecto_cpu = 1;
				//verifico si cpu ya conecto para ponerlo en un estado operativo.
				if( conecto_dam == 1 ){
					// CONSOLA//
					pthread_create(&hilo_consola , NULL , (void*)consolaSafa  , NULL );
				}
			}


		}

		int conect=0;
		if( Escribe_Socket(socketCliente, (char *)&conect, sizeof(int)) == -1 ) {
			puts( "Error en envio de mensaje" );
			log_info(logger, "Error al enviar mensaje  %s", "INFO");
			exit(EXIT_FAILURE);
		}


		printf("se recibio el id: %d\n",buffer);


	}



	cerrar_socket(socket_servidor);

*/

	pthread_join(hilo_plp,NULL);
	pthread_join(hilo_pcp,NULL);
	pthread_join(hilo_consola,NULL);



	// libero struct config_inicial  //
	liberarMemoriaConfig(c_inicial);
	log_info(logger, "Libero memoria de configuracion inicial %s", "INFO");

	//destruyo logger
	log_destroy(logger);


	//destruyo semaforos
	sem_destroy(&sem_nuevo_vacio);
	sem_destroy(&mutex_dtb_dummy);
	sem_destroy(&mutex_listo);
	sem_destroy(&sem_listo_vacio);


	return EXIT_SUCCESS;
}





//hilo de conexion con dam, debera quedarse a la espera
void conexion_dam(void* socket){

	while(1){

		printf("conexion dam correcta");

	}


}

void conexion_cpu(void* socket){

	while(1){

		printf("conexion cpu correcta");

	}


	pthread_detach(pthread_self()); //libera recursos del hilo
	pthread_exit(NULL);

}



void plp(){


	//aca verificar grado de multiprogramacion , si se lo permite hacer el pop

	while(1){

		//controlo lista no vacia
		sem_wait( &sem_nuevo_vacio);

		struct_dtb *dtb;
		//seccion critica
		pthread_mutex_lock(&mutex_sem_nuevo );

			dtb = queue_pop( nuevos);

		pthread_mutex_unlock(&mutex_sem_nuevo );

		//hecho el pop debo ir a desbloquear el dtb dummy , incializarlo ,  y pasarlo a cola de ready.

		//inicializo dtb dummy
		sem_wait( &mutex_dtb_dummy );

			dtb_dummy.idDtb = dtb->idDtb;
			dtb_dummy.escriptorio = string_duplicate( dtb->escriptorio );

		sem_post(&mutex_dtb_dummy);

		//si grado de multprogramacion lo permite pasarlo a new.  -->pendiente

		//agrego a cola de listos el dummy
		sem_wait( &mutex_listo );

			queue_push( listos , &dtb_dummy  );

		sem_post( &mutex_listo );


	}

}


void pcp(){


}


void consolaSafa(){

	puts("\nConsola SAFA \n");

	while(1){


		char **p_chars=NULL;
		char * codigo_str = NULL;

		codigo_str = string_duplicate( readline("Comando: ") );

		if( !string_is_empty(codigo_str)){

			p_chars = string_split( codigo_str , " ");

			//EJECUTAR
			if( string_equals_ignore_case( *p_chars , "ejecutar" ) ){


				if(  *(p_chars+1) != NULL ) {

					ejecutar_path_plp( *(p_chars+1) );
					puts( *(p_chars+1) );
				}

				else{
					puts("Error, indique un path");
				}

			}

			//STATUS
			else if( string_equals_ignore_case( *p_chars , "status" ) ){

				puts("status");

				if( *(p_chars+1) != NULL ){
					status_dtb(  atoi(*(p_chars+1) ) );
				}
				else{
					status_colas();
				}
			}

			//FINALIZAR
			else if( string_equals_ignore_case ( *p_chars , "finalizar" )  ){

				if( *(p_chars+1) != NULL ){
					finalizar_dtb(  atoi(*(p_chars+1 ) ) );
				}
				else{
					puts("Error, indique un id de DTB");
				}
			}

			//METRICAS
			else if( string_equals_ignore_case( *p_chars , "metricas" )  ){

				metricas( atoi(*(p_chars+1) ) );
			}


			free(codigo_str);
			string_iterate_lines(p_chars, (void*)free);

		}


		else
		{
			puts("Indique un comando");
		}

	}

}



void leer_configuracion(t_config *inicializador , config_inicial *c_inicial ){

	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	c_inicial->puerto_safa = string_duplicate(config_get_string_value(inicializador, "PUERTO"));
	c_inicial->algoritmo = string_duplicate(config_get_string_value(inicializador, "ALGORITMO"));
	c_inicial->quantum= config_get_int_value(inicializador, "QUANTUM");
	c_inicial->multiprogramacion= config_get_int_value(inicializador, "MULTIPROGRAMACION");
	c_inicial->retardo= config_get_int_value(inicializador, "RETARDO_PLANIF");

}

void prueba_leer_archivo_cfg(config_inicial* c_inicial) {
	puts("lectura de archivo correcta");
	puts(c_inicial->puerto_safa);
	puts(c_inicial->algoritmo);
	printf("%d\n", c_inicial->quantum);
	printf("%d\n", c_inicial->multiprogramacion);
	printf("%d\n", c_inicial->retardo);
}

void liberarMemoriaConfig(config_inicial* c_inicial) {

	free(c_inicial->puerto_safa);
	free(c_inicial->algoritmo);
	free(c_inicial);
}





//CONSOLA

void ejecutar_path_plp( char* path){

	struct_dtb dtb_nuevo = crear_dtb(path);

	pthread_mutex_lock( &mutex_sem_nuevo );

	 queue_push(nuevos,  &dtb_nuevo);

	pthread_cond_signal(&sem_nuevo_vacio);
	pthread_mutex_unlock( &mutex_sem_nuevo );


}


struct_dtb crear_dtb( char *path ){

	struct_dtb dtb_nuevo;
	dtb_nuevo.idDtb =id_total;
	dtb_nuevo.escriptorio = string_duplicate( path );
	dtb_nuevo.inicializado = 1;
	dtb_nuevo.direcciones = NULL;

	id_total++;

	return dtb_nuevo;
}


void status_dtb(long id){

}

void status_colas(){

}

void finalizar_dtb(long id ){

}

void metricas( long id ){

}



