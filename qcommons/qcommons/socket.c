#include "socket.h"

int obtener_socket_cliente(int* socket_cliente, const char* direccion_ip,
		const int puerto) {
	struct sockaddr_in config_servidor;
	*socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (*socket_cliente == -1)
		printf("No se pudo crear el socket.\n");
	config_servidor.sin_addr.s_addr = inet_addr(direccion_ip);
	config_servidor.sin_family = AF_INET;
	config_servidor.sin_port = htons(puerto);
	if (connect(*socket_cliente, (struct sockaddr *) &config_servidor,
			sizeof(config_servidor)) < 0) {
		perror("Conexión fallida");
		return (-1);
	}
	printf("\n .:: Conectado al servidor en %s:%d ::.\n", direccion_ip, puerto);
	return 0;
}

int configurar_socket_servidor(int* socket_servdor, const char* direccion_ip,
		const int puerto, const int cantidad_clientes) {
	int yes=1;
	struct sockaddr_in config_servidor;
	*socket_servdor = socket(AF_INET, SOCK_STREAM, 0);
	config_servidor.sin_family = AF_INET;
	config_servidor.sin_addr.s_addr = inet_addr(direccion_ip);
	config_servidor.sin_port = htons(puerto);
    if (setsockopt(*socket_servdor, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
        perror("setsockopt");
        return (-1);
    }
	if (bind(*socket_servdor, (struct sockaddr *) &config_servidor,
			sizeof(config_servidor)) < 0) {
		perror("Falló el bind");
		return (-1);
	}
	listen(*socket_servdor, cantidad_clientes);
	return 0;
}

int aceptar_conexion(int socket_servidor) {
	struct sockaddr_in config_cliente;
	int nuevo_cliente;
	int tamanio_struct = sizeof(struct sockaddr_in);
	nuevo_cliente = accept(socket_servidor, (struct sockaddr *) &config_cliente,
			(socklen_t*) &tamanio_struct);
	if (nuevo_cliente < 0) {
		perror("Falló el accept");
		return (-1);
	}
	printf(" .:: Conexión nueva aceptada con socket: %d ::.\n", nuevo_cliente);
	return nuevo_cliente;
}
