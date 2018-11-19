#ifndef PROTOCOLOS_H_
#define PROTOCOLOS_H_

#include "serialize.h"
#include "socket.h"
#include <commons/log.h>
#include <commons/collections/list.h>

typedef enum {
	CPU = 1, DAM = 2, SAFA = 3, FM9 = 4, MDJ = 5
} tipo_instancia_e;

typedef enum {
	HANDSHAKE = 0,
	ABRIR = 1,
	CONCENTRAR = 2,
	ASIGNAR = 3,
	WAIT = 4,
	SIGNAL = 5,
	FLUSH = 6,
	CLOSE = 7,
	CREAR = 8,
	BORRAR = 9,

	/**
	 * Operacion entre DAM/FM9
	 * */
	INSERTAR = 29, /*fm9 while hasta que no termine, terminar de cargar todo, fm9->dam me tenes que decir la direccion de donde empieza*/
	DESCARGAR = 31, /*dam->fm9 direccion de donde esta, me tenes que ir mandando las lineas hasta donde termina*/

	/**
	 * Operacion entre DAM/FM9
	 * */
	ACTUALIZAR = 32,
	ELIMINAR = 33,

	/*****ERRORES POSIBLES TODO:hay errores que se repiten pero cambia el codigo. Decidir con grupo que hacer. *****/
	PATHINEXISTENTE = 10001,
	ESPACIOINSUFICIENTEFM9 = 10002,
	ARCHIVOSINABRIR = 20001,
	FALLODEMEMORIA = 20002,
	ESPACIOINSUFICIENTEMDJ = 30003,
	ARCHIVONOEXISTE = 30004,
	ARCHIVOYAEXISTENTE = 50001,

} tipo_operacion_e;

//typedef enum {
//	INSERTAR = 29, /*fm9 while hasta que no termine, terminar de cargar todo, fm9->dam me tenes que decir la direccion de donde empieza*/
//	DESCARGAR = 31 /*dam->fm9 direccion de donde esta, me tenes que ir mandando las lineas hasta donde termina*/
//
//} tipo_operacion_dam_fm9;

typedef enum {
	ENVIARDTB = 10,
	CERRARCONEXION = 11,
	BLOQUEARDTB = 12,
	TERMINARDTB = 13,
	FINDEQUANTUM = 14,
	PEDIRRECURSO = 15,
	LIBERARRECURSO = 16,
	CERRARARCHIVO = 17,
	QUANTUMEJECUTADO = 18
} tipo_operacion_cpu_safa;

typedef enum {
	ARCHIVOCARGADO = 20,
	ARCHIVOCREADO = 21,
	ARCHIVOMODIFICADO = 22,
	ARCHIVOBORRADO = 23
} tipo_operacion_dam_safa;

typedef enum {
	CREAR_ARCHIVO = 24,
	BORRAR_ARCHIVO = 25,
	OBTENER_DATOS = 26,
	GUARDAR_DATOS = 27,
} tipo_operacion_dam_mdj;

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

/**
 *  NUEVOIOOOO
 * */

typedef struct {
	int pid;
	int flag_fin_linea;
	char *linea;
} linea_struct;

typedef struct {
	tipo_instancia_e tipo_instancia;
	int tamanio_mensaje;
	int tipo_operacion;
}__attribute__ ((__packed__)) header_paquete;

typedef struct {
	header_paquete* encabezado;
	void* mensaje;
} paquete_struct;

/**
 * NUEVOIOOOO
 * */

typedef enum {
	NUEVO = 1, LISTO = 2, EJECUTANDO = 3, BLOQUEADO = 4, CARGANDODUMMY = 5
} tipo_estado_dtb;

//TODO: discutir si estas estructuras pertenecen o no al DTB para la muestra de las metricas
//analizar alternativas
//comentado pq rompe la serializacion, falta serializar la struct o no se vemos
//uint8_t sentencias_espera;
//uint8_t sentencias_dam;
//uint8_t sentencias_totales;

typedef struct {
	uint8_t id_dtb;
	char *escriptorio;
	uint8_t program_counter;
	uint8_t inicializado;
	uint8_t quantum;
	tipo_estado_dtb estado;
	t_list *direcciones;
} dtb_struct;

typedef struct {
	int pid;
	char* ruta_archivo;
} operacion_archivo;

typedef struct {
	char* ruta_archivo;
} operacion_archivo_mdj;

typedef struct {
	int pid;
	int cant_lineas;
	char* ruta_archivo;
} operacion_crear;

typedef struct {
	int cant_lineas;
	char* ruta_archivo;
} operacion_crear_mdj;

typedef struct {
	int pid;
	char* ruta_archivo;
	direccion_struct* direccion;
} operacion_flush;

typedef struct {
	int pid;
	direccion_struct* direccion;
} operacion_archivo_direccion;

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

void myMemCpy(void *dest, void *src, size_t n);
void* serializar_dtb(dtb_struct *dtb, int * tamanio_buffer);
dtb_struct* deserializar_dtb(void *buffer);

int tamanio_dtb(dtb_struct *dtb);

void* serializar_linea_struct(linea_struct *linea,int max_linea, int* tam_buffer);
linea_struct* deserializar_linea_struct(void* buffer,int max_linea);

void* serializar_operacion_archivo(operacion_archivo* struct_archivo,
		int* tamanio_buffer);
operacion_archivo* deserializar_operacion_archivo(void *buffer);

void* serializar_operacion_archivo_direccion(
		operacion_archivo_direccion* struct_archivo, int* tamanio_buffer);

operacion_archivo_direccion* deserializar_operacion_archivo_direccion(
		void *buffer);
void* serializar_operacion_archivo_mdj(operacion_archivo_mdj* struct_archivo,
		int* tamanio_buffer);
operacion_archivo_mdj* deserializar_operacion_archivo_mdj(void *buffer);
void* serializar_operacion_crear(operacion_crear* struct_archivo_crear,
		int* tamanio_buffer);
operacion_crear* deserializar_operacion_crear(void *buffer);
void* serializar_operacion_crear_mdj(operacion_crear_mdj* struct_archivo_crear,
		int* tamanio_buffer);
operacion_crear_mdj* deserializar_operacion_crear_mdj(void *buffer);

#endif /* PROTOCOLOS_H_ */
