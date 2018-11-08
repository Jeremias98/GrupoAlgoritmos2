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

struct abb {
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter {
	pila_t* pila;
};

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

bool hijo_der(abb_nodo_t* nodo, abb_nodo_t* padre, abb_comparar_clave_t cmp) {
	return (cmp(padre->clave, nodo->clave) < 0);	
}

bool _abb_guardar(abb_t *arbol, abb_nodo_t* nodo, abb_nodo_t* padre, const char *clave, void *dato) {
	
	abb_comparar_clave_t cmp = arbol->cmp;
	// Caso nuevo elemento
	if (!nodo) {
		
		nodo = abb_nodo_crear(clave, dato);
		if (!nodo) return false;
		
		if (!padre) arbol->raiz = nodo;
		else if(hijo_der(nodo, padre, cmp)) padre->der = nodo;
		else padre->izq = nodo;
		arbol->cantidad++;
				
		return true;
		
	}
	
	// Caso ya existente
	if (cmp(nodo->clave, clave) == 0) {
		
		abb_destruir_dato_t destruir_dato = arbol->destruir_dato;
		
		if (destruir_dato) destruir_dato(nodo->dato);
		
		nodo->dato = dato;
		
		return true;
		
	}
	else if (cmp(nodo->clave, clave) < 0) return _abb_guardar(arbol, nodo->der, nodo, clave, dato);
	else return _abb_guardar(arbol, nodo->izq, nodo, clave, dato);
	
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	
	if (!arbol) return false;
	
	return _abb_guardar(arbol, arbol->raiz, NULL, clave, dato);
	
}

bool _abb_pertenece(const abb_nodo_t *nodo, abb_comparar_clave_t cmp, const char *clave) {
	
	if (!nodo) return false;
	
	if (cmp(nodo->clave, clave) == 0) return true;
	else if (cmp(nodo->clave, clave) < 0) return _abb_pertenece(nodo->der, cmp, clave);
	else return _abb_pertenece(nodo->izq, cmp, clave);
	
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	
	if (!arbol) return false;
	
	return _abb_pertenece(arbol->raiz, arbol->cmp, clave);
	
}

void* _abb_obtener(abb_nodo_t* nodo, abb_comparar_clave_t cmp, const char *clave) {
	
	if (!nodo) return NULL;
	
	if (cmp(nodo->clave, clave) == 0) return nodo->dato;
	else if (cmp(nodo->clave, clave) < 0) return _abb_obtener(nodo->der, cmp, clave);
	else return _abb_obtener(nodo->izq, cmp, clave);
	
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	
	if (!arbol) return NULL;
	
	return _abb_obtener(arbol->raiz, arbol->cmp, clave);
}

abb_nodo_t* buscar_nodo_y_padre(abb_nodo_t* nodo, abb_comparar_clave_t cmp,const char* clave, abb_nodo_t** padre, abb_nodo_t* _padre) {
	
	if (!nodo) return NULL;
	
	if (cmp(nodo->clave, clave) == 0) {
		*padre = _padre;
		return nodo;
	}
	else if (cmp(nodo->clave, clave) < 0) return buscar_nodo_y_padre(nodo->der, cmp, clave, padre, nodo);
	else return buscar_nodo_y_padre(nodo->izq, cmp, clave, padre, nodo);
}

void actualizar_referencia(abb_t* arbol, abb_nodo_t* actual, abb_nodo_t* padre, abb_nodo_t* hijo) {
	
	if(!padre) arbol->raiz = hijo;
	else if (hijo_der(actual, padre, arbol->cmp)) padre->der = hijo;
	else padre->izq = hijo; 
}

abb_nodo_t* buscar_reemplazante(abb_nodo_t* nodo) {
	
	if(!nodo->izq) return nodo;
	return buscar_reemplazante(nodo->izq);
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	
	if(!arbol) return NULL;

	abb_nodo_t* padre = NULL;
	abb_nodo_t* nodo = buscar_nodo_y_padre(arbol->raiz, arbol->cmp, clave, &padre, NULL);

	if (!nodo) return NULL;
	
	void* dato = nodo->dato;

	if (!(nodo->izq && nodo->der)) {  // CASOS SIN HIJOS Y 1 HIJO
		if(nodo->izq) actualizar_referencia(arbol, nodo, padre, nodo->izq);
		else actualizar_referencia(arbol, nodo, padre, nodo->der);
		free(nodo->clave);
		free(nodo);
		arbol->cantidad --;
	}
	else { // CASO DOS HIJOS
		abb_nodo_t* reemplazante = buscar_reemplazante(nodo->der);
		char* clave_r = strdup(reemplazante->clave);
		nodo->dato = abb_borrar(arbol, clave_r);
		free(nodo->clave);
		nodo->clave = clave_r;
	}
	return dato;
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


