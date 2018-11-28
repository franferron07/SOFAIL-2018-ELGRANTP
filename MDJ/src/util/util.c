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
int cantidadDeCaracteres_file(FILE* bloque){

	int i ;
	for(i=0;getc(bloque)!=EOF;i++);
	return i;
}
int cantidadDeCaracteres_path(char* path ){//OK
	FILE * f = fopen(path,"r+");
	int n = cantidadDeCaracteres_file(f);
	fclose(f);
	return n ;
}
bool quedaContenidoParaMapear(char* contenido){return strlen(contenido)>0 && contenido!=NULL;}
void crearBloques(int cantidad){//ok, se puede mejorar con realloc
//	int n = metadata.cantidad_bloques;
	for(int var = 0;var<cantidad;var++){
		char* unPath = malloc(100);
		sprintf(unPath,"%d.bin",var);
		FILE* f = fopen(unPath,"w+");
		txt_close_file(f);
		free(unPath);
	}
}
char* str_concat(  char* from,const char* add){//devuelve algo malloqueado,OK,guarda
	if(from==NULL)return strdup(add);
	else{
		int malloq=(strlen(from)+strlen(add))*sizeof(char);
		char* aux = malloc(malloq);
		sprintf(aux,"%s%s",from,add);
		free(from);
		return aux;
	}
}
char* intToString( int n){//OK,se puede borrar el printf
//	int cantidadDeDigitos;
//	int resto=n*10;
//	for(cantidadDeDigitos=0;resto%10==0;cantidadDeDigitos++)resto=resto/10;
//	char* aux = malloc(cantidadDeDigitos*sizeof(char));
	char* aux = malloc(4);
	sprintf(aux,"%d",n);
	return aux;
}

