/*
 ============================================================================
 Name        : MDJ.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "MDJ.h"
#define str_length(string) ((string==NULL)?0:strlen(string))

pthread_t hilo_escucha_v2;
pthread_t hilo_consola_fifa_v2;

pthread_t hilo_inicio_aux;




int main(void) {
	pthread_create(&hilo_inicio_aux, NULL, cargarConfiguracionMDJ,NULL);
	pthread_join(hilo_inicio_aux, NULL);
	pthread_attr_destroy(&hilo_inicio_aux);
	puts("despuees de inicializar");//hasta aca oK, probado

//	pthread_create(&hilo_escucha_v2, NULL,escuchaMensajes,NULL);
////	pthread_create(&hilo_consola_fifa_v2, NULL, &consola_fifa, NULL);
//
////	pthread_join(&hilo_escucha_v2, NULL);
////	pthread_join(&hilo_consola_fifa_v2,NULL);
//
//	pthread_attr_destroy(&hilo_escucha_v2);
////	pthread_attr_destroy(&hilo_consola_fifa_v2);
//
//	consola_fifa();

	obtener_datos("scripts/la_12.escriptorio",0,200);
	printf("%d \n",(validarArchivo("scripts/la_12.escriptorio")));
////
	char panadero[]="scripts/riBerGanoPor.escriptorio";//unPath puede ser char* tambien
//
	puts("creaando archivo");
	crearArchivo(panadero,400);//400 bytes reservados,OK
	puts("guardando datos");
	guardar_datos(panadero,0,strlen("Buenas Noches!"),"Buenas Noches!");
////	guardar_datos(panadero,strlen("Buenas Noches!"),5,"QWERT");
////	guardar_datos(panadero,strlen("Buenas Noches!")+5,1,"A");
//puts("haberr");
	puts("obteniedo datos");
	obtener_datos(panadero,3,10);
////	validarArchivo(panadero);
////	printf("%d \n",(validarArchivo(panadero)));//tira false ,hay problemas con el manejo de bits
	puts("por borrar archivo");
	borrarArchivo(panadero);//ok

	finalizarMDJ();

	puts("fin");
	return 0;
}

void cargarConfiguracionMDJ(){

	cargar_configuracion_mdj("mdj.config");
	mostrar_configuracion_mdj();
	cargar_configuracion_metadata("fifa-examples/fifa-entrega/Metadata/Metadata.bin");
	mostrar_configuracion_metadata();
	cargar_configuracion_bitmap();

//	setear_bloque_ocupado_en_posicion(62);
//	setear_bloque_ocupado_en_posicion(61);
//	setear_bloque_ocupado_en_posicion(5);
//	setear_bloque_ocupado_en_posicion(200);



//	mostrar_bitarray();//funciona , pero comento por que me ocupa pantalla
}
void finalizarMDJ(){
	bitarray_destroy(bitarray_);
}
void consola_fifa(){
	puts("press \"exit\" para salir de consola ");
	loop{
			buffer_input_keyboard=readline("fifa@mdj=>  ");
			if(!strncmp(buffer_input_keyboard, "exit", 4)) break;
			ejecutar_linea_entrante(buffer_input_keyboard);
			free(buffer_input_keyboard);
	}
}
void  ejecutar_linea_entrante(char* buffer_entrante){
	system(buffer_entrante);//la consola fifa solo ejecuta esto, ya que solo sirve si estan persistidos bien
//	switch (buffer_entrante) {
//		case "md5":
//			perror("use md5sum y no md5");
//			break;
//		default:
//			break;
//	}
//	persistirContenido(buffer_entrante);
}

//INTERFAZ MDJ
bool validarArchivo(const char* pathDelArchivo){//ver si existe el archivo, OK, se puede borrar todos los printf() y puts(),era para probar
  	puts("---------validacion de archivo-------");
	int contador_bloques_aux=0;
  	int cantidadDeBloquesPersistidos=0,bytesOcupados=0;

  	char* pathCompleto_aux=malloc(2000);
  	sprintf(pathCompleto_aux,"%s%s%s",mdj.punto_de_montaje,"Archivos/",pathDelArchivo);
  		t_config* config_aux=config_create(pathCompleto_aux);

  		if (config_aux==NULL) {
  			fprintf(stderr,"->validarArchivo() ,no existe el archivo o path incorrecto : %s \n",pathDelArchivo);
  			return false;
		}
  		bytesOcupados=config_get_int_value(config_aux,"TAMANIO");
  		cantidadDeBloquesPersistidos=(bytesOcupados/metadata.tamanio_de_bloque);
  		if((bytesOcupados%metadata.tamanio_de_bloque)!=0)cantidadDeBloquesPersistidos++;//esto es importante ,es como saber el numero de paginas ocupadasb
  		t_list *listaDeBloques=bloquesToList(pathCompleto_aux);
  		for (int var = 0; var < cantidadDeBloquesPersistidos; var++) {
  			if(testear_bloque_libre_en_posicion(list_get(listaDeBloques,var)))contador_bloques_aux++;
  		}
  		list_destroy(listaDeBloques);
  		free(pathCompleto_aux);
  		puts("-------------fin validacion de archivo-------------------");
	return contador_bloques_aux==cantidadDeBloquesPersistidos;
}
char* obtener_datos(const char* pathDelArchivo,int offset, int size){//Ok
	puts("begin obtenerDatos()");
//	if(validarArchivo(pathDelArchivo)){
//		fprintf(stderr,"->obtener_datos() no se puede validar path : %s",pathDelArchivo);
//		return NULL;
//	}
//	else{
		char * unScript = bloquesToString(pathDelArchivo);//este ya se encarga de resolver el path
		char * unScriptParcial =recortarString(unScript,offset,size);
		free(unScript);
		puts(unScriptParcial);
		puts("end obtenerDatos()");
		return unScriptParcial;

//	}
}
void crearArchivo( const char* pathDelArchivo,int cantidadDeBytesDelArchivoAReservar){//OK
	puts("---------crearArchivo()-------------------");
	int cantidadDeBloquesAOcupar=((cantidadDeBytesDelArchivoAReservar)/metadata.tamanio_de_bloque);
	if((cantidadDeBytesDelArchivoAReservar%metadata.tamanio_de_bloque)!=0)cantidadDeBloquesAOcupar++;

	if(getCantidadDeBloquesLibres()<cantidadDeBloquesAOcupar || cantidadDeBloquesAOcupar<0)perror("No se puede crearArchivo(), espacio o bloques insuficientes");
	else{
//		crearBloques(bloques_a_ocupar);
		char* path_completo_aux = malloc(1500);
		sprintf(path_completo_aux,"%s%s%s",mdj.punto_de_montaje,"Archivos/",pathDelArchivo);
		FILE* soloParaCrearArchivo_=fopen(path_completo_aux,"w+");

//		t_config *config_de_inicializacion_aux = config_create(path_completo_aux);//COMENTO CONFIG,POR  QUE FALLA , DEBERIA DE FUNCIONAR
//		if(config_de_inicializacion_aux==NULL)fprintf(stderr,"error en crearArchivo(), en config para path %s \n",path_completo_aux);
		printf("bloques a ocupar :%d  en %s \n", cantidadDeBloquesAOcupar,path_completo_aux);
		free(path_completo_aux);
		printf("TAMANIO :%d \n", cantidadDeBytesDelArchivoAReservar);
		fprintf(soloParaCrearArchivo_,"TAMANIO=%d\n",cantidadDeBytesDelArchivoAReservar);
		char* listaDeBloquesParaConfig = getBloquesLibres_string(cantidadDeBloquesAOcupar);
		printf("BLOQUES: %s \n",listaDeBloquesParaConfig);
		fprintf(soloParaCrearArchivo_,"BLOQUES=%s\n",listaDeBloquesParaConfig);
//		config_set_value(config_de_inicializacion_aux,"BLOQUES",listaDeBloquesParaConfig);
//		config_set_value(config_de_inicializacion_aux,"TAMANIO",intToString(cantidadDeBytesDelArchivoAReservar));//preguntar  el tercer parametro
//		config_save_in_file(config_de_inicializacion_aux,pathDelArchivo);
//		config_destroy(config_de_inicializacion_aux);//comento por que no lo persiste con config_save
		free(listaDeBloquesParaConfig);
		fclose(soloParaCrearArchivo_);
	}
	puts("--------------fin crearArchivo()-----------");
}
void borrarArchivo(const char* pathDelArchivo){//ok
	puts("----------------borrarArchivo()---------");
	int cantidadDeBloques=0,bytesOcupados=0;
	char* path_completo_aux=malloc(1050);
			sprintf(path_completo_aux,"%s%s%s",mdj.punto_de_montaje,"Archivos/",pathDelArchivo);
	t_config* aux=config_create(path_completo_aux);
	if(aux==NULL){
		perror("->borrarArchivo() , no existe el archivo o path incorrecto");
		puts("-------------fin de borrarArchivo()----------");
		free(path_completo_aux);
	}
	else {
		bytesOcupados=config_get_int_value(aux,"TAMANIO");
		cantidadDeBloques=(bytesOcupados/metadata.tamanio_de_bloque)+1;
		printf(" cantidad de bloques a borrar: %d \n",cantidadDeBloques);
		char** bloques_aux= config_get_array_value(aux,"BLOQUES");
		for(int var=0;var<cantidadDeBloques;var++){
			setear_bloque_libre_en_posicion(atoi(bloques_aux[var]));
		}
		remove(path_completo_aux);
		free(bloques_aux);
		config_destroy(aux);
		puts("-------------fin de borrarArchivo()----------");
		free(path_completo_aux);
	}
}
void guardar_datos(const char* pathDelArchivo,int offset,int size,const char* contenidoAGuardar){
	char* pathCompletoDelArchivoAModificarOLeer_aux=malloc(1300);//de ejemplo , me importa que albergue el path completo


	sprintf(pathCompletoDelArchivoAModificarOLeer_aux,"%s%s%s",mdj.punto_de_montaje,"Archivos/",pathDelArchivo);
		int cantidadDeBloquesInvolucrados_y_ReservadosEnElPath=getCantidadDeBloquesOcupadosSegunPath(pathCompletoDelArchivoAModificarOLeer_aux);
		t_list* listaDeBloquesSegunPath=bloquesToList(pathCompletoDelArchivoAModificarOLeer_aux);
		mostrarLista(listaDeBloquesSegunPath);
	//	FILE* bloquesInvolucrados[cantidadDeBloquesInvolucrados_y_ReservadosEnElPath];
//		t_config* config_aux=config_create()
		int offset_variable=offset;
		char* contenido_buffer_variable=strdup(contenidoAGuardar);
		size_t totalDeBytesReservados = metadata.tamanio_de_bloque*cantidadDeBloquesInvolucrados_y_ReservadosEnElPath;
			if(totalDeBytesReservados< (offset+size)){
				fprintf(stderr,"no hay bytes disponibles o bloques insuficientes para \"%s\" \n",pathDelArchivo);
			}
		for (int bloque_i_esimo = 0; bloque_i_esimo < cantidadDeBloquesInvolucrados_y_ReservadosEnElPath || str_length(contenido_buffer_variable)>0; bloque_i_esimo++) {
			sprintf(pathCompletoDelArchivoAModificarOLeer_aux,"%s%s%d.bin",mdj.punto_de_montaje,"Bloques/",atoi(list_get(listaDeBloquesSegunPath,bloque_i_esimo)));
	//		bloquesInvolucrados[bloque_i_esimo]=fopen(pathCompletoDelBloqueAModificar_aux,"r+w");
//			salto1:
			persistirAlBloque(pathCompletoDelArchivoAModificarOLeer_aux,&offset_variable,contenido_buffer_variable);
//			if(!seNecesitaOtroBloque){
//	//			fclose(bloquesInvolucrados[bloque_i_esimo]);
////				break;
//				goto salto1;
//			}
		}
		free(pathCompletoDelArchivoAModificarOLeer_aux);
		list_destroy(listaDeBloquesSegunPath);
}

void persistirAlBloque(const char* pathDeUnBloque,off_t * offset, char* buffer){
	int tamanioBloque =metadata.tamanio_de_bloque;

//
	if(tamanioBloque<*offset){
		*offset=*offset-tamanioBloque;
		fprintf(stderr,"se necesita el siguiente bloque, bloque \"%s\"  esta ocupado ;\n",pathDeUnBloque);
		return ;//necesito el siguiente bloque
	}
	else {
		bool seNecesitaElSiguienteBloque=false;
		size_t size_del_buffer=str_length(buffer);
		FILE* unFile=fopen(pathDeUnBloque,"r+w");
		fseek(unFile,*offset,SEEK_SET);
		size_t cantidadAPersistir=minimo(tamanioBloque-*offset,str_length(buffer));//,(size_del_buffer);//+*offset-1);
		char* bufferParcial=recortarPrimerosCaracteres(buffer,cantidadAPersistir);
		fprintf(unFile,"%s",bufferParcial);
		free(bufferParcial);
		fclose(unFile);
		return ;//seNecesitaElSiguienteBloque;
	}
}

char* getBloquesLibres_string(int cantidadDeBloques){//OK,solo para crearArchivo ,da en formato "[1,2,3,54,56,6]" de ejemplo
//	char* bloques_string=(char*)malloc(cantidadDeBloques*sizeof(char)*3);
	char* aux=strdup("[");
	t_list *bloques_libres_aux=getBloquesLibres_list(cantidadDeBloques);
	aux=str_concat(aux,intToString((int)list_get(bloques_libres_aux,0)));
	if(cantidadDeBloques==1){
			aux=str_concat(aux,"]");
			list_destroy(bloques_libres_aux);
			return aux;
	}
	for(int var = 1; var < cantidadDeBloques; var++) {
			aux=str_concat(aux,",");
			aux=str_concat(aux,intToString((int)list_get(bloques_libres_aux,var)));
	}
		list_destroy(bloques_libres_aux);
		aux=str_concat(aux,"]");
	return aux;
}

t_list* getBloquesLibres_list(int cantidadDeBloques){
	t_list *lista=list_create();
	for(int p=0;p<cantidadDeBloques;p++ ){
		list_add(lista,getBloqueLibre_int());
	}
	return lista;
}
int getCantidadDeBloquesLibres(){//ok
	int aux,  contador=0;
	for(aux=0;aux<metadata.cantidad_bloques;aux++){
		if(testear_bloque_libre_en_posicion(aux)==0)contador++;
	}
	return contador;
}


//INTERFAZ MDJ







