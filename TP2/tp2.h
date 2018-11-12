#include <stdbool.h>
#include "abb.h"
#include "hash.h"


typedef struct vuelo vuelo_t;

vuelo_t* vuelo_crear(char* strv[], vuelo_t* vuelo_actualizar);

void free_datos_vuelo(vuelo_t* vuelo);

void vuelo_destruir(vuelo_t* vuelo);

char** strv_comandos();

int num_comando(const char* str, char** strv_operadores);

bool agregar_archivo(const char* ruta, abb_t* abb, hash_t* hash);

bool imprimir_info_vuelo(const char* flight_n, hash_t* hash);



