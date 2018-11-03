#include "utilitaria.h"

void _finalizar_cadena(char *cadena) {
	if ((strlen(cadena) > 0) && (cadena[strlen(cadena) - 1] == '\n'))
		cadena[strlen(cadena) - 1] = '\0';
}

bool _esta_vacia(char* cadena) {
	return cadena == NULL || string_is_empty(cadena)
			|| string_contains(cadena, " ");
}

bool _es_cadena_valida(char* cadena) {
	if (_esta_vacia(cadena)) {
		printf("El parametro esta vacio o contiene espacios\n");
		return false;
	}
	return true;
}

char* _obtener_iesimo_parametro(char** entrada, int i) {
	if (entrada[i] != NULL)
		return entrada[i];
	return "";
}

const char* get_filename(const char* path) {
	const char *file = strrchr(path, '/');
	if (!file || file == path)
		return "";
	return file + 1;
}

const char* get_file_extension(const char* filename) {
	const char *extension = strrchr(filename, '.');
	if (!extension || extension == filename)
		return "";
	return extension + 1;
}
