#include "FM9.h"

int main(int argc, char *argv[]) {
	if (inicializar() < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();

	pthread_create(&hilo_principal, NULL, (void*) iniciar_fm9, NULL);
	pthread_create(&hilo_consola, NULL, (void*) escuchar_consola, NULL);

	pthread_join(hilo_consola, NULL);

	Socket socket = crear_socket("127.0.0.1", string_itoa(fm9.puerto));
	//Asocio el servidor a un puerto
	asociar_puerto(socket);
	//Escucho Conexiones Entrantes
	escuchar(socket);

	/*Por cada una de las conexiones que sean aceptadas, se lanza
	 un Hilo encargado de atender la conexión*/
	while (1) {

		int socket_cliente = Acepta_Conexion_Cliente(socket.socket);
		pthread_create(&id_hilo, NULL, (void*) nueva_conexion, &socket_cliente);
	}

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
}

void liberar_recursos(int tipo_salida) {
	print_footer(FM9, fm9_log);
	destruir_archivo_log(fm9_log);
	terminar_exitosamente(tipo_salida);
}

void terminar_exitosamente(int ret_val) {
	exit(ret_val);
}

void nueva_conexion(void *parametro) {
	int *sock = (int *) parametro;
	log_info(fm9_log, "Nueva conexion %d perrooo!!", sock);
}
