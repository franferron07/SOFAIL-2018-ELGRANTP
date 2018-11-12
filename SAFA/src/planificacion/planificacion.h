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
void ejecutar_planificacion();
void ejecutar_planificacion_largo_plazo();
dtb_struct* aplicarRR();
dtb_struct* aplicarVRR();
dtb_struct* aplicarPropio();

dtb_struct* quitar_dtb_lista_id( t_list *lista  ,int id ); //a partir de un id de dtb lo quita de la lista y devuelve el elemento
dtb_struct* buscar_dtb_id( t_list *lista  ,int id ); //a partir de un id busca el dtb en una lista

recurso_struct* buscar_recurso( char*nombre_recurso );
recurso_struct*  crear_recurso( char*nombre_recurso );
void asignar_recurso( recurso_struct *recurso );
void liberar_recurso( recurso_struct *recurso );


#endif /* PLANIFICACION_PLANIFICACION_H_ */
