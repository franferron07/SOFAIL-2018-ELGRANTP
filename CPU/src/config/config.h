#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>


typedef struct {
	int ip_safa;
	int puerto_safa;
	int ip_diego;
	int puerto_diego;
	int ip_fm9;
	int puerto_fm9;
	int retardo;
} cpu_config;


cpu_config cpu;
t_log *cpu_log;

int cargar_archivo_config(char* path);
void imprimir_config();
void liberar_recursos_configuracion();
int crear_log();
void destruir_archivo_log(t_log *cpu_log);



#endif /* CONFIG_CONFIG_H_ */
