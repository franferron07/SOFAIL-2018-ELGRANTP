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

void myMemCpy(void *dest, void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;

   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}

void* serializar_operacion_archivo(operacion_archivo* struct_archivo, int* tamanio_buffer)
{
	int tamanio_path = strlen(struct_archivo->ruta_archivo);
	int TAMANIO_OPERACION_ARCHIVO = sizeof(struct_archivo->pid)+sizeof(tamanio_path)+tamanio_path;

	void* buffer = malloc(TAMANIO_OPERACION_ARCHIVO);
	int lastIndex = 0;

	memcpy(buffer, &(struct_archivo->pid), sizeof(struct_archivo->pid));
	lastIndex += sizeof(struct_archivo->pid);
	memcpy(buffer+lastIndex, &(tamanio_path), sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);
	memcpy(buffer+lastIndex, &(struct_archivo->ruta_archivo),tamanio_path);

	*tamanio_buffer = lastIndex;

	return buffer;
}

operacion_archivo* deserializar_operacion_archivo(void *buffer) {

	operacion_archivo* struct_archivo = malloc(sizeof(operacion_archivo));
	int tamanio_path;

	int lastIndex = 0;

	memcpy(&(struct_archivo->pid), buffer, sizeof(struct_archivo->pid));
	lastIndex += sizeof(struct_archivo->pid);
	memcpy(&(tamanio_path), buffer+lastIndex, sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);

	struct_archivo->ruta_archivo = malloc(tamanio_path+1);
	myMemCpy(&(struct_archivo->ruta_archivo), buffer+lastIndex, tamanio_path);
	struct_archivo->ruta_archivo[tamanio_path] = '\0';
	return struct_archivo;
}

void* serializar_operacion_archivo_mdj(operacion_archivo_mdj* struct_archivo, int* tamanio_buffer)
{
	int tamanio_path = strlen(struct_archivo->ruta_archivo);
	int TAMANIO_OPERACION_ARCHIVO = sizeof(tamanio_path)+tamanio_path;

	void* buffer = malloc(TAMANIO_OPERACION_ARCHIVO);
	int lastIndex = 0;

	memcpy(buffer+lastIndex, &(tamanio_path), sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);
	memcpy(buffer+lastIndex, &(struct_archivo->ruta_archivo),tamanio_path);

	*tamanio_buffer = lastIndex;

	return buffer;
}

operacion_archivo_mdj* deserializar_operacion_archivo_mdj(void *buffer) {

	operacion_archivo_mdj* struct_archivo = malloc(sizeof(operacion_archivo_mdj));
	int tamanio_path;

	int lastIndex = 0;

	memcpy(&(tamanio_path), buffer+lastIndex, sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);

	struct_archivo->ruta_archivo = malloc(tamanio_path+1);
	myMemCpy(&(struct_archivo->ruta_archivo), buffer+lastIndex, tamanio_path);
	struct_archivo->ruta_archivo[tamanio_path] = '\0';
	return struct_archivo;
}

void* serializar_operacion_crear(operacion_crear* struct_archivo_crear, int* tamanio_buffer)
{
	int tamanio_path = strlen(struct_archivo_crear->ruta_archivo);
	int TAMANIO_OPERACION_ARCHIVO = sizeof(struct_archivo_crear->pid)+sizeof(struct_archivo_crear->cant_lineas)+sizeof(tamanio_path)+tamanio_path;

	void* buffer = malloc(TAMANIO_OPERACION_ARCHIVO);
	int lastIndex = 0;

	memcpy(buffer, &(struct_archivo_crear->pid), sizeof(struct_archivo_crear->pid));
	lastIndex += sizeof(struct_archivo_crear->pid);
	memcpy(buffer, &(struct_archivo_crear->cant_lineas), sizeof(struct_archivo_crear->cant_lineas));
	lastIndex += sizeof(struct_archivo_crear->cant_lineas);
	memcpy(buffer+lastIndex, &(tamanio_path), sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);
	memcpy(buffer+lastIndex, &(struct_archivo_crear->ruta_archivo),tamanio_path);

	*tamanio_buffer = lastIndex;

	return buffer;
}

operacion_crear* deserializar_operacion_crear(void *buffer) {

	operacion_crear* struct_archivo = malloc(sizeof(operacion_crear));
	int tamanio_path;

	int lastIndex = 0;

	memcpy(&(struct_archivo->pid), buffer, sizeof(struct_archivo->pid));
	lastIndex += sizeof(struct_archivo->pid);
	memcpy(&(struct_archivo->cant_lineas), buffer, sizeof(struct_archivo->cant_lineas));
	lastIndex += sizeof(struct_archivo->cant_lineas);
	memcpy(&(tamanio_path), buffer+lastIndex, sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);

	struct_archivo->ruta_archivo = malloc(tamanio_path+1);
	myMemCpy(&(struct_archivo->ruta_archivo), buffer+lastIndex, tamanio_path);
	struct_archivo->ruta_archivo[tamanio_path] = '\0';

	return struct_archivo;
}

