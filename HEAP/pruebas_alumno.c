#include "testing.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
bool _es_heap(void* arr[], size_t n, size_t idx_act, cmp_func_t cmp);
void heapify(void* elementos[], size_t cant, cmp_func_t cmp);
bool es_heap(void* elementos[], size_t cant, cmp_func_t cmp);

int comparar_numeros(const void* str1, const void* str2) {

	if ( *((int*)str1) > *((int*)str2) ) return 1;
	else if ( *((int*)str1) < *((int*)str2) ) return -1;
	else return 0;

}

void test_encolar() {
	printf("\n####### TEST ENCOLAR #######\n");

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

void test_desencolar() {

	printf("\n####### TEST DESENCOLAR #######\n");
	heap_t* heap = heap_crear(comparar_numeros);

	int arreglo[6] = {9,5,12,11,10,2};

	bool encolo_ok = true;
	for (int i = 0; i < 6 && encolo_ok; i++) {
		encolo_ok = heap_encolar(heap, &arreglo[i]);
	}
	print_test("Encolar 6 elementos (9,5,12,11,10,2)", encolo_ok);
	print_test("Cantidad es 6", heap_cantidad(heap) == 6);
	print_test("Ver max es 12", heap_ver_max(heap) == &arreglo[2]);

	print_test("Desencolar es 12", heap_desencolar(heap) == &arreglo[2]);
	print_test("Cantidad es 5", heap_cantidad(heap) == 5);
	print_test("Ver max es 11", heap_ver_max(heap) == &arreglo[3]);

	print_test("Desencolar es 11", heap_desencolar(heap) == &arreglo[3]);
	print_test("Cantidad es 4", heap_cantidad(heap) == 4);
	print_test("Ver max es 10", heap_ver_max(heap) == &arreglo[4]);

	print_test("Desencolar es 10", heap_desencolar(heap) == &arreglo[4]);
	print_test("Cantidad es 3", heap_cantidad(heap) == 3);
	//printf("Max %d\n", *((int*)(heap_ver_max(heap))));
	print_test("Ver max es 9", heap_ver_max(heap) == &arreglo[0]);

	print_test("Desencolar es 9", heap_desencolar(heap) == &arreglo[0]);
	print_test("Cantidad es 2", heap_cantidad(heap) == 2);
	print_test("Ver max es 5", heap_ver_max(heap) == &arreglo[1]);

	print_test("Desencolar es 5", heap_desencolar(heap) == &arreglo[1]);
	print_test("Cantidad es 1", heap_cantidad(heap) == 1);
	print_test("Ver max es 2", heap_ver_max(heap) == &arreglo[5]);

	print_test("Desencolar es 2", heap_desencolar(heap) == &arreglo[5]);
	print_test("Cantidad es 0", heap_cantidad(heap) == 0);
	print_test("Ver max es NULL", heap_ver_max(heap) == NULL);

	print_test("Desencolar es NULL", heap_desencolar(heap) == NULL);

	heap_destruir(heap, NULL);
}

void test_crear_arreglo() {

	printf("\n####### TEST CREAR ARREGLO #######\n");
	size_t n = 6;
	int num[] = {5, 9, 17, 4, 11, 8};
	int** arreglo = malloc(n*sizeof(int*));
	for (int i = 0; i < n; i++) {
		arreglo[i] = malloc(sizeof(int));
		*arreglo[i] = num[i];
	}

	heap_t* heap = heap_crear_arr((void**)arreglo, n, comparar_numeros);
	printf("Heap crear arreglo con elementos 5,9,17,4,11,8\n");
	
	print_test("Ver max es 17", heap_ver_max(heap) == arreglo[2]);
	print_test("Cantidad es 6", heap_cantidad(heap) == 6);

	print_test("Desencolar es 17", heap_desencolar(heap) == arreglo[2]);
	print_test("Cantidad es 5", heap_cantidad(heap) == 5);
	print_test("Ver max es 11", heap_ver_max(heap) == arreglo[4]);

	print_test("Desencolar es 11", heap_desencolar(heap) == arreglo[4]);
	print_test("Cantidad es 4", heap_cantidad(heap) == 4);
	print_test("Ver max es 9", heap_ver_max(heap) == arreglo[1]);

	print_test("Desencolar es 9", heap_desencolar(heap) == arreglo[1]);
	print_test("Cantidad es 3", heap_cantidad(heap) == 3);
	print_test("Ver max es 8", heap_ver_max(heap) == arreglo[5]);	

	print_test("Desencolar es 8", heap_desencolar(heap) == arreglo[5]);
	print_test("Cantidad es 2", heap_cantidad(heap) == 2);
	print_test("Ver max es 5", heap_ver_max(heap) == arreglo[0]);

	print_test("Desencolar es 5", heap_desencolar(heap) == arreglo[0]);
	print_test("Cantidad es 1", heap_cantidad(heap) == 1);
	print_test("Ver max es 4", heap_ver_max(heap) == arreglo[3]);

	print_test("Desencolar es 4", heap_desencolar(heap) == arreglo[3]);
	print_test("Cantidad es 0", heap_cantidad(heap) == 0);
	print_test("Ver max es NULL", heap_ver_max(heap) == NULL);		
	print_test("Desencolar es NULL", heap_desencolar(heap) == NULL);

	heap_destruir(heap, NULL);
	for (size_t i = 0; i < n; i++) free(arreglo[i]);
	free(arreglo);
}

void test_heapify() {

	printf("\n####### TEST HEAPIFY #######\n");
	
	size_t n = 10;
	int num[] = {9, 6, 10, 2, 13, 8, 15, 5, 26, 20};
	int** arreglo = malloc(n*sizeof(int*));
	for (int i = 0; i < n; i++) {
		arreglo[i] = malloc(sizeof(int));
		*arreglo[i] = num[i];
	}

	printf("Creo el siguiente arreglo\n");
	for (size_t i = 0; i < n; i++) printf("%d - ", *(int*)arreglo[i]);
	printf("\n");
	print_test("Arreglo no es heap", !es_heap((void**)arreglo, n, comparar_numeros));

	heapify((void**)arreglo, n, comparar_numeros);
	printf("Resultado heapify\n");
	for (size_t i = 0; i < n; i++) printf("%d - ", *(int*)arreglo[i]);
	printf("\n");
	print_test("Arreglo es heap", es_heap((void**)arreglo, n, comparar_numeros));

	for (size_t i = 0; i < n; i++) free(arreglo[i]);
	free(arreglo);	
}

void test_heapsort(){

	printf("\n####### TEST HEAPSORT #######\n");
	
	size_t n = 14;
	int num[] = {3, 34, 12 , 6, 8, 1, 25, 7, 9, 14, 13, 31, 5, 8};
	int** arreglo = malloc(n*sizeof(int*));
	for (int i = 0; i < n; i++) {
		arreglo[i] = malloc(sizeof(int));
		*arreglo[i] = num[i];
	}
	printf("Aplico HEAPSORT al siguiente arreglo\n");
	for (size_t i = 0; i < n; i++) printf("%d - ", *(int*)arreglo[i]);
	printf("\n");

	heap_sort((void**) arreglo, n, comparar_numeros);
	printf("Resultado:\n");
	for (size_t i = 0; i < n; i++) printf("%d - ", *(int*)arreglo[i]);
	printf("\n\n");

	for (size_t i = 0; i < n; i++) free(arreglo[i]);
	free(arreglo);	
}
void pruebas_heap_alumno(void) {
	test_encolar();
	test_desencolar();
	test_crear_arreglo();
	test_heapify();
	test_heapsort();
}
