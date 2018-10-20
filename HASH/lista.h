#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void); 

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *)); 

// Devuelve verdadero o falso, según si la lista tiene o no elementos encolados.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista); 

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al inicio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato); 

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del último elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del último, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el último elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene el número de elementos que posee la lista. Si la lista está vacía,
// devuelve 0.
// Pre: la lista fue creada.
// Post: se devolvió el número de elementos de la lista.
size_t lista_largo(const lista_t *lista);

/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera cada uno de los elementos de la lista y aplica la función "visitar" al iterar
// cada elemento. Si la función "visitar" devuelve false se detiene la iteración, caso
// contrario sigue iterando.
// Pre: la lista fue creada
// Post: se iteró la lista y se le aplicó la función "visitar" a cada elemento, hasta que
// la función interrumpió la iteración o llegó al final de lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea el iterador para listas.
// Pre: se creó una lista.
// Post: devuelve el iterador para listas, el elemento actual del iterador
// es el primer elemento de la lista, si no está vacía.
lista_iter_t *lista_iter_crear(lista_t *lista);

// El iterador avanza al siguiente elemento de la lista. Si no hay un siguiente elemento
// en la lista, devuelve false.
// Pre: se creó el iterador.
// Post: el iterador avanzó al siguiente elemento de la lista, cuando haya un siguiente.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del elemento actual del iterador de la lista. Si el iterador
// tiene un elemento actual, lo devuelve, caso contrario devuelve NULL.
// Pre: se creó el iterador.
// Post: se devolvió el elemento actual del iterador, cuando tiene un actual.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// El iterador avanza hasta recorres todos los elementos de la lista. Si la lista
// está vacía o ya recorrió la lista, devuelve false.
// Pre: se creó el iterador.
// Post: el iterador recorrió todos los elementos de la lista, cuando no está vacía.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: se creó el iterador.
// Post: se eliminó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento a la lista, devuelve falso en caso de error.
// Pre: el iterador fue creado.
// Post: se agregó un nuevo elemento a la lista, el valor se encuentra antes
// del elemento anterior actual del iterador, y es el nuevo actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Saca el elemento actual del iterador de la lista. Si la lista tiene elementos, se quita el
// elemento actual del iterador, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: el iterador fue creado.
// Post: se devolvió el valor del elemento actual del iterador anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en cola.h
// (esto es, las pruebas no pueden acceder a los miembros del struct cola).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);

#endif // PILA_H
