#include <stdbool.h>
#include "abb.h"
#include "hash.h"
#include "pila.h"

typedef struct vuelo vuelo_t;

// CREA STRUCT VUELO CON LOS DATOS CONTENIDOS EN STRV, SI SE PASA
// VUELO_ACTUALIZAR LO MODIFICA CON LOS DATOS DE STRV.
vuelo_t* vuelo_crear(char* strv[], vuelo_t* vuelo_actualizar);

// LIBERA LOS TODOS LOS ELEMENTOS DEL VUELO (NO DESTRUYE EL STRUCT).
void free_datos_vuelo(vuelo_t* vuelo);

// LIBERA TODOS LOS ELEMENTOS DEL VUELO Y EL STRUCT.
void vuelo_destruir(vuelo_t* vuelo);

// GENERA UNA STRV CON TODOS LOS COMANDOS VALIDOS.
char** strv_comandos();

// DEVUELVE UN INT SI COINCIDE CON ALGUN COMANDO VALIDO.
// 1:AGR_ARCH; 2:VER_TAB; 3: INFO_V; 4:PRIORI; 5:BORRAR; 0:COMANDO INCORRECTO.
int num_comando(const char* str, char** strv_operadores);

// DEVUELVE LA CANTIDAD DE PARAMETROS INGRESADOS EN LA LINEA
// CADA UNO DE ELLOS ES UN ELEMENTO DE STRV.
size_t n_parametros(char* strv[]);

// DEVUELVE TRUE SI EL STR INGRESADO ES UN NUMERO NATURAL.
// FALSE CASO CONTRARIO.
bool str_es_num(const char* str);


// MODIFICA EL STR CLAVE Y LE DA EL FORMATO <STR1 - STR2>
// PRE: EL STR CLAVE NO DEBE TENER DATOS Y DEBE TENER ESPACIO SUFICIENTE.
void crear_clave(char* clave, char* str1, char* str2);

// FUNCION PARA COMANDO AGREGAR_ARCHIVO
bool agregar_archivo(const char* ruta, abb_t* abb, hash_t* hash);

// IMPRIME LA PILA DE FORMA ASCENDENTE (DEL ULTIMO AL PRIMERO) SI ASC  ES TRUE
// CASO CONTRARIO IMPRIME DE FORMA DESCENDIENTE. SI LIBERAR ES TRUE DESTRUYE
// LOS ELEMENTOS DESAPILADOS.
void imprimir_pila_asc(pila_t* p_result, bool asc, bool liberar);

// FUNCION PARA COMANDO VER_TABLERO.
bool ver_tablero(char* k, char* modo, char* desde, char* hasta, abb_t* abb);

// FUNCION PARA COMANDO INFO_VUELO.
bool imprimir_info_vuelo(const char* flight_n, hash_t* hash);

// FUNCION DE COMPARACION PARA ARMAR EL HASH DE MINIMOS.
// DE CLAVES CON FORMATO <PRIORIDAD - N_VUELO>.
int cmp_prioridad (const void* str1, const void* str2);

// FUNCION PARA COMANDO PRIORIDAD_VUELOS.
bool prioridad_vuelos(char* k, hash_t* hash);

// FUNCION PARA COMANDO BORRAR.
bool borrar(char* fecha_desde, char* fecha_hasta, hash_t* hash, abb_t* abb);


