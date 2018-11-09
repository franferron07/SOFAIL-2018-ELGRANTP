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
void inicializar_dummy(dtb_struct* dtb);
dtb_struct* aplicarRR();
dtb_struct* aplicarVRR();
dtb_struct* aplicarPropio();

recurso_struct* buscar_recurso( char*nombre_recurso );
recurso_struct*  crear_recurso( char*nombre_recurso );
void asignar_recurso( recurso_struct *recurso );
void liberar_recurso( recurso_struct *recurso );


void ejecutar_planificacion_largo_plazo_aux();
dtb_struct *obtener_dtb_a_ejecutar_dummy(); //funcion que me devuelve el primer dtb de la lista de nuevos en estado nuevo
bool dtb_estado_nuevo(dtb_struct *dtb);

#endif /* PLANIFICACION_PLANIFICACION_H_ */
