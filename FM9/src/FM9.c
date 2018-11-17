#include "FM9.h"

/**
 * El proceso memoria cumplira la funcion de disponibilizar
 * los datos requeridos para la ejecución de cualquier G.DT.
 * Cada vez que un G.DT requiera consultar o actualizar información
 * siempre deberá existir previamente en la Memoria.
 * */

int main(int argc, char *argv[]) {
	if (inicializar() < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();
	iniciar_administracion_memoria();
	pthread_create(&hilo_principal, NULL, (void*) iniciar_fm9, NULL);
	pthread_create(&hilo_consola, NULL, (void*) escuchar_consola, NULL);

	pthread_join(hilo_consola, NULL);
	pthread_cancel(hilo_principal);
	liberar_recursos(EXIT_SUCCESS);
	return 0;
}

int inicializar() {
	if (crear_log() == EXIT_FAILURE)
		terminar_exitosamente(EXIT_FAILURE);

	print_header(FM9, fm9_log);

	if (cargar_archivo_config(FILE_CONFIG_FM9) < 0) {
		return -1;
	}

	return 0;
}

void escuchar_consola() {
	log_info(fm9_log, "Se inicio hilo con la consola");

	while (true) {
		if (consola_leer_comando(fm9_log) == CONSOLA_TERMINAR) {
			pthread_exit(0);
			return;
		}
	}
}

void iniciar_fm9() {
	log_info(fm9_log, "Se inicio hilo principal FM9");
	crear_servidor();
	log_info(fm9_log, "Esperando por conexiones entrantes...");
	atender_conexiones();
	pthread_exit(0);
}

void crear_servidor() {
	if (configurar_socket_servidor(&socket_fm9, "127.0.0.1", fm9.puerto,
	TAMANIO_CANT_CLIENTES) < 0) {
		log_error(fm9_log, "No se pudo iniciar el servidor");
		terminar_exitosamente(EXIT_FAILURE);
	}
}

void atender_conexiones() {
	int socket_cliente, *socket_nuevo;
	while ((socket_cliente = aceptar_conexion(socket_fm9))) {
		log_info(fm9_log, "Se agrego una nueva conexión, socket: %d",
				socket_cliente);

		socket_nuevo = malloc(1);
		*socket_nuevo = socket_cliente;
		pthread_create(&hilo_cliente, NULL, (void*) administrar_servidor,
				(void*) &socket_nuevo);
	}
	if (socket_cliente < 0) {
		log_error(fm9_log, "Error al aceptar nueva conexión");
	}
}

void administrar_servidor(void *puntero_fd) {
//	int cliente_socket = *(int *) puntero_fd;
//	mensaje_reconocimiento_type mensaje_reconocimiento;
//	void *buffer_reconocimiento;
//	void *buffer_header = malloc(TAMANIO_HEADER_CONEXION);
//
//	int res = recv(cliente_socket, buffer_header, TAMANIO_HEADER_CONEXION,
//	MSG_WAITALL);
//
//	if (res <= 0) {
//		log_error(fm9_log, "¡Error en el handshake con el cliente!");
//		close(cliente_socket);
//		free(buffer_header);
//	}
//
//	header_conexion = deserializar_header_conexion(buffer_header);
//
//	log_info(fm9_log, "Se realizo handshake del cliente: %s",
//			header_conexion->nombre_instancia);
//
//	strcpy(mensaje_reconocimiento.nombre_instancia, FM9);
//
//	buffer_reconocimiento = serializar_mensaje_reconocimiento(
//			&mensaje_reconocimiento);
//
//	if (send(cliente_socket, buffer_reconocimiento,
//			TAMANIO_MENSAJE_RECONOCIMIENTO, 0)
//			!= TAMANIO_MENSAJE_RECONOCIMIENTO) {
//		log_error(fm9_log, "¡No se pudo devolver el handshake al cliente!");
//		close(cliente_socket);
//	} else {
//		log_info(fm9_log, "El cliente %s se ha conectado correctamente",
//				header_conexion->nombre_instancia);
//	}
//
//	free(buffer_header);
//	free(header_conexion);
//	free(buffer_reconocimiento);
//	free(puntero_fd);

//	int socketActual = *(int*) socket;
//	int datosRecibidos = 0;
//	nombre_paquete* paquete;
//	void* datos;
//	int datosARecibir;
//	while ((datosARecibir = RecibirDatos(&paquete, socketActual,
//			sizeof('Header'))) > 0) {
//		datos = paquete->mensaje;
//		switch (paquete->encabezado.nombre_instancia) {
//		case "CPU": {
//			coordinarCPU(socketActual, datos, paquete);
//		}
//			break;
//		case "DAM": {
//			coordinarDAM(socketActual, datos, paquete);
//		}
//			break;
//		}
//	}
//
//	close(socketActual);
//	sacar_CPU(socketActual);

}

int RecibirDatos(void* paquete, int socketFD, uint32_t cantARecibir) {
	void* datos = malloc(cantARecibir);
	int recibido = 0;
	int totalRecibido = 0;
	do {
		recibido = recv(socketFD, datos + totalRecibido,
				cantARecibir - totalRecibido, 0);
		totalRecibido += recibido;
	} while (totalRecibido != cantARecibir && recibido > 0);
	memcpy(paquete, datos, cantARecibir);
	free(datos);
	if (recibido < 0) {
		printf("Cliente Desconectado\n");
		close(socketFD); // ¡Hasta luego!
	} else if (recibido == 0) {
		printf("Fin de Conexion en socket %d\n", socketFD);
		close(socketFD); // ¡Hasta luego!
	}
	return recibido;
}

void iniciar_administracion_memoria() {
	inicializar_memoria();
	switch (fm9.modo) {
	case SEG:
		iniciar_segmentacion_pura(fm9.tamanio);
		break;
	case SPA:
		iniciar_segmentacion_paginada(fm9.tamanio);
		break;
	case TPI:
		iniciar_paginas_invertidas(fm9.tamanio);
		break;
	}
}

void liberar_recursos(int tipo_salida) {
	print_footer(FM9, fm9_log);
	destruir_archivo_log(fm9_log);
	terminar_exitosamente(tipo_salida);
}

void terminar_exitosamente(int ret_val) {
	if (socket_fm9 != 0)
		close(socket_fm9);
	exit(ret_val);
}

//void coordinarDAM(int socket, void* datos, nombre_paquete* paquete) {
//
//	switch (paquete->encabezado.tipo_operacion) {
//	/**
//	 * INSERTAR, HANDSHAKE, ELIMINAR, ACTUALIZAR, DESCARGAR
//	 * */
//	case "Handshake": {
//
//		int memoria_requerida = *((int*) datos);
//		datos += sizeof(int);
//		char* file_name = malloc(strlen(datos));
//		memcpy(file_name, datos, strlen(datos));
//	}
//		break;
//	case "Lineas a insertar": {
//		//leer cantidad lineas
//		//creo el segmento
//	}
//		break;
//	case "Datos Archivo": {
//		//Inserto la lineas
//	}
//		break;
//	}
//}
