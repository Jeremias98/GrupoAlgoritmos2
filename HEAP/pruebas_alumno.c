#include "testing.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int comparar_numeros(const void* str1, const void* str2) {

	if ( *((int*)str1) > *((int*)str2) ) return 1;
	else if ( *((int*)str1) < *((int*)str2) ) return -1;
	else return 0;

}

void test_insertar() {
	printf("\n####### TEST INSERTAR #######\n");

	heap_t* heap = heap_crear(comparar_numeros);

	int valor_1 = 5;
	heap_encolar(heap, &valor_1);

	heap_destruir(heap, NULL);
}

void pruebas_heap_alumno(void) {
	test_insertar();
}
