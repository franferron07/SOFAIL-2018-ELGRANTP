/*
 * util.h
 *
 *  Created on: 13 nov. 2018
 *      Author: jhondaniel
 */

#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_
#include <string.h>
#include <stdbool.h>
#include <stdio.h>



#define loop while(1)
#define minimo(unNum,otroNum)  ((unNum>otroNum)?unNum:otroNum)

char* recortarPrimerosCaracteres(char* s, int primerosCaracteres);//ok y  malloquea automaticamente

int cantidadDeCaracteres_file(FILE* bloque);
int cantidadDeCaracteres_path(char* path );//ok

#endif /* UTIL_UTIL_H_ */
