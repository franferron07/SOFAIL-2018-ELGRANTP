#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>
#include "../commons/commons.h"

#define RR_STRING "RR"
#define VRR_STRING "VRR"
#define PROPIO_STRING "PROPIO"

typedef enum {
	RR, VRR, PROPIO
} algoritmo_type;

typedef struct {
	int puerto;
	algoritmo_type algoritmo;
	int quantum;
	int multiprogramacion;
	int retardo_planif;

} safa_config;

safa_config safa;
t_log *safa_log;

int cargar_archivo_config(char* path);
int crear_log();
void imprimir_config();
void liberar_recursos_configuracion();
char* obtener_nombre_algoritmo(safa_config safa);
void destruir_archivo_log(t_log *safa_log);


//segun la multiprogramacion cambiada hace los post o wait correspondientes para aumentar la multiprogramacion o disminuirla. Si es igual no se hace nada.
void actualizar_multiprogramacion(int multi_anterior , int multi_actual);

#endif /* CONFIG_CONFIG_H_ */
