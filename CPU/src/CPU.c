/*
 ============================================================================
 Name        : CPU.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Proceso CPU
 ============================================================================
 */

#include "CPU.h"

int main(void) {
	puts("CPU"); /* prints CPU */

	t_config *inicializador;
	c_inicial = malloc(sizeof(config_inicial));
	inicializador = config_create("cpu.cfg");
	if (inicializador == NULL) {
		free(c_inicial);
		puts("No se encuentra archivo.");
		exit(EXIT_FAILURE);
	}

	//leo archivo
	leer_configuracion(inicializador, c_inicial);
	//muestro consola valor leido de archivo como prueba
	prueba_leer_archivo_cfg(c_inicial);
	/* libero memoria de inicializacion  */
	config_destroy(inicializador);

	socket_dam = conectar_dam(c_inicial);
	socket_safa = conectar_safa(c_inicial);

	/* libero struct config_inicial  */
	liberarMemoriaConfig(c_inicial);

	return EXIT_SUCCESS;
}

Socket conectar_dam(config_inicial* c_inicial){
	Socket socket;
	//Header header;

	socket = crear_socket(c_inicial->ip_diego , c_inicial->puerto_diego);
	conectar(socket);

	//Envio a Coordinador Mensaje Protocolo soy un ESI
	/*
	header.id_protocolo=IdEsi;
	if( Escribe_Socket(socket.socket,(char *)&header, sizeof(Header)) == -1 ) {
		puts( "Error en envio de mensaje" );
	}*/

	//Recibo un Identificador del Coordinador
	if( Lee_Socket(socket.socket, (char *)&identificadorCPU, sizeof(int)) == -1 ) {
		puts("Error de lectura");
		exit(EXIT_FAILURE);
	}

	printf("Soy el CPU: %d\n",identificadorCPU);

	printf("Se envio el mensaje. Conexion correcta a dam. \n");

	return socket;
}


Socket conectar_safa(config_inicial* c_inicial){
	Socket socket;
	//Header header;

	socket = crear_socket(c_inicial->ip_safa , c_inicial->puerto_safa);
	conectar(socket);

	//Envio a Coordinador Mensaje Protocolo soy un ESI
	/*
	header.id_protocolo=IdEsi;
	if( Escribe_Socket(socket.socket,(char *)&header, sizeof(Header)) == -1 ) {
		puts( "Error en envio de mensaje" );
	}*/

	//Recibo un Identificador del safa
	/*if( Lee_Socket(socket.socket, (char *)&identificadorCPU, sizeof(int)) == -1 ) {
		puts("Error de lectura");
		exit(EXIT_FAILURE);
	}

	printf("Soy el CPU: %d\n",identificadorCPU);*/

	printf("Se envio el mensaje. Conexion correcta a safa. \n");

	return socket;
}

/*
 * 	char *ip_safa;
	char *puerto_safa;
	char *ip_diego;
	char *puerto_diego;
	int retardo;
 * */

void leer_configuracion(t_config *inicializador , config_inicial *c_inicial ){

	//tomo las key para inicializar duplicando el string devuelvo para luego hacer los free
	c_inicial->ip_safa = string_duplicate(config_get_string_value(inicializador, "IP_SAFA"));
	c_inicial->puerto_safa = string_duplicate(config_get_string_value(inicializador, "PUERTO_SAFA"));
	c_inicial->ip_diego = string_duplicate(config_get_string_value(inicializador, "IP_DIEGO"));
	c_inicial->puerto_diego = string_duplicate(config_get_string_value(inicializador, "PUERTO_DIEGO"));
	c_inicial->retardo= config_get_int_value(inicializador, "RETARDO");

}

void prueba_leer_archivo_cfg(config_inicial* c_inicial) {
	puts("lectura de archivo correcta");
	printf("IP_SAFA: %s \n",c_inicial->ip_safa);
	printf("PUERTO_SAFA: %s \n",c_inicial->puerto_safa);
	printf("IP_DIEGO: %s \n",c_inicial->ip_diego);
	printf("PUERTO_DIEGO: %s \n" ,c_inicial->puerto_diego);
	printf("RETARDO %d \n", c_inicial->retardo);
}

void liberarMemoriaConfig(config_inicial* c_inicial) {

	free(c_inicial->ip_safa);
	free(c_inicial->puerto_safa);
	free(c_inicial->ip_diego);
	free(c_inicial->puerto_diego);
	free(c_inicial);
}
