#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"

#include <string.h>
#include <stdio.h>

typedef struct abb_nodo {
	struct abb_nodo* izq;
	struct abb_nodo* der;
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

abb_nodo_t* abb_nodo_crear(const char* clave, void* dato) {
	
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	
	nodo->izq = NULL;
	nodo->der = NULL;
	
	nodo->clave = strdup(clave);
	nodo->dato = dato;
	
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

bool _abb_guardar(abb_t *arbol, abb_nodo_t* nodo, const char *clave, void *dato) {
	
	// Caso nuevo elemento
	if (!nodo) {
		
		nodo = abb_nodo_crear(clave, dato);
		if (!nodo) return false;
		
		if (!arbol->raiz) arbol->raiz = nodo;
		
		arbol->cantidad++;
				
		return true;
		
	}
	
	abb_comparar_clave_t cmp = arbol->cmp;
	
	// Caso ya existente
	if (cmp(nodo->clave, clave) == 0) {
		
		abb_destruir_dato_t destruir_dato = arbol->destruir_dato;
		
		if (destruir_dato) destruir_dato(nodo->dato);
		
		nodo->dato = dato;
		
		return true;
		
	}
	else if (cmp(nodo->clave, clave) > 0) return _abb_guardar(arbol, nodo->der, clave, dato);
	else return _abb_guardar(arbol, nodo->izq, clave, dato);
	
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	
	if (!arbol) return false;
	
	return _abb_guardar(arbol, arbol->raiz, clave, dato);
	
}

bool _abb_pertenece(const abb_nodo_t *nodo, abb_comparar_clave_t cmp, const char *clave) {
	
	if (!nodo) return false;
	
	if (cmp(nodo->clave, clave) == 0) return true;
	else if (cmp(nodo->clave, clave) > 0) return _abb_pertenece(nodo->der, cmp, clave);
	else return _abb_pertenece(nodo->izq, cmp, clave);
	
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	
	if (!arbol) return false;
	
	return _abb_pertenece(arbol->raiz, arbol->cmp, clave);
	
}

void* _abb_obtener(abb_nodo_t* nodo, abb_comparar_clave_t cmp, const char *clave) {
	
	if (!nodo) return NULL;
	
	if (cmp(nodo->clave, clave) == 0) return nodo->dato;
	else if (cmp(nodo->clave, clave) > 0) return _abb_obtener(nodo->der, cmp, clave);
	else return _abb_obtener(nodo->izq, cmp, clave);
	
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	
	if (!arbol) return NULL;
	
	return _abb_obtener(arbol->raiz, arbol->cmp, clave);
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	return NULL;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cantidad;
}

void _abb_destruir(abb_nodo_t *nodo, abb_destruir_dato_t destruir_dato) {
	
	if (!nodo) return;
	
	_abb_destruir(nodo->izq, destruir_dato);
	_abb_destruir(nodo->der, destruir_dato);
	
	
	free(nodo->clave);
	if (destruir_dato) destruir_dato(nodo->dato);
	free(nodo);
	
	
}

void abb_destruir(abb_t *arbol) {
	
	if (!arbol) return;
	
	printf("Destruir\n");
	
	_abb_destruir(arbol->raiz, arbol->destruir_dato);
	
	free(arbol);
	
}

void _abb_in_order(abb_nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra) {
	
	if (!nodo) return;
	
	_abb_in_order(nodo->izq, visitar, extra);
	
	visitar(nodo->clave, nodo->dato, extra);
	
	_abb_in_order(nodo->der, visitar, extra);
	
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	
	if (!arbol) return;
	
	_abb_in_order(arbol->raiz, visitar, extra);
	
}


