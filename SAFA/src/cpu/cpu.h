#ifndef CPU_CPU_H_
#define CPU_CPU_H_

#include "../commons/commons.h"
#include "../config/config.h"
#include <stdbool.h>

t_list *cpus;


cpu_struct crear_cpu(int socket);
cpu_struct *obtener_cpu_libre();
void inicializar_lista_cpus();
void liberar_cpu(cpu_struct * cpu);
void liberar_recursos_cpu();
bool cpu_libre(cpu_struct *cpu);

#endif /* CPU_CPU_H_ */
