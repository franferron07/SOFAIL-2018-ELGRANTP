#ifndef CPU_H_
#define CPU_H_

#include <commons/string.h> //Commons string
#include <qcommons/console.h>
#include <qcommons/socket.h> //Libreria Socket Cliente
#include <qcommons/protocolos.h>
#include <signal.h>

#include "config/config.h"//Llamadas para leer y mostrar el archivo config
#include "parser/parser.h"//parser del Lenguaje EscripTorio


#define FILE_CONFIG_CPU "cpu.config"
#define EXTENSION_FILE_LOG_CPU ".log"
#define CPU "CPU"

int socket_safa, socket_diego, socket_fm9;
struct_instruccion instruccion;




//Funciones
void conectarse_con_safa();
void conectarse_con_diego();
void conectarse_con_fm9();
int inicializar(char* nombre_archivo_log);
char* crear_nombre_file_log();
void liberar_recursos(int tipo_salida);
void exit_gracefully();

void ejecutar_instruccion(struct_instruccion instruccion);
void liberar_instruccion(struct_instruccion instruccion);


//////EJECUCION DE OPERACIONES//////
void escriptorio_abrir(char** parametros);
void escriptorio_concentrar(char** parametros);
void escriptorio_asignar(char** parametros);
void escriptorio_wait(char** parametros);
void escriptorio_signal(char** parametros);
void escriptorio_flush(char** parametros);
void escriptorio_close(char** parametros);
void escriptorio_crear(char** parametros);
void escriptorio_borrar(char** parametros);
void escriptorio_comentario(char** parametros);
///////////////////////////////////



#endif /* CPU_H_ */