void* serializar_operacion_crear_mdj(operacion_crear_mdj* struct_archivo_crear, int* tamanio_buffer)
{
	int tamanio_path = strlen(struct_archivo_crear->ruta_archivo);
	int TAMANIO_OPERACION_ARCHIVO = sizeof(struct_archivo_crear->cant_lineas)+sizeof(tamanio_path)+tamanio_path;

	void* buffer = malloc(TAMANIO_OPERACION_ARCHIVO);
	int lastIndex = 0;

	memcpy(buffer, &(struct_archivo_crear->cant_lineas), sizeof(struct_archivo_crear->cant_lineas));
	lastIndex += sizeof(struct_archivo_crear->cant_lineas);
	memcpy(buffer+lastIndex, &(tamanio_path), sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);
	memcpy(buffer+lastIndex, &(struct_archivo_crear->ruta_archivo),tamanio_path);

	*tamanio_buffer = lastIndex;

	return buffer;
}

operacion_crear_mdj* deserializar_operacion_crear_mdj(void *buffer) {

	operacion_crear_mdj* struct_archivo = malloc(sizeof(operacion_crear_mdj));
	int tamanio_path;

	int lastIndex = 0;


	memcpy(&(struct_archivo->cant_lineas), buffer, sizeof(struct_archivo->cant_lineas));
	lastIndex += sizeof(struct_archivo->cant_lineas);
	memcpy(&(tamanio_path), buffer+lastIndex, sizeof(tamanio_path));
	lastIndex += sizeof(tamanio_path);

	struct_archivo->ruta_archivo = malloc(tamanio_path+1);
	myMemCpy(&(struct_archivo->ruta_archivo), buffer+lastIndex, tamanio_path);
	struct_archivo->ruta_archivo[tamanio_path] = '\0';

	return struct_archivo;
}
//////////////////////////////////////////
///////INTENTO DE SERIALIZO DE DTB/////////
//////////////////////////////////////////


int tamanio_lista_para_buffer(t_list *direcciones){
	//retorna el tamanio de la lista de direcciones para el buffer
	int tamanio = 	sizeof((uint8_t)direcciones->elements_count);
	uint8_t tamanio_ruta = 0;
	uint8_t tamanio_direccion = 0;

	t_link_element *element = direcciones->head;
	t_link_element *aux = NULL;

	while (element != NULL) {
		aux = element->next;

		direccion_struct* direcciones = element->data;

		tamanio_ruta = strlen(direcciones->path);
		tamanio += (sizeof(tamanio_ruta) + tamanio_ruta);

		tamanio_direccion = strlen(direcciones->direccion);
		tamanio += (sizeof(tamanio_direccion) + tamanio_direccion);


		element = aux;
	}

	return tamanio;
}


void* serializar_dtb(dtb_struct *dtb, int * tamanio_buffer){
	//funcion que serializa un dtb
	//recibe como parametro un puntero al dtb a serializar
	//y un puntero a un int que va a ser el tamanio del buffer despues de serializar
	//bastante util al momento de hacer un send(dtb)
	uint8_t cantidad_elementos;
	uint8_t tamanio_ruta_escriptorio = strlen(dtb->escriptorio);

	int tamanio = 	sizeof(dtb->id_dtb) +
					sizeof(tamanio_ruta_escriptorio) +
					tamanio_ruta_escriptorio +
					sizeof(dtb->program_counter) +
					sizeof(dtb->inicializado) +
					sizeof(dtb->quantum) +
					sizeof(cantidad_elementos) +
					tamanio_lista_para_buffer(dtb->direcciones);

	void* buffer = malloc(tamanio);
	printf("longitud: %d\n",tamanio);


	int lastIndex = 0;
	serialize_data(&(dtb->id_dtb),sizeof(dtb->id_dtb), &buffer, &lastIndex);
	serialize_data(&(tamanio_ruta_escriptorio),sizeof(tamanio_ruta_escriptorio), &buffer, &lastIndex);
	serialize_data(&(dtb->escriptorio),tamanio_ruta_escriptorio, &buffer, &lastIndex);
	serialize_data(&(dtb->program_counter),sizeof(dtb->program_counter), &buffer, &lastIndex);
	serialize_data(&(dtb->inicializado),sizeof(dtb->inicializado), &buffer, &lastIndex);
	serialize_data(&(dtb->quantum),sizeof(dtb->quantum), &buffer, &lastIndex);

	//serializo la cantidad de elementos
	cantidad_elementos = dtb->direcciones->elements_count;
	serialize_data(&(cantidad_elementos),sizeof(cantidad_elementos), &buffer, &lastIndex);

	//serializo los elementos de la forma [tamanio -> elemento]
	uint8_t tamanio_ruta = 0;
	uint8_t tamanio_direccion = 0;
	t_link_element *element = dtb->direcciones->head;
	t_link_element *aux = NULL;
	while (element != NULL) {
		aux = element->next;

		direccion_struct* direccion = element->data;

		tamanio_ruta = strlen(direccion->path);
		serialize_data(&(tamanio_ruta),sizeof(tamanio_ruta), &buffer, &lastIndex);
		serialize_data(&(direccion->path),tamanio_ruta, &buffer, &lastIndex);

		tamanio_direccion = strlen(direccion->direccion);
		serialize_data(&(tamanio_direccion),sizeof(tamanio_direccion), &buffer, &lastIndex);
		serialize_data(&(direccion->direccion),tamanio_direccion, &buffer, &lastIndex);

		element = aux;
	}

	*tamanio_buffer = lastIndex;

	return buffer;
}


