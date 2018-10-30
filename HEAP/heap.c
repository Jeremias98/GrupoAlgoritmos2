#include "heap.h"
#include <stdlib.h>

#define CAPACIDAD_INICIAL 10
#define FACTOR_AGRANDAR 2
#define FACTOR_ACHICAR 0.5

typedef struct heap {
    size_t tam;
    size_t capacidad;
    void** arr;
    cmp_func_t cmp;
} heap_t;

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

heap_t *heap_crear(cmp_func_t cmp) {

    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->arr = malloc(CAPACIDAD_INICIAL * sizeof(void*));
    heap->tam = 0;
    heap->capacidad = CAPACIDAD_INICIAL;
    heap->cmp = cmp;

    return heap;

}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {

    size_t index = 0;

    void* dato = heap->tam > 0 ? heap->arr[index] : NULL;

    while(dato) {

        if (destruir_elemento) {
            destruir_elemento(dato);
        }

        index++;

        dato = heap->arr[index];

    }

    free(heap->arr);
    free(heap);

}
