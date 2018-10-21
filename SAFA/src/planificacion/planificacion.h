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

void aplicar_algoritmo_planificacion();
void set_quantum(int p_quantum);
void set_algoritmo(int p_algoritmo);
void aplicarRR(bool es_virtual);
void aplicarPropio();

#endif /* PLANIFICACION_PLANIFICACION_H_ */
