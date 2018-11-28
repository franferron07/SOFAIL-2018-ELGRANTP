/*
 * bitmap.c
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */
#include "bitmap.h"



//BITMAP begin
void cargar_configuracion_bitmap(char bitmap_array[], int cantidadDeBytes){
	bloqueActual_int=0;
	bitarray_=NULL;
//	static char  bitmap_array[cantidadDeBytes];
	for(int k =0;k<cantidadDeBytes;k++)bitmap_array[k]=0b00000000;
//	char* bitmap_string=malloc(cantidadDeBytes);
//	memset(bitmap_string,'0',cantidadDeBytes);//mejor con array
	printf("strlen es  %d con valor %s \n",cantidadDeBytes,bitmap_array);
	bitarray_ = bitarray_create_with_mode(bitmap_array, cantidadDeBytes, LSB_FIRST);
	bitmap_file=fopen("Bitmap.bin","w+r");
	txt_write_in_file(bitmap_file,bitarray_->bitarray);//hacerlo con mmap()
//	memmove(bitmap_file,bitarray_->bitarray,bitarray_->size);
	txt_close_file(bitmap_file);
}

void setear_bloque_ocupado_en_posicion(off_t pos){//ok
	bitarray_set_bit(bitarray_,pos);
}
void setear_bloque_libre_en_posicion(off_t pos){//ok
	bitarray_clean_bit(bitarray_,pos);
}
bool testear_bloque_libre_en_posicion(int pos){
	return bitarray_test_bit(bitarray_,(off_t)(pos));
}
void mostrar_bitarray(){
	for(int k =0;k<(bitarray_get_max_bit(bitarray_));k++)printf("test bit posicion, despues de seteo %d en pos %d \n", bitarray_test_bit(bitarray_,k),k);
}
void setBloqueActuaLleno(){//agregar un 1 al bitmap.bin
	bitarray_set_bit(bitarray_,bloqueActual_int);
}
void getBloqueLibre_path(){
	int j=getBloqueLibre_int();
	bloqueActual_int=j;
//	bloqueActual_path=malloc()
	sprintf(bloqueActual_path,"%d.bin",j);//rehacer path con punto de ontaje y carpeta segun dam
	FILE* f_aux= fopen(bloqueActual_path,"w");//txt_open_for_append(path_bloque); SI LO ABRO COMO "W" SE BORRA EL CONTENIDO
	txt_close_file(f_aux);
//	return bloqueActual_path;
}
bool estaLibreElBloqueActual(FILE* bloqueActual, int tamanioDeBloque){
	return cantidadDeCaracteres_file(bloqueActual)<tamanioDeBloque;
}
int getBloqueLibre_int(){//obtiene el proximo bloque libre ,OK
	int j;
	for( j =0;testear_bloque_libre_en_posicion(j);j++);//hasta un bloque lbre,OK

	setear_bloque_ocupado_en_posicion(j);
	if(j>metadata.cantidad_bloques){
		perror("cantidad insuficiente de espacio o bloques ");
		exit(EXIT_SUCCESS);
	}
	return j;
}
//BITMAP end



