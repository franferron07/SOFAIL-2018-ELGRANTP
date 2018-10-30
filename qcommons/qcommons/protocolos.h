#ifndef PROTOCOLOS_H_
#define PROTOCOLOS_H_

#include "serialize.h"
#include "socket.h"
#include <commons/log.h>

typedef enum {
	CPU = 1, DAM = 2, SAFA = 3, FM9 = 4, MDJ = 5
} tipo_instancia_e;

typedef enum {
	ABRIR = 1,
	CONCENTRAR = 2,
	ASIGNAR = 3,
	WAIT = 4,
	SIGNAL = 5,
	FLUSH = 6,
	CLOSE = 7,
	CREAR = 8,
	BORRAR = 9,
	ENVIARDTB = 10,
	CERRARCONEXION= 11
} tipo_operacion_e;

typedef struct {
	char nombre_instancia[31];
	tipo_instancia_e tipo_instancia;
} header_conexion_type;

static const int TAMANIO_HEADER_CONEXION = 31 + 4;

typedef struct {
	char nombre_instancia[31];
} mensaje_reconocimiento_type;

static const int TAMANIO_MENSAJE_RECONOCIMIENTO = 31;

typedef struct {
	tipo_operacion_e tipo_operacion;
	char clave[41];
	unsigned int tamanio_payload;
} request_operacion_type;

static const int TAMANIO_REQUEST_OPERACION = 4 + 41 + 4;


typedef struct {
	char *path;
	char *direccion;
} direccion_struct;


typedef struct {
	uint8_t id_dtb;
	char *escriptorio;
	uint8_t program_counter;
	uint8_t inicializado;
	uint8_t quantum;
	//char** direcciones;
} dtb_struct;



int enviar_header_conexion(int socket_server, char* nombre_instancia,
		tipo_instancia_e tipo_instancia, t_log *logger);

int esperar_reconocimiento(int socket_servidor, t_log *logger);

int ejecutar_handshake(int socket_servidor, char* nombre_instancia,
		tipo_instancia_e tipo_instancia, t_log *logger);

void* serializar_header_conexion(header_conexion_type *header);

header_conexion_type* deserializar_header_conexion(void *buffer);

void* serializar_mensaje_reconocimiento(
		mensaje_reconocimiento_type *mensaje_reconocimiento);

mensaje_reconocimiento_type* deserializar_mensaje_reconocimiento(void* buffer);

void* serializar_request_operacion_(request_operacion_type *request);

request_operacion_type* deserializar_request_operacion(void *buffer);


void* serializar_dtb(dtb_struct *dtb_a_enviar);
dtb_struct* deserializar_dtb(void *buffer);


#endif /* PROTOCOLOS_H_ */
