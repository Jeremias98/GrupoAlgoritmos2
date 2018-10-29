#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void abb_nodo_destruir(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato) {
	
	if (!nodo) return;
	
	free(nodo->clave);
	if (destruir_dato) destruir_dato(nodo->dato);
	free(nodo);
	
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

abb_nodo_t* abb_insertar(abb_t *arbol, abb_nodo_t* nodo, const char *clave, void *dato) {
	
	abb_destruir_dato_t destruir_dato = arbol->destruir_dato;

	if (!nodo) {
		arbol->cantidad++;
		return abb_nodo_crear(clave, dato);
	}
	
	if (arbol->cmp(nodo->clave, clave) == 0) {
		if (destruir_dato) destruir_dato(nodo->dato);
		nodo->dato = dato;
	}
	else if (arbol->cmp(nodo->clave, clave) < 0) { 
		nodo->der = abb_insertar(arbol, nodo->der, clave, dato);
	}
	else { 
		nodo->izq = abb_insertar(arbol, nodo->izq, clave, dato);
	}
	
	return nodo;
	
}

abb_nodo_t* _abb_obtener(abb_nodo_t* nodo, abb_comparar_clave_t cmp, const char *clave) {
	
	if (!nodo) return NULL;
	
	if (cmp(nodo->clave, clave) < 0) return _abb_obtener(nodo->der, cmp, clave);
	else if (cmp(nodo->clave, clave) > 0) return _abb_obtener(nodo->izq, cmp, clave);
	else return nodo;
	
}

abb_nodo_t* get_reemplazante(abb_nodo_t* nodo) {
	
	abb_nodo_t* actual = nodo;
	
	while(actual->izq) {
		actual = actual->izq;
	}
	
	return actual;
	
}

// #### PRIMITIVAS #### //
bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	
	if (!arbol) return false;

	if (!arbol->raiz) {
		arbol->raiz = abb_nodo_crear(clave, dato);
		arbol->cantidad++;
		return true;
	}
	
	return abb_insertar(arbol, arbol->raiz, clave, dato);
	
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	
	if (!arbol) return false;
	
	return _abb_obtener(arbol->raiz, arbol->cmp, clave) ? true : false;
	
}


void *abb_obtener(const abb_t *arbol, const char *clave) {
	
	if (!arbol) return NULL;
	
	abb_nodo_t* obtenido = _abb_obtener(arbol->raiz, arbol->cmp, clave);
	
	if (!obtenido) return NULL;
	
	return obtenido->dato;
}


abb_nodo_t* _abb_borrar(abb_t* arbol, abb_nodo_t *nodo, const char *clave) {
	
	if (!nodo) return NULL;
	
	if (arbol->cmp(nodo->clave, clave) > 0) {
		nodo->izq = _abb_borrar(arbol, nodo->izq, clave);
	}
	else if (arbol->cmp(nodo->clave, clave) < 0) {
		nodo->der = _abb_borrar(arbol, nodo->der, clave);
	}
	else {
		
		// No hay nodo izquierdo, obtengo el hijo der
		if (!nodo->izq) {
			
			abb_nodo_t* temp = nodo->der;
			
			free(nodo->clave);
			free(nodo);
			
			arbol->cantidad--;
			
			return temp;
			
		}
		// Viceversa
		else if (!nodo->der) {
			
			abb_nodo_t* temp = nodo->izq;
			
			free(nodo->clave);
			free(nodo);
				
			arbol->cantidad--;
			return temp;
		}
		
		// Caso dos hijos
		abb_nodo_t* temp = get_reemplazante(nodo->der);
		
		free(nodo->clave);
		nodo->clave = strdup(temp->clave);
		nodo->dato = temp->dato;
		
		nodo->der = _abb_borrar(arbol, nodo->der, temp->clave);
		
	}
	
	return nodo;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	
	if (!arbol) return NULL;
	
	void* dato_borrado = abb_obtener(arbol, clave);
	
	arbol->raiz = _abb_borrar(arbol, arbol->raiz, clave);
	
	return dato_borrado;
	
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

bool _abb_in_order(abb_nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra) {
	
	if (!nodo) return true;
	
	if(!_abb_in_order(nodo->izq, visitar, extra)) return false;
	
	if (visitar) {
		if (!visitar(nodo->clave, nodo->dato, extra)) return false;
	}

	if(!_abb_in_order(nodo->der, visitar, extra)) return false;

	return true;
	
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	
	if (!arbol) return;
	
	_abb_in_order(arbol->raiz, visitar, extra);
	
}

// Iterador INORDER //

void apilar_inorder(pila_t* pila, abb_nodo_t* raiz) {
	while (raiz) {
		pila_apilar(pila, raiz);
		raiz = raiz->izq;
	}
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;
	
	iter->pila = pila_crear();
	if (!iter->pila) return NULL;
	
	// Apilo raiz y todos los hijos izquierdos
	apilar_inorder(iter->pila, arbol->raiz);
	
	return iter;
	
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	
	if (abb_iter_in_al_final(iter)) return false;
	
	abb_nodo_t* desapilado = pila_desapilar(iter->pila);
	
	// Apilo hijo derecho del desapilado
	// y todos los hijos izquierdos
	apilar_inorder(iter->pila, desapilado->der);
	
	return true;
	
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	abb_nodo_t* actual = pila_ver_tope(iter->pila);
	if (!actual) return NULL;
	return actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	
	pila_destruir(iter->pila);
	
	free(iter);
	
}
