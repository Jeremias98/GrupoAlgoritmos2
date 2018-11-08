#include "pila.h"
#include <stdlib.h>

#define CANTIDAD_INICIAL 0
#define CAPACIDAD_INICIAL 10
#define MULTIPLICADOR_REDIMENSION 2
#define DIVISOR_CHECK 4
#define DIVISOR_REDIMENSION 2 

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void) {
    pila_t* pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
        return NULL;
    }
    pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->cantidad = CANTIDAD_INICIAL;
    pila->capacidad = CAPACIDAD_INICIAL;
    return pila;    
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila); 
}

bool pila_esta_vacia(const pila_t *pila) {
    return (pila->cantidad == 0);
}

bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva) {                   //Función auxiliar para agrandar o achicar
    void** datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));   //la pila cuando sea necesario al apilar y
    if (capacidad_nueva > 0 && datos_nuevo == NULL) {                             //desapilar.  
        return false;
    }
    pila->datos = datos_nuevo;
    pila->capacidad = capacidad_nueva;
    return true;    
}

bool pila_apilar(pila_t *pila, void* valor) {
    if (pila->cantidad == pila->capacidad) {
        if (!pila_redimensionar(pila, pila->capacidad * MULTIPLICADOR_REDIMENSION)) return false;   
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad ++;
    return true;
}

void* pila_ver_tope(const pila_t *pila) {
    if (pila->cantidad == 0) return NULL;
    return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t *pila) {
    if (pila->cantidad == 0) return NULL;
    void* valor_desapilado = pila->datos[pila->cantidad - 1];
    pila->cantidad --;
    if (pila->cantidad <= pila-> capacidad / DIVISOR_CHECK) {
        size_t capacidad_nueva = pila->capacidad / DIVISOR_REDIMENSION;
        if (capacidad_nueva >= CAPACIDAD_INICIAL) pila_redimensionar(pila, capacidad_nueva);
    }
    return valor_desapilado;
}

