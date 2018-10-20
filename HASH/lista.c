#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct nodo {
    void* dato;
    struct nodo* prox;
}nodo_t;


struct lista {
    nodo_t* prim;
    nodo_t* ult;
    size_t largo;
};

struct lista_iter {
    nodo_t* actual;
    nodo_t* anterior;
    lista_t* lista;
};

nodo_t* crear_nodo(void* valor) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = valor;
    nodo->prox = NULL;
    return nodo;
}

lista_t* lista_crear(void) {
    lista_t* lista = malloc(sizeof(lista_t));
    
    if (!lista) return NULL;

    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    return lista;    
}

void lista_destruir(lista_t *lista, void destruir_dato(void*)) {
    while (lista->prim != NULL) {
        if (!destruir_dato) lista_borrar_primero(lista);
        else destruir_dato(lista_borrar_primero(lista));
    }   
    free(lista);
}

bool lista_esta_vacia(const lista_t *lista) {
    if (!lista->prim) return true;
    return false;
}

bool lista_insertar_primero(lista_t *lista, void* dato) {
    nodo_t* nodo = crear_nodo(dato);
    if (!nodo) return false;

    if(!lista->prim) {
        lista->prim = nodo;
        lista->ult = nodo;
    }
    else {
        nodo_t* segundo = lista->prim;
        lista->prim = nodo;
        lista->prim->prox = segundo;
    }
    lista->largo ++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void* dato) {
    nodo_t* nodo = crear_nodo(dato);
    if (!nodo) return false;

    if(!lista->prim) {
        lista->prim = nodo;
        lista->ult = nodo;
    }
    else {
        lista->ult->prox = nodo;
        lista->ult = nodo;
    }
    lista->largo ++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (!lista->prim) return NULL;
    
    void* valor_borrado = lista->prim->dato;
    nodo_t* nodo_borrado = lista->prim;
    
    if (lista->prim == lista->ult) lista->ult = NULL;
    lista->prim = lista->prim->prox;
    free(nodo_borrado);
    lista->largo --;
    return valor_borrado;
}

void *lista_ver_primero(const lista_t *lista) {
    if (!lista->prim) return NULL;
    return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t *lista) {
    if (!lista->prim) return NULL;
    return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    if (!lista->prim) return;

    nodo_t* actual = lista->prim;
    while(actual) {
        if (!visitar(actual->dato, extra)) break;
        actual = actual->prox;
    }
}

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if(!iter) return NULL;

    iter->lista = lista;
    iter->actual = lista->prim;
    iter->anterior = NULL;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (!iter->actual) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (!iter->actual) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    if (iter->anterior == iter->lista->ult) return true;
    return false;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t* nodo = crear_nodo(dato);
    if (!nodo) return false;

    if (iter->lista->prim == iter->actual) iter->lista->prim = nodo;
    if (iter->lista->ult == iter->anterior) iter->lista->ult = nodo;

    nodo->prox = iter->actual;
    iter->actual = nodo;
    if (iter->anterior) iter->anterior->prox = iter->actual;
    iter->lista->largo ++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (!iter->actual) return NULL;
    
    void* valor_desencolado = iter->actual->dato;
    nodo_t* nodo_desencolado = iter->actual;
    
    if (iter->lista->prim == iter->actual) iter->lista->prim = iter->actual->prox;
    if (iter->lista->ult == iter->actual) iter->lista->ult = iter->anterior;

    iter->actual = iter->actual->prox;
    if (iter->anterior) iter->anterior->prox = iter->actual;
    free(nodo_desencolado);
    iter->lista->largo --;
    return valor_desencolado;
}