int tamanio_dtb( dtb_struct *dtb ){
	//esta funcion calcula el tamaño que va a tener el buffer de un dtb
	//no se usa por ahora...
	//va a ser util al momento de enviar un send
	//aunque tambien se puede solucionar ese problema agregando un parametro mas a la funcion serializar
	//que acepte un puntero a int y cambie el valor por el tamanio del buffer

	uint8_t tamanio_ruta_escriptorio = strlen(dtb->escriptorio);

	int tamanio =	sizeof(dtb->id_dtb) +
					sizeof(tamanio_ruta_escriptorio) +
					tamanio_ruta_escriptorio +
					sizeof(dtb->program_counter) +
					sizeof(dtb->inicializado) +
					sizeof(dtb->quantum);

	return tamanio;

}

dtb_struct* deserializar_dtb(void *buffer){
	//tal como su nombre lo dice esta funcion deserializa un dtb a partir de un buffer
	//este dtb a deserializar es completamente dinamico en la cantida de direcciones y tamanio de cada direccion
	//tiene un error de stack smashing en las lineas 213 y 214, si lo descomentas funcion
	//pero en algun lugar hay un stack smashing...
	dtb_struct* dtb = malloc(sizeof(dtb_struct));
	int lastIndex = 0;

	deserialize_data(&(dtb->id_dtb),sizeof(dtb->id_dtb), buffer, &lastIndex);
	uint8_t tamanio_ruta_escriptorio;
	deserialize_data(&(tamanio_ruta_escriptorio),sizeof(uint8_t), buffer, &lastIndex);
	dtb->escriptorio = malloc((tamanio_ruta_escriptorio + 1) * sizeof(char));

	myMemCpy(&(dtb->escriptorio), buffer+lastIndex, tamanio_ruta_escriptorio);
	lastIndex += tamanio_ruta_escriptorio;
	(dtb->escriptorio)[tamanio_ruta_escriptorio]='\0';

	deserialize_data(&(dtb->program_counter),sizeof(dtb->program_counter), buffer, &lastIndex);
	deserialize_data(&(dtb->inicializado),sizeof(dtb->inicializado), buffer, &lastIndex);
	deserialize_data(&(dtb->quantum),sizeof(dtb->quantum), buffer, &lastIndex);


	dtb->direcciones = list_create();

	uint8_t cantidad_de_direcciones = 0;
	deserialize_data(&cantidad_de_direcciones,sizeof(cantidad_de_direcciones), buffer, &lastIndex);
	printf("Cantidad de direcciones a deserializar: %d\n", cantidad_de_direcciones);


	uint8_t tamanio_path = 0;
	uint8_t tamanio_direccion = 0;
	int var;

	for (var = 0; var < cantidad_de_direcciones; var++) {

		direccion_struct * direcciones = malloc(sizeof(direccion_struct));

		deserialize_data(&tamanio_path,sizeof(uint8_t), buffer, &lastIndex);
		direcciones->path = malloc(tamanio_path+1);
		myMemCpy(&(direcciones->path), buffer+lastIndex, tamanio_path);
		lastIndex += tamanio_path;
		(direcciones->path)[tamanio_path]='\0';

		deserialize_data(&tamanio_direccion,sizeof(uint8_t), buffer, &lastIndex);
		direcciones->direccion = malloc(tamanio_direccion+1);
		myMemCpy(&(direcciones->direccion), buffer+lastIndex, tamanio_direccion);
		lastIndex += tamanio_direccion;
		(direcciones->direccion)[tamanio_direccion]='\0';

		list_add(dtb->direcciones,direcciones);


		//TODO: en estas 2 lineas esta el problema y tambien la solucion...
		//deserialize_data(&direccion,tamanio_direccion, buffer, &lastIndex);
		//direccion[tamanio_direccion] = '\0';




		//printf("direccion: %s\n",direccion);
		//list_add(dtb->direcciones,strdup(direccion));

		//tamanio_direccion = 0;
		//break;


	}
	//free(direccion);
	//TODO: corregir (stack smashing error)
	/*
	puts("///////////////////////");
	puts("Direcciones: ");
	list_iterate(dtb->direcciones, (void *)puts);
	puts("///////////////////////");
*/
	return dtb;

}

//////////////////////////////////////////
///////INTENTO DE SERIALIZO DE DTB/////////
//////////////////////////////////////////





header_conexion_type* deserializar_header_conexion(void *buffer) {
	header_conexion_type* header = malloc(sizeof(header_conexion_type));
	int lastIndex = 0;

	deserialize_data(&(header->tipo_instancia), 4, buffer, &lastIndex);
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
