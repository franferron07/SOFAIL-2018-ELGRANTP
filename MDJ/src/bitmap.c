/*
 * bitmap.c
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */
#include "bitmap.h"









void configurar_bitmap(int cantidadDeBytes){
	char bitmap_array[cantidadDeBytes];
	for(int i =0;i<cantidadDeBytes;i++)bitmap_array[i]=0;
	bitarray = bitarray_create_with_mode(bitmap_array, sizeof(bitmap_array), LSB_FIRST);
	bitmap_file=txt_open_for_append("Bitmap.bin");
	txt_write_in_file(bitmap_file,bitarray->bitarray);//hacerlo con mmap()
	txt_close_file(bitmap_file);
//	bitarray_destroy(bitarray);
}
void setear_bloque_ocupado_en_posicion(int pos){
	bitarray_set_bit(bitarray,(off_t)(pos));
}
bool testear_bloque_libre_en_posicion(int pos){
	return bitarray_test_bit(bitarray,(off_t)(pos));
}
void mostrar_bitarray(int cantidadDeBloques){
	for(int k =0;k<cantidadDeBloques;k++)printf("test bit posicion, despues de seteo %d en pos %d \n", bitarray_test_bit(bitarray,k),k);
}
void setBloqueActuaLleno(){//agregar un 1 al bitmap.bin
	bitarray_set_bit(bitarray,bloqueActual_int);
}



FILE* getBloqueLibre_file(){
	int i;
	for( i =0;testear_bloque_libre_en_posicion(i);i++);//hasta un bloque lbre
	char* path_del_bloque_libre = malloc(1000);
	sprintf(path_del_bloque_libre,"%d.bin",i);//rehacer path con punto de ontaje y carpeta segun dam
	bloqueActual_file = fopen(path_del_bloque_libre,"w");//txt_open_for_append(path_bloque); SI LO ABRO COMO "W" SE BORRA EL CONTENIDO
	free(path_del_bloque_libre);
	return bloqueActual_file;
}
bool estaLibreElBloqueActual(FILE* bloqueActual, int tamanioDeBloque){
	return cantidadDeCaracteres_file(bloqueActual)<tamanioDeBloque;
}

