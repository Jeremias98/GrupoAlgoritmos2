#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_INICIAL 10
#define FACTOR_AGRANDAR 2
#define FACTOR_ACHICAR 0.5

struct heap {
    size_t tam;
    size_t capacidad;
    void** arr;
    cmp_func_t cmp;
};

size_t get_index_padre(size_t posicion) {
    return posicion == 0 ? -1 : (posicion - 1) / 2;
}

size_t get_index_der(size_t posicion) {
    return 2 * posicion + 2;
}

size_t get_index_izq(size_t posicion) {
    return 2 * posicion + 1;
}

void swap(void **valor_1, void **valor_2)
{
    void *valor_aux = *valor_1;
    *valor_1 = *valor_2;
    *valor_2 = valor_aux;
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

void upheap(void* arr[], cmp_func_t cmp, size_t index_hijo) {

    size_t index_padre = get_index_padre(index_hijo);

    // Caso en el padre
    if (index_padre == -1) return;
    if (cmp(arr[index_padre], arr[index_hijo]) >= 0) return;

    swap(&arr[index_padre], &arr[index_hijo]);

    upheap(arr, cmp, index_padre);

    return;

}

void downheap(void* arr[], size_t tam, cmp_func_t cmp, size_t index_padre) {

    size_t i_hijo_der = get_index_der(index_padre);
    size_t i_hijo_izq = get_index_izq(index_padre);

    // Caso sin hijos
    if (i_hijo_der >= tam && i_hijo_izq >= tam) return;

    size_t index_hijo = -1;

    if (i_hijo_der < tam && i_hijo_izq < tam)
        index_hijo = cmp(arr[i_hijo_der], arr[i_hijo_izq]) < 0 ? i_hijo_izq : i_hijo_der;
    else
        index_hijo = i_hijo_izq < tam ? i_hijo_izq : i_hijo_der;

    // Caso que el hijo mayor no sea mayor que el padre
    if (cmp(arr[index_padre], arr[index_hijo]) > 0) return;

    swap(&arr[index_padre], &arr[index_hijo]);

    downheap(arr, tam, cmp, index_hijo);

    return;

}
bool _es_heap(void* arr[], size_t n, size_t idx_act, cmp_func_t cmp) {

    if (idx_act >= n) return true;
    size_t i_hijo_der = get_index_der(idx_act);
    size_t i_hijo_izq = get_index_izq(idx_act);

    if (i_hijo_izq < n && cmp(arr[i_hijo_izq], arr[idx_act]) > 0) return false;
    if (i_hijo_der < n && cmp(arr[i_hijo_der], arr[idx_act]) > 0) return false;

    return _es_heap(arr, n, i_hijo_izq, cmp) && _es_heap(arr, n, i_hijo_der, cmp); 
}

bool es_heap(void* elementos[], size_t cant, cmp_func_t cmp) {
    return _es_heap(elementos, cant, 0, cmp);
}

void heapify(void* elementos[], size_t cant, cmp_func_t cmp) {

    if(!elementos || cant<= 0 || !cmp) return;

    for(int i = (int)cant-1; i >= 0; i --) downheap(elementos, cant, cmp, i);
}

heap_t *heap_crear(cmp_func_t cmp) {

    if (!cmp) return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->arr = malloc(CAPACIDAD_INICIAL * sizeof(void*));
    heap->tam = 0;
    heap->capacidad = CAPACIDAD_INICIAL;
    heap->cmp = cmp;

    return heap;

}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {

    if (!arreglo || n <= 0 || !cmp) return NULL;

    heap_t* heap = heap_crear(cmp);
    if (!heap) return NULL;

    bool encolo_ok = true;
    for (size_t i = 0; i < n && encolo_ok; i++) {
        encolo_ok = heap_encolar(heap, arreglo[i]);
    }

    if (!encolo_ok) return NULL;

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

    upheap(heap->arr, heap->cmp, heap->tam - 1);

    return true;
}

void *heap_desencolar(heap_t *heap) {

    if (heap_esta_vacio(heap)) return NULL;

    if (heap->tam <= heap->capacidad / 4 && heap->tam > CAPACIDAD_INICIAL) {
        if (!heap_redimensionar(heap, FACTOR_ACHICAR)) return false;
    }

    void* elemento_quitado = heap->arr[0];

    // Swapeo el primero con el ultimo
    swap(&heap->arr[0], &heap->arr[heap->tam - 1]);

    heap->tam--;

    downheap(heap->arr, heap->tam, heap->cmp, 0);

    return elemento_quitado;

}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {

    for (size_t i = 0; i < heap->tam && destruir_elemento; i++) {
        destruir_elemento(heap->arr[i]);
    }

    free(heap->arr);
    free(heap);

}

void _heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
  
    if (cant == 0) return;

    swap(&elementos[0], &elementos[cant-1]);
    downheap(elementos, cant-1, cmp, 0);
    _heap_sort(elementos, cant-1, cmp);
}    

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
 
    heapify(elementos, cant, cmp);
    _heap_sort(elementos, cant, cmp);
}
