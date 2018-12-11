#ifndef PLANIFICACION_PLANIFICACION_H_
#define PLANIFICACION_PLANIFICACION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include "../config/config.h"
#include "../cpu/cpu.h"
#include "../dtb/dtb.h"

dtb_struct * aplicar_algoritmo_planificacion();
void set_quantum(int p_quantum);
void set_algoritmo(int p_algoritmo);
void ejecutar_planificacion_corto_plazo();
void ejecutar_planificacion_largo_plazo();
dtb_struct* aplicarRR();
dtb_struct* aplicarVRR();
dtb_struct* aplicarPropio(); //IOBF IOBF (IO Bound First): Priorización de DTBs IO bound.
							 //Aquellos procesos que realicen muchas E/S (operaciones de MDJ) deberán ser favorecidos a la hora de la planificación.
dtb_struct* quitar_dtb_lista_id( t_list *lista  ,int id ); //a partir de un id de dtb lo quita de la lista y devuelve el elemento
dtb_struct* buscar_dtb_id( t_list *lista  ,int id ); //a partir de un id busca el dtb en una lista

void agregar_dtb_a_listos( dtb_struct *dtb ); //agrega un dtb a cola de listos. Verifica la multiprogramacion
void desbloquear_dtb( dtb_struct* dtb ); //solicitud de desbloqueo de un dtb a listo TODO verificar en el caso que sea por liberacion de un recurso.

recurso_struct* buscar_recurso( char*nombre_recurso );
recurso_struct*  crear_recurso( char*nombre_recurso );
void asignar_recurso( recurso_struct *recurso );
void liberar_recurso( recurso_struct *recurso );


#endif /* PLANIFICACION_PLANIFICACION_H_ */
