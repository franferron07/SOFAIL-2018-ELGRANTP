/*
 * util.c
 *
 *  Created on: 13 nov. 2018
 *      Author: jhondaniel
 */
#include "util.h"

char* recortarPrimerosCaracteres(char* s, int primerosCaracteres){//ok y  malloquea automaticamente
	char* recorte=NULL;
	if(s==NULL ||  strlen(s)==0){
		perror("la cadena a recortar es es nula o termino de recortar cadena \n");
		return strdup("") ;
	}
	else if(strlen(s)<primerosCaracteres){
		recorte=strndup(s,primerosCaracteres);
		strcpy(s,"");
		return recorte;
	}
	recorte= strndup(s,primerosCaracteres);
	char* aux=strdup(s);
//	if(strlen(s)<primerosCaracteres)perror("Error en recorte de caracteres \n");
	strcpy(s,aux+primerosCaracteres);
	free(aux);
	return recorte;
}
