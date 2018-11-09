#include "SAFA.h"

int main(int argc, char *argv[]) {
	if (inicializar() < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();


	pthread_create(&hilo_principal, NULL, (void*) iniciar_safa, NULL);

	verificar_estado();

	pthread_create(&hilo_consola, NULL, (void*) escuchar_consola, NULL);
	pthread_create(&hilo_planificacion, NULL, (void*) ejecutar_planificacion,NULL);
	pthread_create(&hilo_plp, NULL, (void*) ejecutar_planificacion_largo_plazo, NULL);

	pthread_join(hilo_plp, NULL);
	pthread_join(hilo_consola, NULL);
	pthread_cancel(hilo_principal);
	pthread_cancel(hilo_planificacion);
	liberar_recursos(EXIT_SUCCESS);
	return 0;
}

int inicializar() {
	if (crear_log() == EXIT_FAILURE)
		terminar_exitosamente(EXIT_FAILURE);

	print_header(SAFA, safa_log);

	if (cargar_archivo_config(FILE_CONFIG_SAFA) < 0) {
		log_error(safa_log, "No se encontró el archivo de configuración\n");
		return -1;
	}

	inicializar_listas_dtb();
	inicializar_lista_cpus();
	inicializar_semaforos();


	dtb_dummy.inicializado = 0;
	dtb_dummy.id_dtb = -1;
	dtb_dummy.quantum = safa.quantum;

	set_quantum(safa.quantum);
	set_algoritmo(safa.algoritmo);

	return 0;
}


void inicializar_semaforos(){

	sem_init(&sem_listo_max, 0, safa.multiprogramacion);

	pthread_mutex_init(&sem_dtb_dummy_mutex, NULL);
	pthread_mutex_init(&sem_listo_mutex, NULL);
	pthread_mutex_init(&sem_cpu_mutex, NULL);
	pthread_mutex_init(&sem_nuevo_mutex, NULL);
}

void escuchar_consola() {
	log_info(safa_log, "Se inicio hilo con la consola");

	while (true) {
		if (consola_leer_comando(safa_log) == CONSOLA_TERMINAR) {
			pthread_exit(0);
			return;
		}
	}
}

void iniciar_safa() {
	log_info(safa_log, "Se inicio hilo principal SAFA");
	crear_servidor();
	log_info(safa_log, "Esperando por conexiones entrantes...");
	atender_conexiones();
	pthread_exit(0);
}


void verificar_estado(){

	while( status_safa == CORRUPTO ){

		if( puede_iniciar_safa() == true ){
			status_safa= INICIALIZADO;
		}

	}

}

bool puede_iniciar_safa(){

	if( cpu_conectado ==true && dam_conectado == true ) return true;
	return false;
}

void crear_servidor() {
	if (configurar_socket_servidor(&socket_safa, "127.0.0.1", safa.puerto,
	TAMANIO_CANT_CLIENTES) < 0) {
		log_error(safa_log, "No se pudo iniciar el servidor");
		terminar_exitosamente(EXIT_FAILURE);
	}
}

void atender_conexiones() {
	int socket_cliente;
	while ((socket_cliente = aceptar_conexion(socket_safa))) {
		log_info(safa_log, "Se agrego una nueva conexión, socket: %d",
				socket_cliente);

		pthread_create(&hilo_cliente, NULL, administrar_servidor,(void*) &socket_cliente);
	}
	if (socket_cliente < 0) {
		log_error(safa_log, "Error al aceptar nueva conexión");
	}
}

void *administrar_servidor(void *puntero_fd) {
	int cliente_socket = *(int *) puntero_fd;
	header_conexion_type *header_conexion = NULL;
	mensaje_reconocimiento_type mensaje_reconocimiento;
	void *buffer_reconocimiento;
	void *buffer_header = malloc(TAMANIO_HEADER_CONEXION);

	/************ LEER EL HANDSHAKE ************/
	int res = recv(cliente_socket, buffer_header, TAMANIO_HEADER_CONEXION,MSG_WAITALL);

	if (res <= 0) {
		log_error(safa_log, "¡Error en el handshake con el cliente! %d",res);
		close(cliente_socket);
		free(buffer_header);
	}

	header_conexion = deserializar_header_conexion(buffer_header);

	log_info(safa_log, "Se realizo handshake del cliente: %s",
			header_conexion->nombre_instancia);

	/************ RESPONDER AL HANDSHAKE ************/
	strcpy(mensaje_reconocimiento.nombre_instancia, SAFA);

	buffer_reconocimiento = serializar_mensaje_reconocimiento(
			&mensaje_reconocimiento);

	if (send(cliente_socket, buffer_reconocimiento,
			TAMANIO_MENSAJE_RECONOCIMIENTO, 0)
			!= TAMANIO_MENSAJE_RECONOCIMIENTO) {
		log_error(safa_log, "¡No se pudo devolver el handshake al cliente!");
		close(cliente_socket);
	} else {
		log_info(safa_log, "El cliente %s se ha conectado correctamente",
				header_conexion->nombre_instancia);
	}

	/*************************** SI EL HANDSHAKE LO HIZO UN CPU  *********************************/
	if (header_conexion->tipo_instancia == CPU) {
		log_info(safa_log, "************* NUEVO CPU **************");

		if(status_safa == CORRUPTO){
			cpu_conectado=true;
		}

		atender_cliente_cpu( &cliente_socket );
	}

	/************************** SI EL HANDSHAKE LO HIZO DAM ***************************************/
	if( header_conexion->tipo_instancia == DAM ){
		log_info(safa_log, "************* NUEVO DAM **************");

		if(status_safa == CORRUPTO){
			dam_conectado=true;
		}

		atender_cliente_dam( &cliente_socket );
	}

	free(buffer_header);
	free(header_conexion);
	free(buffer_reconocimiento);
	//free(puntero_fd);

	return 0;
}


void atender_cliente_cpu( int *cliente_socket ){

	request_operacion_type *header_operacion = NULL;
	void *buffer_operacion = malloc(TAMANIO_REQUEST_OPERACION);
	int res ;

	cpu_struct cpu_nueva;

	cpu_nueva = crear_cpu(*cliente_socket);
	/* AGREGO CPU EN LISTA */
	pthread_mutex_lock(&sem_cpu_mutex);
		list_add(cpus, &cpu_nueva);
	pthread_mutex_unlock(&sem_cpu_mutex);

	/*
	res = recv(*cliente_socket, buffer_operacion, TAMANIO_REQUEST_OPERACION,MSG_WAITALL);

	if (res <= 0) {
		log_error(safa_log, "¡Error en el mensaje con CPU!");
		close(*cliente_socket);
		free(buffer_operacion);
	}*/


	/****** ESPERANDO MENSAJES DE CPU *******/
	while ( ( res = recv(*cliente_socket, buffer_operacion, TAMANIO_REQUEST_OPERACION,MSG_WAITALL) )  > 0) {

		header_operacion = deserializar_request_operacion(buffer_operacion);
		log_info(safa_log, "Se recibio operacion del CPU: %s",header_operacion->tipo_operacion);

		switch (header_operacion->tipo_operacion ) {

		case ENVIARDTB:{

			/* espero hasta que cpu tenga un dtb para ejecutar.  */
			while( cpu_nueva.dtb_ejecutar == NULL ){

			}

			/***** ENVIO A CPU DTB A EJECUTAR  ******/
			int tamanio_buffer=0;
			void *buffer = serializar_dtb( cpu_nueva.dtb_ejecutar , &tamanio_buffer);
			send(cpu_nueva.socket, buffer, tamanio_dtb( cpu_nueva.dtb_ejecutar ) , 0);
			log_info(safa_log, "Envio a ejecutar en cpu el dtb: %d",cpu_nueva.dtb_ejecutar->id_dtb);
		}
		break;

		case CERRARCONEXION:{

		}
		break;

		case BLOQUEARDTB:{

			/* TODO: verificar que el id recibido sea el que esta en el dummy, si lo es debe reiniciarse el dummy para que pueda volver a ser inicializado
			 * si no lo es mandar a lista de bloqueados el dtb.
			 *
			 *   */



		}
		break;

		case TERMINARDTB:{

		}
		break;

		case FINDEQUANTUM:{

		}
		break;

		case PEDIRRECURSO:{

			recurso_struct *recurso =NULL;

			recurso = buscar_recurso( "nombre_recurso" );
			if( recurso == NULL ){

				recurso = crear_recurso( "nombre_recurso" );
			}

			asignar_recurso( recurso );

		}
		break;

		case LIBERARRECURSO:{

			recurso_struct *recurso =NULL;

			recurso = buscar_recurso( "nombre_recurso" );
			if( recurso == NULL ){

				recurso = crear_recurso( "nombre_recurso" );
			}

			liberar_recurso( recurso );

		}
		break;

		case QUANTUMEJECUTADO:{


		}
		break;

		default:

		break;

		}



		if (header_operacion->clave != NULL){
			free(header_operacion->clave );
		}

	}


	pthread_detach(pthread_self()); //libera recursos del hilo
	pthread_exit(NULL);

}


void atender_cliente_dam( int *cliente_socket ){


	request_operacion_type *header_operacion = NULL;
	void *buffer_operacion = malloc(TAMANIO_REQUEST_OPERACION);
	int res ;


	/****** ESPERANDO MENSAJES DE DAM *******/
	while ( ( res = recv(*cliente_socket, buffer_operacion, TAMANIO_REQUEST_OPERACION,MSG_WAITALL) )  > 0) {

		switch (header_operacion->tipo_operacion ) {

			case ARCHIVOCARGADO:{

				/*TODO se realiza otro recv para recibir el path del
				 * archivo cargado el id de dtb y en donde se cargo. si salio ok se debe desbloquear dtb y sino cerrarlo-*/

				/* TODO verificar si el id del dtb corresponde a un dtb en estado cargandoDUmmy si lo es debe pasarse el id del dtb original
				 * de la cola de nuevos a listos y cambiar su estado . Y si no esta en cargandodummy se manda a lista de bloqueados.
				 * */


			}
			break;

			case ARCHIVOCREADO:{

				/*TODO se realiza otro recv con el id del dtb que hizo el abrir, se debe desbloquear el dtb si salio todo ok */

			}
			break;

			case ARCHIVOMODIFICADO:{

			}
			break;

			case ARCHIVOBORRADO:{

			}
			break;

			default:
			break;

		}

	}



}



void liberar_recursos(int tipo_salida) {
	print_footer(SAFA, safa_log);

	pthread_mutex_destroy(&sem_nuevo_mutex);
	pthread_mutex_destroy(&sem_dtb_dummy_mutex);
	pthread_mutex_destroy(&sem_listo_mutex);
	pthread_mutex_destroy(&sem_cpu_mutex);
	sem_destroy(&sem_listo_max);

	liberar_recursos_dtb();
	destruir_archivo_log(safa_log);
	terminar_exitosamente(tipo_salida);
}

void terminar_exitosamente(int ret_val) {
	if (socket_safa != 0)
		close(socket_safa);
	exit(ret_val);
}

void escuchar_dam() {
	while (true) {
		printf("conexion dam correcta");

	}
}








