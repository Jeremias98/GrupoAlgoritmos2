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

	print_test("Heap vacio", heap_esta_vacio(heap));
	print_test("Cantidad es 0", heap_cantidad(heap) == 0);
	print_test("Ver max es NULL", !heap_ver_max(heap));

	int valor_1 = 19;
	print_test("Encolar 19", heap_encolar(heap, &valor_1));
	print_test("Cantidad es 1", heap_cantidad(heap) == 1);
	print_test("Ver max es 19", heap_ver_max(heap) == &valor_1);

	int valor_2 = 9;
	print_test("Encolar 9", heap_encolar(heap, &valor_2));
	print_test("Cantidad es 2", heap_cantidad(heap) == 2);
	print_test("Ver max sigue siendo 19", heap_ver_max(heap) == &valor_1);

	int valor_3 = 12;
	print_test("Encolar 12", heap_encolar(heap, &valor_3));
	print_test("Cantidad es 3", heap_cantidad(heap) == 3);
	print_test("Ver max sigue siendo 19", heap_ver_max(heap) == &valor_1);

	int valor_4 = 5;
	print_test("Encolar 2", heap_encolar(heap, &valor_4));
	print_test("Cantidad es 4", heap_cantidad(heap) == 4);
	print_test("Ver max sigue siendo 19", heap_ver_max(heap) == &valor_1);

	int valor_5 = 30;
	print_test("Encolar 30", heap_encolar(heap, &valor_5));
	print_test("Cantidad es 5", heap_cantidad(heap) == 5);
	print_test("Ver max ahora es 30", heap_ver_max(heap) == &valor_5);

	int valor_6 = 120;
	print_test("Encolar 120", heap_encolar(heap, &valor_6));
	print_test("Cantidad es 6", heap_cantidad(heap) == 6);
	print_test("Ver max es 120", heap_ver_max(heap) == &valor_6);

	heap_destruir(heap, NULL);
}

void pruebas_heap_alumno(void) {
	test_insertar();
}
