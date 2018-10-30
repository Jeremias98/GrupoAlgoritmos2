#include "testing.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void test_insertar() {
	printf("\n####### TEST INSERTAR #######\n");

	heap_t* heap = heap_crear(NULL);

	heap_destruir(heap, NULL);
}

void pruebas_heap_alumno(void) {
	test_insertar();
}
