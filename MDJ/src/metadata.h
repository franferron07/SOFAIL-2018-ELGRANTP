/*
 * metadata.h
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */

#ifndef FILE_SYSTEM_METADATA_H_
#define FILE_SYSTEM_METADATA_H_
#include <commons/config.h>
#include <stdio.h>
#include <string.h>

struct METADATA{
	int tamanio_de_bloque;//en bytes, ni dice eso el enunciado
	int cantidad_bloques;
	char* MAGIC_NUMBER;//es fifa , puede ser un charArray, charlable, no sirve , es la extension de los archivos d config
}metadata;

void mostrar_configuracion_metadata();
void cargar_configuracion_metadata();//hardcodeada, completar con config.h y  Metadata.bin


#endif /* FILE_SYSTEM_METADATA_H_ */
