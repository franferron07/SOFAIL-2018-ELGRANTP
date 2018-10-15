#include "SAFA.h"

int main(int argc, char *argv[]) {
	if (inicializar() < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	sem_init(&sem_cpus, 0, 0);
	imprimir_config();

	pthread_mutex_init(&mutex_consola, NULL);
	pthread_mutex_init(&mutex_planificador, NULL);

	pthread_create(&hilo_principal, NULL, (void*) iniciar_safa, NULL);
	pthread_create(&hilo_consola, NULL, (void*) escuchar_consola, NULL);
	pthread_create(&hilo_planificacion, NULL, (void*) ejecutar_planificacion,
	NULL);

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

	inicializar_listas_cpu();

	set_quantum(safa.quantum);
	set_algoritmo(safa.algoritmo);

	return 0;
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

void crear_servidor() {
	if (configurar_socket_servidor(&socket_safa, "127.0.0.1", safa.puerto,
	TAMANIO_CANT_CLIENTES) < 0) {
		log_error(safa_log, "No se pudo iniciar el servidor");
		terminar_exitosamente(EXIT_FAILURE);
	}
}

void atender_conexiones() {
	int socket_cliente, *socket_nuevo;
	while ((socket_cliente = aceptar_conexion(socket_safa))) {
		log_info(safa_log, "Se agrego una nueva conexión, socket: %d",
				socket_cliente);

		socket_nuevo = malloc(1);
		*socket_nuevo = socket_cliente;
		pthread_create(&hilo_cliente, NULL, administrar_servidor,
				(void*) &socket_nuevo);
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
	int res = recv(cliente_socket, buffer_header, TAMANIO_HEADER_CONEXION,
	MSG_WAITALL);

	if (res <= 0) {
		log_error(safa_log, "¡Error en el handshake con el cliente!");
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

	/*************************** SI EL HANDSHAKE LO HIZO UN CPU *********************************/
	if (header_conexion->tipo_instancia == CPU) {
		log_info(safa_log, "************* NUEVO CPU **************");

		int id_dtb = generar_id_dtb();
		dtb_struct * dtb= cread_dtb(id_dtb, cliente_socket);
		agregar_nuevo_dtb(dtb);
	}
	free(buffer_header);
	free(header_conexion);
	free(buffer_reconocimiento);
	free(puntero_fd);

	return 0;
}

void ejecutar_planificacion() {
	while (true) {
		sem_wait(&sem_cpus);
		aplicar_algoritmo_planificacion();
	}

}

int generar_id_dtb() {
	return id_dtb++;
}

void aplicar_algoritmo_planificacion() {
	switch (safa.algoritmo) {
	case RR:
		aplicarRR(false);
		break;
	case VRR:
		aplicarRR(true);
		break;
	case PROPIO:
		aplicarPropio();
		break;
	}
}

void liberar_recursos(int tipo_salida) {
	print_footer(SAFA, safa_log);
	pthread_mutex_destroy(&mutex_consola);
	pthread_mutex_destroy(&mutex_planificador);

	sem_destroy(&sem_cpus);
	liberar_recursos_cpu();
	destruir_archivo_log(safa_log);
	terminar_exitosamente(tipo_salida);
}

void terminar_exitosamente(int ret_val) {
	if (socket_safa != 0)
		close(socket_safa);
	exit(ret_val);
}

//int main(void) {
//	dtb_dummy.inicializado = 0;
//	//CREO COLA DE ESTADOS
//	nuevos = queue_create();
//	listos = queue_create();
//	ejecucion = queue_create();
//	bloqueados = queue_create();
//	terminados = queue_create();
//
//	//inicio semaforos
//	sem_init(&sem_nuevo_mutex, 0, 1);
//	sem_init(&sem_nuevo_vacio, 0, 0);
//	sem_init(&sem_dtb_dummy_mutex, 0, 1);
//	sem_init(&sem_listo_mutex, 0, 1);
//	sem_init(&sem_listo_vacio, 0, 0);
//	sem_init(&sem_listo_max, 0, c_inicial->multiprogramacion);
//	sem_init(&sem_cpu_mutex, 0, 1);
//
//	int buffer;
//
//	pthread_create(&hilo_consola, NULL, (void*) consolaSafa, NULL);
//	pthread_create(&hilo_plp, NULL, (void*) plp, NULL);
//	pthread_create(&hilo_pcp, NULL, (void*) pcp, NULL);


//	pthread_join(hilo_plp, NULL);
//	pthread_join(hilo_pcp, NULL);
//	pthread_join(hilo_consola, NULL);
//	//destruyo semaforos
//	sem_destroy(&sem_nuevo_vacio);
//	sem_destroy(&sem_nuevo_mutex);
//	sem_destroy(&sem_dtb_dummy_mutex);
//	sem_destroy(&sem_listo_mutex);
//	sem_destroy(&sem_listo_vacio);
//	sem_destroy(&sem_listo_max);
//	sem_destroy(&sem_cpu_mutex);
//
//	return EXIT_SUCCESS;
//}
//
////hilo de conexion con dam, debera quedarse a la espera
//void conexion_dam(void* socket) {
//
//	while (1) {
//
//		printf("conexion dam correcta");
//
//	}
//
//}
//
//void conexion_cpu(void* socket) {
//
//	//inicializo struct cpu y lo pongo en list cpu
//	struct_cpu *cpu;
//	cpu = crear_struct_cpu((int) *socket);
//
//	//pongo struct cpu en la cola de cpus para esperar dtbs  a ejecutar
//	sem_wait(&sem_cpu_mutex);
//
//	queue_push(cpus, cpu);
//
//	sem_post(&sem_cpu_mutex);
//
//	while (1) {
//
//		//espero que pcp haga un post para comenzar ejecucion
//		sem_wait(&cpu->sem_mutex_ejecucion_cpu);
//
//		//aca va a comenzar a hablar con cpu real para pasarle los datos del dtb
//		printf("conexion cpu correcta");
//
//		sem_post(&cpu->sem_mutex_ejecucion_cpu);
//
//	}
//
//	pthread_detach(pthread_self()); //libera recursos del hilo
//	pthread_exit(NULL);
//
//}
//
//
//void plp() {
//
//	struct_dtb *dtb;
//
//	while (1) {
//
//		//controlo lista no vacia
//		sem_wait(&sem_nuevo_vacio);
//
//		//Tomo de nuevos dtb
//		sem_wait(&sem_nuevo_mutex);
//
//		dtb = queue_pop(nuevos);
//		sem_post(&sem_nuevo_mutex);
//
//		//tomo dtb dummy y lo inicializo
//		sem_wait(&sem_dtb_dummy_mutex);
//
//		dtb_dummy.idDtb = dtb->idDtb;
//		dtb_dummy.escriptorio = string_duplicate(dtb->escriptorio);
//
//		//agrego a cola de listos el dummy verificando multiprogramacion
//		sem_wait(&sem_listo_max);
//
//		sem_wait(&sem_listo_mutex);
//		queue_push(listos, &dtb_dummy);
//		sem_post(&sem_listo_mutex);
//
//		sem_post(&sem_listo_vacio);
//
//	}
//
//}
//
//void pcp() {
//
//	struct_dtb *dtb = NULL;
//	struct_cpu *cpu_libre = NULL;
//
//	while (1) {
//
//		sem_wait(&sem_listo_vacio);
//
//		//obtengo dtb segun algoritmo
//		dtb = obtener_proximo_dtb(c_inicial->algoritmo);
//
//		//pasarlo a ejecucion de la cpu libre
//		cpu_libre = obtener_cpu_libre();
//		if (*cpu_libre != NULL) {
//			cpu_libre->dtb_ejecutar(dtb);
//			sem_post(&cpu_libre->sem_mutex_ejecucion_cpu); //aviso a hilo cpu que puede comenzar con la ejecucion del dtb
//		}
//
//		//estas ejecuciones tendran que hacerlo los algoritmos
//		sem_wait(&sem_listo_mutex);
//		dtb = queue_pop(listos);
//		sem_post(&sem_listo_mutex);
//
//		sem_post(&sem_listo_max);
//
//		//segun algoritmo
//
//	}
