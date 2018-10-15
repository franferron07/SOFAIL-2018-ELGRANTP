#include "protocolos.h"

int enviar_header_conexion(int socket_server, char* nombre_instancia,
		tipo_instancia_e tipo_instancia, t_log *logger) {

	header_conexion_type *header_conexion = malloc(
			sizeof(header_conexion_type));
	strcpy(header_conexion->nombre_instancia, nombre_instancia);
	header_conexion->tipo_instancia = tipo_instancia;

	char *buffer = serializar_header_conexion(header_conexion);

	log_trace(logger, "Enviando mensaje de header de conexión...");
	int result = send(socket_server, buffer, TAMANIO_HEADER_CONEXION, 0);

	free(buffer);
	free(header_conexion);

	if (result <= 0) {
		log_error(logger, "No se pudo enviar header de conexión al servidor.");
		return -1;
	}

	return 0;
}

int esperar_reconocimiento(int socket_servidor, t_log *logger) {
	void* buffer_reconocimiento = malloc(TAMANIO_MENSAJE_RECONOCIMIENTO);

	if (recv(socket_servidor, buffer_reconocimiento,
			TAMANIO_MENSAJE_RECONOCIMIENTO, MSG_WAITALL) <= 0) {
		log_error(logger, "Erro recibiendo respuesta de handshake.");
		return -1;
	}

	mensaje_reconocimiento_type *mensaje_reconocimiento =
			deserializar_mensaje_reconocimiento(buffer_reconocimiento);

	log_info(logger, "Se realizo handshake con el servidor: %s.",
			mensaje_reconocimiento->nombre_instancia);

	free(buffer_reconocimiento);
	free(mensaje_reconocimiento);

	return 0;
}

int ejecutar_handshake(int socket_servidor, char* nombre_instancia,
		tipo_instancia_e tipo_instancia, t_log *logger) {

	if (!enviar_header_conexion(socket_servidor, nombre_instancia,
			tipo_instancia, logger)) {
		return false;
	}

	log_trace(logger, "Handshake ejecutado. Esperando respuesta...");

	if (!esperar_reconocimiento(socket_servidor, logger)) {
		log_error(logger, "El handshake falló!");
		return -1;

	}
	log_info(logger, "Handshake realizado exitosamente.");
	return 0;

}

void* serializar_header_conexion(header_conexion_type *header) {
	void* buffer = malloc(TAMANIO_HEADER_CONEXION);
	int lastIndex = 0;
	int tipo_instancia = header->tipo_instancia;

	serialize_data(&tipo_instancia, 4, &buffer, &lastIndex);
	serialize_data(&(header->nombre_instancia), 31, &buffer, &lastIndex);

	return buffer;
}

header_conexion_type* deserializar_header_conexion(void *buffer) {
	header_conexion_type* header = malloc(sizeof(header_conexion_type));
	int lastIndex = 0;
	int tipo_instancia = 0;

	deserialize_data(&tipo_instancia, 4, buffer, &lastIndex);
	header->tipo_instancia = tipo_instancia;
	deserialize_data(&(header->nombre_instancia), 31, buffer, &lastIndex);

	return header;
}

void* serializar_mensaje_reconocimiento(mensaje_reconocimiento_type *mensaje_reconocimiento) {
	void* buffer = malloc(TAMANIO_MENSAJE_RECONOCIMIENTO);
	int lastIndex = 0;

	serialize_data(&(mensaje_reconocimiento->nombre_instancia), 31, &buffer, &lastIndex);

	return buffer;
}

mensaje_reconocimiento_type* deserializar_mensaje_reconocimiento(void* buffer) {
	mensaje_reconocimiento_type* mensaje = malloc(sizeof(mensaje_reconocimiento_type));
	int lastIndex = 0;

	deserialize_data(&(mensaje->nombre_instancia), 31, buffer, &lastIndex);

	return mensaje;
}

void* serializar_request_operacion_(request_operacion_type *request) {
	void* buffer = malloc(TAMANIO_REQUEST_OPERACION);
	int lastIndex = 0;

	serialize_data(&(request->tipo_operacion), 4, &buffer, &lastIndex);
	serialize_data(&(request->clave), 41, &buffer, &lastIndex);
	serialize_data(&(request->tamanio_payload), 4, &buffer, &lastIndex);

	return buffer;
}

request_operacion_type* deserializar_request_operacion(void *buffer) {
	request_operacion_type* request = malloc(sizeof(request_operacion_type));
	int lastIndex = 0;

	deserialize_data(&(request->tipo_operacion), 4, buffer, &lastIndex);
	deserialize_data(&(request->clave), 41, buffer, &lastIndex);
	deserialize_data(&(request->tamanio_payload), 4, buffer, &lastIndex);

	return request;
}