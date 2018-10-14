#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>

typedef struct {
	int puerto_dam;
	char *ip_safa;
	int puerto_safa;
	char *ip_mdj;
	int puerto_mdj;
	char *ip_fm9;
	int puerto_fm9;
	int transfer_size;
} dam_config;

dam_config dam;
t_log *dam_log;

int cargar_archivo_config(char* path);
void imprimir_config();
void liberar_recursos_configuracion();
char* obtener_nombre_modo(dam_config dam);
int crear_log();
void destruir_archivo_log(t_log *dam_log);

#endif /* CONFIG_CONFIG_H_ */
