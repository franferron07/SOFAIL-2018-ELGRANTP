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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/mman.h>

#include <commons/string.h>
#include <commons/collections/list.h>

#define loop while(1)
#define minimo(unNum,otroNum)  ((unNum>otroNum)?otroNum:unNum)

char* recortarPrimerosCaracteres(char* s, int primerosCaracteres);//ok y  malloquea automaticamente

int cantidadDeCaracteres_file(FILE* bloque);
int cantidadDeCaracteres_path(char* path );//ok
bool quedaContenidoParaMapear(char* contenido);

char* str_concat(  char* from,const char* add);//devuelve algo malloqueado,OK,guarda
char* intToString( int n);//OK,se puede borrar el printf

void crearBloques(int cantidad);//ok
off_t cantidadDeCaracteresDeFile(const char* pathFile);

char* fileToString(const char* pathFile);
char* fileToString_v2(const char* pathFile);



#endif /* CONFIG_UTIL_H_ */
