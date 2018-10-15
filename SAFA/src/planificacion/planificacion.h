#ifndef PLANIFICACION_PLANIFICACION_H_
#define PLANIFICACION_PLANIFICACION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include "../config/config.h"
#include "../cpu/cpu.h"

void aplicarRR(bool es_virtual);
void aplicarPropio();

#endif /* PLANIFICACION_PLANIFICACION_H_ */
