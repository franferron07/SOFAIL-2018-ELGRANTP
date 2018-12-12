/*
 * bitmap.c
 *
 *  Created on: 15 nov. 2018
 *      Author: jhondaniel
 */
#include "bitmap.h"



//BITMAP begin
void cargar_configuracion_bitmap(){
	bloqueActual_int=0;
	bitarray_=NULL;
//	char bits[metadata.cantidad_bloques/8];//bits es una variable auxiliar
//	char* bitmap_array=strdup(bits);
//	int cantidadDeBytes=metadata.cantidad_bloques/8;
//	for(int k =0;k<cantidadDeBytes;k++)bitmap_array[k]=0b00000000;
//	printf("strlen es  %d con valor %s \n",cantidadDeBytes,bitmap_array);
	char* bits=fileToString_v2("fifa-examples/fifa-entrega/Metadata/Bitmap.bin");
	bitarray_ = bitarray_create_with_mode(bits,metadata.cantidad_bloques/8,MSB_FIRST);// LSB_FIRST);
//	bitmap_file=fopen("Bitmap.bin","r+w");
//	txt_write_in_file(bitmap_file,bitarray_->bitarray);//hacerlo con mmap()
////	memmove(bitmap_file,bitarray_->bitarray,bitarray_->size);
//	txt_close_file(bitmap_file);

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
char* bloquesToString(const char* pathFile){//ok
//	puts("begin bloquesToString()");
	char* bloques=NULL;
	char* path_aux = malloc((5024)*sizeof(char));
	sprintf(path_aux,"%s%s%s",mdj.punto_de_montaje,"Archivos/",pathFile);
//	puts(path_aux);
	t_list* listaDeBloques =  bloquesToList(path_aux);
//	mostrarLista(listaDeBloques);
	for (int var = 0; var < list_size(listaDeBloques); var++) {
		sprintf(path_aux,"%s%s%d.bin",mdj.punto_de_montaje,"Bloques/",list_get(listaDeBloques,var));
//		puts(path_aux);
		bloques=str_concat(bloques,fileToString_v2(path_aux));
	}
	free(path_aux);
	list_destroy(listaDeBloques);
//	puts("end bloquesToString()");
	return bloques;

}
t_list* bloquesToList(const char* pathFile){//Ok
	t_config* scriptGuardado=config_create(pathFile);
		if(scriptGuardado==NULL)fprintf(stderr,"-> bloquesToList() , no existe path:%s\n",pathFile);
			char** listaDeBloques=config_get_array_value(scriptGuardado,"BLOQUES");
			config_destroy(scriptGuardado);
			t_list* listaDeBloques_ = list_create();
			for(int aux=0;listaDeBloques[aux]!=NULL;aux++){
				int unBloque = atoi(listaDeBloques[aux]);
				list_add(listaDeBloques_,unBloque);
			}
	string_trim(listaDeBloques);//free() para char**
	return listaDeBloques_;
}
void mostrarLista(t_list* listaDeBloques_){
	puts("--inicion mostrando lista de  bloques ");
	for(int p =0; p<list_size(listaDeBloques_);p++){
				puts(intToString(list_get(listaDeBloques_,p)));
			}
	printf("--fin muestra de lista de bloques, total bloques : %d\n",list_size(listaDeBloques_));

}
char* recortarString(const char* stream, off_t desde, off_t size){
	char* parcial = string_substring(stream,(off_t)desde,(off_t)size);
	return parcial;
}
//BITMAP end



