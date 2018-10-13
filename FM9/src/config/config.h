#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>

#define SEG_STRING "SEG"
#define TPI_STRING "TPI"
#define SPA_STRING "SPA"

typedef enum {
	SEG, TPI, SPA
} modo_type;

typedef struct {
	int puerto;
	modo_type modo;
	int tamanio;
	int max_linea;
	int tam_pagina;
} fm9_config;

fm9_config fm9;
t_log *fm9_log;

int cargar_archivo_config(char* path);
void imprimir_config();
void liberar_recursos_configuracion();
char* obtener_nombre_modo(fm9_config fm9);
int crear_log();
void destruir_archivo_log(t_log *fm9_log);

#endif /* CONFIG_CONFIG_H_ */
