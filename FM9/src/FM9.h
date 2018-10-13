#ifndef FM9_H_
#define FM9_H_

#include <qcommons/console.h>
#include <qcommons/serialize.h>
#include <qcommons/socket.h>
#include <qcommons/socketServer.h>
#include <pthread.h>

#include "config/config.h"
#include "consola/consola.h"

#define FILE_CONFIG_FM9 "fm9.config"
#define FM9 "FM9"

pthread_t id_hilo;
pthread_t hilo_consola;
pthread_t hilo_principal;

int inicializar();
void escuchar_consola();
void iniciar_fm9();
void liberar_recursos(int tipo_salida);
void terminar_exitosamente();

void nueva_conexion (void *parametro);

#endif /* FM9_H_ */
