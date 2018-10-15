#include "gestorGDT.h"

static t_comando_struct tabla_referencia_comandos[] = { { "ejecutar",
		CONSOLA_COMANDO_EJECUTAR }, { "status", CONSOLA_COMANDO_STATUS }, {
		"finalizar", CONSOLA_COMANDO_FINALIZAR }, { "metricas",
		CONSOLA_COMANDO_METRICAS } };

int retorno = CONSOLA_CONTINUAR;

void liberar_parametros(char** split);
char** parsear_entrada_por_espacios(char *entrada);
char* obtener_comando(char** split);
char* obtener_parametro(char** split);

int consola_leer_comando() {
	size_t tamanio = TAMANIO_ENTRADA_STDIN;
	char *entrada = malloc(tamanio);
	fgets(entrada, tamanio, stdin);

	char** split_entrada = malloc(sizeof(char**));
	char *comando = malloc(sizeof(char*));
	char *parametro = malloc(sizeof(char*));
	split_entrada = parsear_entrada_por_espacios(entrada);
	comando = obtener_comando(split_entrada);
	parametro = obtener_parametro(split_entrada);

	switch (obtener_valor_por_clave(comando)) {
	case CONSOLA_COMANDO_EJECUTAR:
		comando_ejecutar(parametro);
		break;
	case CONSOLA_COMANDO_STATUS:
		comando_status(parametro);
		break;
	case CONSOLA_COMANDO_FINALIZAR:
		comando_finalizar(parametro);
		break;
	case CONSOLA_COMANDO_METRICAS:
		comando_metricas(parametro);
		break;
	case CONSOLA_COMANDO_DESCONOCIDO:
		log_warning(safa_log, "El comando ingresado no fue encontrado\n");
		break;
	}

	liberar_parametros(split_entrada);
	if (entrada != NULL) {
		free(entrada);
	}
	if (!_esta_vacia(comando)) {
		free(comando);
	}
	if (!_esta_vacia(parametro)) {
		free(parametro);
	}

	return retorno;
}

void comando_ejecutar(char* ruta_sript) {
	if (!_es_cadena_valida(ruta_sript)) {
		log_info(safa_log,
				"Comando ejecutar, el parámetro ingresado no es correcto!\n");
	} else {
		log_info(safa_log, "Comando ejecutar con ruta_sript: %s\n", ruta_sript);
		dtb_struct dtb_nuevo = crear_dtb(ruta_sript);
		sem_wait(&sem_nuevo_mutex);
		queue_push(cpu_nuevos, &dtb_nuevo);
		sem_post(&sem_nuevo_vacio);
		sem_post(&sem_nuevo_mutex);
	}
}

void comando_status(char* id_dt_block) {
	if (!_es_cadena_valida(id_dt_block)) {
		log_info(safa_log, "Comando status SIN id_dt_block\n");
	} else {
		log_info(safa_log, "Comando status con id_dt_block: %s\n", id_dt_block);
	}
}

void comando_finalizar(char* id_dt_block) {
	if (!_es_cadena_valida(id_dt_block)) {
		log_info(safa_log,
				"Comando finalizar, el parámetro ingresado no es correcto!\n");
	} else {
		log_info(safa_log, "Comando finalizar con id_dt_block: %s\n",
				id_dt_block);
	}
}

void comando_metricas(char* id_dt_block) {
	if (!_es_cadena_valida(id_dt_block)) {
		log_info(safa_log, "Comando metricas SIN id_dt_block: %s\n");
	} else {
		log_info(safa_log, "Comando metricas con id_dt_block: %s\n",
				id_dt_block);
	}
}

char** parsear_entrada_por_espacios(char *entrada) {
	_finalizar_cadena(entrada);
	char **split = string_split(entrada, " ");

	return split;
}

char* obtener_comando(char** split_entrada) {
	char *comando = _obtener_iesimo_parametro(split_entrada, 0);
	string_to_lower(comando);

	return comando;
}

char* obtener_parametro(char** split_entrada) {
	char *parametro = _obtener_iesimo_parametro(split_entrada, 1);
	string_to_lower(parametro);

	return parametro;
}

void liberar_parametros(char** split) {
	int i = 0;
	while (split[i] != NULL) {
		free(split[i]);
		i++;
	}
	free(split);
}

int obtener_valor_por_clave(char *clave) {
	int i;

	for (i = 0; i < TAMANIO_CLAVE; i++) {
		t_comando_struct comando = tabla_referencia_comandos[i];
		if (strcmp(comando.clave, clave) == 0)
			return comando.valor;
	}
	return CONSOLA_COMANDO_DESCONOCIDO;
}
