#include "gestorGDT.h"

static t_comando_struct tabla_referencia_comandos[] = { { "ejecutar",
		CONSOLA_COMANDO_EJECUTAR }, { "status", CONSOLA_COMANDO_STATUS }, {
		"finalizar", CONSOLA_COMANDO_FINALIZAR }, { "metricas",
		CONSOLA_COMANDO_METRICAS } };

int retorno = CONSOLA_CONTINUAR;
int id_dtb = 0;

void liberar_parametros(char** split);
char** parsear_entrada_por_espacios(char *entrada);
char* obtener_comando(char** split);
char* obtener_parametro(char** split);




void consola() {

	while (true) {
		if (consola_leer_comando() == CONSOLA_TERMINAR) {
			pthread_exit(0);
			return;
		}
	}
}

int generar_id_dtb() {
	return id_dtb++;
}


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

	/*if (entrada != NULL) {
		free(entrada);
	}*/

	if (!_esta_vacia(entrada)) {
			free(entrada);
		}

	/*if (!_esta_vacia(comando)) {
		free(comando);
	}*/
	/*if (!_esta_vacia(parametro)) {
		free(parametro);
	}*/


	return retorno;
}

void comando_ejecutar(char* ruta_sript) {
	if (!_es_cadena_valida(ruta_sript)) {
		log_info(safa_log,
				"Comando ejecutar, el parámetro ingresado no es correcto!\n");
	} else {
		log_info(safa_log, "Comando ejecutar con ruta_sript: %s\n", ruta_sript);
		generar_id_dtb();
		dtb_struct dtb_nuevo = crear_dtb(id_dtb ,ruta_sript);
		agregar_dtb_nuevo( dtb_nuevo );
		log_info(safa_log, "DTB creado y en cola de nuevos:%d\n", dtb_nuevo.id_dtb);
	}
}

void agregar_dtb_nuevo( dtb_struct dtb_nuevo ){

	/*pthread_mutex_lock(&sem_nuevo_mutex);*/
	list_add(dtb_nuevos, &dtb_nuevo);
	/*pthread_mutex_unlock(&sem_nuevo_mutex);*/
	list_add( dtbs , &dtb_nuevo );
}

void comando_status(char* id_dt_block) {
	if (!_es_cadena_valida(id_dt_block)) {
		log_info(safa_log, "Comando status SIN id_dt_block\n");

		//TODO Se deberá mostrar el estado de cada cola, así como la información complementaria a las mismas.


	} else {
		log_info(safa_log, "Comando status con id_dt_block: %s\n", id_dt_block);

		int id_dtb = atoi(id_dt_block );

		/* SI EXISTE MUESTRO TODA LA INFO DEL DTB */
		dtb_struct *dtb = buscar_dtb_id( dtbs , id_dtb );

		if( dtb != NULL ){

			log_info(safa_log, "******INICIO INFO DEL DTB ********");

			log_info(safa_log, "id_dtb: %d" , dtb->id_dtb);
			log_info(safa_log, "escriptorio: %s" , dtb->escriptorio);
			log_info(safa_log, "program_counter: %d" , dtb->program_counter);
			log_info(safa_log, "inicializado: %d" , dtb->inicializado);
			log_info(safa_log, "quantum: %d" , dtb->quantum);
			log_info(safa_log, "estado: %s" , dtb->estado);
			log_info(safa_log, "direcciones");
			log_info(safa_log, "sentencias_espera: %d" , dtb->sentencias_espera);
			log_info(safa_log, "sentencias_dam: %d" , dtb->sentencias_dam);
			log_info(safa_log, "sentencias_totales: %d" , dtb->sentencias_totales);
			log_info(safa_log, "sentencias_mdj: %d" , dtb->sentencias_mdj);

			log_info(safa_log, "******FIN INFO DEL DTB ********");
		}
		else{

			log_info(safa_log, "No existe el dtb con ese id en el sistema: %s\n", id_dt_block);
		}

	}
}

void comando_finalizar(char* id_dt_block) {
	if (!_es_cadena_valida(id_dt_block)) {
		log_info(safa_log,
				"Comando finalizar, el parámetro ingresado no es correcto!\n");
	} else {
		log_info(safa_log, "Comando finalizar con id_dt_block: %s\n",id_dt_block);

		int id_dtb = atoi(id_dt_block);

		/* SI EXISTE MUESTRO VERIFICO ESTADO DEL DTB Y LO FINALIZO */
		dtb_struct *dtb = NULL;
		dtb =buscar_dtb_id( dtbs , id_dtb );

		if( dtb != NULL ){

			//SI DTB NO ESTA EJECUTANDO LO FINALIZO SINO ESPERO
			if( dtb->estado != EJECUTANDO ){

				quitar_dtb_lista_id( dtb_nuevos , dtb->id_dtb );
				quitar_dtb_lista_id( dtb_listos , dtb->id_dtb );
				quitar_dtb_lista_id( dtb_bloqueados , dtb->id_dtb );
			}
			else{

				//ESPERO A QUE FINALICE SU EJECUCION
				while( dtb->estado == EJECUTANDO ){

				}
				//FINALIZO EJECUCION LO QUITO DE LISTOS Y BLOQUEADOS (posibles listas a las que puede ir desde ejecutando)
				quitar_dtb_lista_id( dtb_listos , dtb->id_dtb );
				quitar_dtb_lista_id( dtb_bloqueados , dtb->id_dtb );

			}

			//TODO esto se puede hacer en una funcion ya que lo usamos aca y en la comunicacion con CPU.
			dtb->estado = FINALIZADO;
			list_add(dtb_terminados , dtb);

			log_info(safa_log, "Se finaliza dtb con id: %d\n",dtb->id_dtb);

			//TODO habria que ver si cuando pasa esto se debe avisar a dam para que se libere la memoria.
		}


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
