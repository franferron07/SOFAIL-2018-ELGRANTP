/*
 * bitmap.h
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */

#ifndef FILE_SYSTEM_BITMAP_H_
#define FILE_SYSTEM_BITMAP_H_
#include <string.h>
#include <stdio.h>
#include <commons/bitarray.h>
#include <commons/txt.h>
#include <stdbool.h>

#include <stdlib.h>
#include "metadata.h"



//BITMAP begin
char* bloqueActual_path;//=NULL;
 int bloqueActual_int;//=0;
char* bitmap_path_directorio;
FILE* bitmap_file;
t_bitarray* bitarray_;

void setear_bloque_ocupado_en_posicion(off_t pos);
bool testear_bloque_libre_en_posicion(int pos);
void setBloqueLleno();//agregar un 1 al bitmap.bin
void mostrar_bitarray();

void getBloqueLibre_path();
bool estaLibreElBloqueActual(FILE* bloqueActual, int tamanioDeBloque);
void cargar_configuracion_bitmap();
int getBloqueLibre_int();//obtiene el proximo bloque libre ,OK

//auxiliares
char* bloquesToString(const char* pathFile);
t_list* bloquesToList(const char* pathFile);
void mostrarLista(t_list* listaDeBloques_);
char* recortarString(const char* stream, off_t desde, off_t size);



//BITMAP end

//void cargar_configuracion_bitmap(char bitmap_array[], int cantidadDeBytes);
//
//void setear_bloque_ocupado_en_posicion(off_t pos);//ok
//
//void setear_bloque_libre_en_posicion(off_t pos);//ok
//
//bool testear_bloque_libre_en_posicion(int pos);
//void mostrar_bitarray();
//void setBloqueActuaLleno();//agregar un 1 al bitmap.bin
//
//void getBloqueLibre_path();
//bool estaLibreElBloqueActual(FILE* bloqueActual, int tamanioDeBloque);

#endif /* FILE_SYSTEM_BITMAP_H_ */
