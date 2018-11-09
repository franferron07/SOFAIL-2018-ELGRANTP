#ifndef DTB_DTB_H_
#define DTB_DTB_H_

#include "../commons/commons.h"
#include "../config/config.h"




void inicializar_listas_dtb();
dtb_struct crear_dtb(int id_dtb, char *path);
void liberar_dtb(dtb_struct * dtb);
void liberar_recursos_dtb();
void reiniciar_dummy(); //REINICIA DTB DUMMY PARA QUE PUEDA SER INICIALIZADO POR OTRO DTB

#endif /* DTB_DTB_H_ */
