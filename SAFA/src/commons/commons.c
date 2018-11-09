#include "commons.h"



void *ultimo_elemento_lista( t_list *lista  ){

	if( list_is_empty(lista) ) return NULL;

	return list_get( lista , list_size(lista) - 1 );

}
