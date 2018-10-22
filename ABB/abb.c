#include "abb.h"
#include "pila.h"

typedef struct abb_nodo {
	abb_nodo_t* izq;
	abb_nodo_t* der;
	char* clave;
	void* dato;
} abb_nodo_t;

typedef struct abb {
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
} abb_t;

typedef struct abb_iter {
	pila_t* pila;
} abb_iter_t;

abb_nodo_t* abb_nodo_crear(char* clave, void* dato) {
	
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	
	nodo->izq = NULL;
	nodo->der = NULL;
	
	nodo->clave = clave;
	nodo->valor = valor;
	
	return nodo;
	
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	
	abb_t* arbol = malloc(sizeof(abb_t));
	if (!arbol) return NULL;
	
	arbol->raiz = NULL;
	
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	
	arbol->cantidad = 0;
	
	return arbol;
	
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	
	if (!arbol) return;
	
	abb_in_order(arbol->raiz->izq, visitar, extra);
	
	visitar(arbol->raiz->clave, arbol->raiz->dato, extra);
	
	abb_in_order(arbol->raiz->der, visitar, extra);
	
}
