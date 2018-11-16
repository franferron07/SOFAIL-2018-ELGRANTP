/*
 * util.h
 *
 *  Created on: 13 nov. 2018
 *      Author: jhondaniel
 */

#ifndef CONFIG_UTIL_H_
#define CONFIG_UTIL_H_
#include <string.h>
#include <stdbool.h>
#include <stdio.h>



#define loop while(1)
#define minimo(unNum,otroNum)  ((unNum>otroNum)?otroNum:unNum)

char* recortarPrimerosCaracteres(char* s, int primerosCaracteres);//ok y  malloquea automaticamente

int cantidadDeCaracteres_file(FILE* bloque);
int cantidadDeCaracteres_path(char* path );//ok
bool quedaContenidoParaMapear(char* contenido);
#endif /* CONFIG_UTIL_H_ */
