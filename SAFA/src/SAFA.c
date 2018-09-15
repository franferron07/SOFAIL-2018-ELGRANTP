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

//globales
t_log* logger= NULL;
Socket socket_servidor;

pthread_t hilo_dam;


int main(void) {

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

	/* libero memoria de inicializacion  */
	config_destroy(inicializador);




	/* crear socket  INADDR_ANY */
	Socket socket_servidor = crear_socket(  "127.0.0.1" , c_inicial->puerto_safa);
	log_info(logger, "Creo socket %s", "INFO");
	//Asocio el servidor a un puerto
	asociar_puerto(socket_servidor);

	//Escucho Conexiones Entrantes
	escuchar(socket_servidor);

	/*lanzo hilo por cada una de las conexiones aceptadas*/
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

		}

		int conect=0;
		if( Escribe_Socket(socketCliente, (char *)&conect, sizeof(int)) == -1 ) {
			puts( "Error en envio de mensaje" );
			log_info(logger, "Error al enviar mensaje  %s", "INFO");
			exit(EXIT_FAILURE);
		}


		printf("se recibio el id: %d\n",buffer);



		//pthread_create (&idHilo, NULL, (void*)nueva_conexion, &socketCliente);
	}


	cerrar_socket(socket_servidor);

	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);
	log_info(logger, "Libero memoria de configuracion inicial %s", "INFO");

	//rompo logger
	log_destroy(logger);

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
