#include "console.h"

void print_header(char* nombre_modulo, t_log *log) {
	log_info(log,
			"\n\t\e[31;1m===========================================\e[0m\n"
					"\t.::	Bievenido a El Gran TP - %s	::."
					"\n\t\e[31;1m===========================================\e[0m",
			nombre_modulo);
}

void print_footer(char* nombre_modulo, t_log *log) {
	log_info(log,
			"\n\t\e[31;1m============================================================\e[0m\n"
					"\t.::	Gracias por utilizar El Gran TP, %s terminada.	::."
					"\n\t\e[31;1m============================================================\e[0m",
			nombre_modulo);
}
