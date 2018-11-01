#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_INICIAL 10
#define FACTOR_AGRANDAR 2
#define FACTOR_ACHICAR 0.5

typedef struct heap {
    size_t tam;
    size_t capacidad;
    void** arr;
    cmp_func_t cmp;
} heap_t;

size_t get_index_padre(size_t posicion) {
    return posicion == 0 ? -1 : (posicion - 1) / 2;
}

size_t get_index_der(size_t posicion) {
    return 2 * posicion + 2;
}

size_t get_index_izq(size_t posicion) {
    return 2 * posicion + 1;
}

/*
void swap(void* valor_1, void* valor_2) {
    printf("%s\n", "Swap");
    void* aux = valor_1;
    valor_1 = valor_2;
    valor_2 = aux;
}*/

void swap(void **p, void **q)
{
    void *t = *p;
    *p = *q;
    *q = t;
}

bool heap_redimensionar(heap_t* heap, const double factor) {

    size_t capacidad_nueva = (size_t)((double)heap->capacidad * factor);

	heap->arr = realloc(heap->arr, capacidad_nueva * sizeof(void*));

	if (!heap->arr) {
		heap_destruir(heap, NULL);
		return false;
	}

	heap->capacidad = capacidad_nueva;

	return true;

}

void upheap(heap_t* heap, size_t index_hijo) {

    size_t index_padre = get_index_padre(index_hijo);

    // Caso en el padre
    if (index_padre == -1) return;
    if (heap->cmp(heap->arr[index_padre], heap->arr[index_hijo]) >= 0) return;

    swap(&heap->arr[index_padre], &heap->arr[index_hijo]);

    upheap(heap, index_padre);

    return;

}

void downheap(heap_t* heap, size_t index_padre) {

    size_t i_hijo_der = get_index_der(index_padre);
    size_t i_hijo_izq = get_index_izq(index_padre);

    // Me quedo con el menor de los hijos
    size_t index_hijo = i_hijo_der > i_hijo_izq ? i_hijo_izq : i_hijo_der;

    // Caso en el padre
    if (index_hijo == -1) return;
    if (heap->cmp(heap->arr[index_padre], heap->arr[index_hijo]) < 0) return;

    swap(&heap->arr[index_padre], &heap->arr[index_hijo]);

    downheap(heap, index_padre);

    return;

}

heap_t *heap_crear(cmp_func_t cmp) {

    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->arr = malloc(CAPACIDAD_INICIAL * sizeof(void*));
    heap->tam = 0;
    heap->capacidad = CAPACIDAD_INICIAL;
    heap->cmp = cmp;

    return heap;

}

void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    return heap->arr[0];
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->tam;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->tam == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {

    if (!elem) return false;

    if (heap->tam == heap->capacidad) {
        if (!heap_redimensionar(heap, FACTOR_AGRANDAR)) return false;
    }

    heap->arr[heap->tam] = elem;
    heap->tam++;

    upheap(heap, heap->tam - 1);

    return true;
}

void *heap_desencolar(heap_t *heap) {

    if (heap_esta_vacio(heap)) return NULL;

    if (heap->tam <= heap->capacidad / 4 && heap->tam > CAPACIDAD_INICIAL) {
        if (!heap_redimensionar(heap, FACTOR_ACHICAR)) return false;
    }
    // Swapeo el primero con el ultimo
    swap(&heap->arr[0], &heap->arr[heap->tam]);

    void* elemento_quitado = heap->arr[heap->tam];

    heap->tam--;

    downheap(heap, 0);

    return elemento_quitado;

}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {

    for (size_t i = 0; i < heap->tam && destruir_elemento; i++) {
        destruir_elemento(heap->arr[i]);
    }

    free(heap->arr);
    free(heap);

}
