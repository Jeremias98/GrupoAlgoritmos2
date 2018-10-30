#include "heap.h"
#include <stdlib.h>

typedef struct heap {
    size_t tam;
    void** arr;
    cmp_func_t cmp;
} heap_t;

heap_t *heap_crear(cmp_func_t cmp) {

    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->arr = NULL;
    heap->tam = 0;
    heap->cmp = cmp;

    return heap;

}
