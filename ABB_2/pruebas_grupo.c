#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparar_numeros(const char* str1, const char* str2) {
	
	if (atoi(str1) > atoi(str2)) return 1;
	else if (atoi(str1) < atoi(str2)) return -1;
	else return 0;
	
}

bool imprimir(const char* clave, void* dato, void* extra) {
	printf("%s\n", clave);
	return true;
}

bool corte_simple(const char *clave, void *dato, void *extra) {
	if (comparar_numeros(clave, "5") == 0) return false;
	else  {
		printf("%s\n", clave);
		return true;
	}
}

bool solo_derecha(const char *clave, void *dato, void *extra) {
	if (comparar_numeros(clave, "10") < 0) return false;
	else  {
		printf("%s\n", clave);
		return true;
	}
}

bool solo_izquierda(const char *clave, void *dato, void *extra) {
	if (comparar_numeros(clave, "10") > 0) return false;
	else  {
		printf("%s\n", clave);
		return true;
	}
}


void test_crear_1() {
	
	printf("\n");
	printf("INICIO PRUEBAS ARBOL 6 ELEMENTOS\n\n");

	abb_t* arbol = abb_crear(comparar_numeros, NULL);
	
	char* clave_1 = "14";
	char* dato_1 = "Perro";
	
	char* clave_2 = "9";
	char* dato_2 = "Gato";

	char* clave_3 = "5";
	char* dato_3 = "Paloma";
	
	char* clave_4 = "21";
	char* dato_4 = "Raton";
	
	char* clave_5 = "15";
	char* dato_5 = "Conejo";	

	char* clave_6 = "11";
	char* dato_6 = "Cuca";
	
	print_test("Obtener es NULL", abb_obtener(arbol, clave_1) == NULL);
	
	print_test("Guardar elemento 1", abb_guardar(arbol, clave_1, dato_1));
	print_test("Obtener elemento 1", abb_obtener(arbol, clave_1) == dato_1);
	print_test("Cantidad es 1", abb_cantidad(arbol) == 1);
	
	print_test("Guardar elemento 2", abb_guardar(arbol, clave_2, dato_2));
	print_test("Obtener elemento 2", abb_obtener(arbol, clave_2) == dato_2);
	print_test("Cantidad es 2", abb_cantidad(arbol) == 2);

	print_test("Guardar elemento 3", abb_guardar(arbol, clave_3, dato_3));
	print_test("Guardar elemento 4", abb_guardar(arbol, clave_4, dato_4));
	print_test("Guardar elemento 5", abb_guardar(arbol, clave_5, dato_5));
	print_test("Guardar elemento 6", abb_guardar(arbol, clave_6, dato_6));	
	print_test("Obtener elemento 3", abb_obtener(arbol, clave_3) == dato_3);
	print_test("Obtener elemento 4", abb_obtener(arbol, clave_4) == dato_4);
	print_test("Obtener elemento 5", abb_obtener(arbol, clave_5) == dato_5);
	print_test("Obtener elemento 6", abb_obtener(arbol, clave_6) == dato_6);				
	print_test("Pertenece elemento 1", abb_pertenece(arbol, clave_1) == true);
	print_test("Pertenece elemento 2", abb_pertenece(arbol, clave_2) == true);
	print_test("Pertenece elemento 3", abb_pertenece(arbol, clave_3) == true);
	print_test("Pertenece elemento 4", abb_pertenece(arbol, clave_4) == true);
	print_test("Pertenece elemento 5", abb_pertenece(arbol, clave_5) == true);			
	print_test("Pertenece elemento 6", abb_pertenece(arbol, clave_6) == true);
	
	print_test("Cantidad es 6", abb_cantidad(arbol) == 6);
	printf("Inorder: \n");
	abb_in_order(arbol, imprimir, NULL);
	print_test("Borrar elemento 1 - '14'(raiz)", abb_borrar(arbol, clave_1) == dato_1);
	printf("Inorder: \n");
	abb_in_order(arbol, imprimir, NULL);
	print_test("Obtener elemento 2", abb_obtener(arbol, clave_2) == dato_2);	
	print_test("Obtener elemento 3", abb_obtener(arbol, clave_3) == dato_3);
	print_test("Obtener elemento 4", abb_obtener(arbol, clave_4) == dato_4);
	print_test("Obtener elemento 5", abb_obtener(arbol, clave_5) == dato_5);
	print_test("Obtener elemento 6", abb_obtener(arbol, clave_6) == dato_6);		
	print_test("Borrar elemento 2 - '9'", abb_borrar(arbol, clave_2) == dato_2);	
	print_test("Borrar elemento 3 - '5'", abb_borrar(arbol, clave_3) == dato_3);
	printf("Inorder: \n");
	abb_in_order(arbol, imprimir, NULL);
	print_test("Obtener elemento 4", abb_obtener(arbol, clave_4) == dato_4);
	print_test("Obtener elemento 5", abb_obtener(arbol, clave_5) == dato_5);
	print_test("Obtener elemento 6", abb_obtener(arbol, clave_6) == dato_6);		
	print_test("Borrar elemento 4 - '21'", abb_borrar(arbol, clave_4) == dato_4);//PROBAR EN DIFERENTE ORDEN
	print_test("Borrar elemento 5 - '15'", abb_borrar(arbol, clave_5) == dato_5);
	print_test("Borrar elemento 6 - '11'", abb_borrar(arbol, clave_6) == dato_6);
	print_test("Cantidad es 0", abb_cantidad(arbol) == 0);
	print_test("No pertenece elemento 1", abb_pertenece(arbol, clave_1) == false);
	print_test("No pertenece elemento 2", abb_pertenece(arbol, clave_2) == false);
	print_test("No pertenece elemento 3", abb_pertenece(arbol, clave_3) == false);			
	print_test("No pertenece elemento 4", abb_pertenece(arbol, clave_4) == false);
	print_test("No pertenece elemento 5", abb_pertenece(arbol, clave_5) == false);
	print_test("No pertenece elemento 6", abb_pertenece(arbol, clave_6) == false);		

	abb_destruir(arbol);
	
}

void test_crear_2() {
	
	printf("\n");
	printf("INICIO PRUEBAS ARBOL 3 ELEMENTOS\n\n");

	abb_t* arbol = abb_crear(comparar_numeros, NULL);
	
	char* clave_1 = "14";
	char* dato_1 = "Perro";
	
	char* clave_2 = "9";
	char* dato_2 = "Gato";
	
	char* clave_3 = "1";
	char* dato_3 = "Jirafa";
	
	print_test("Obtener es NULL", abb_obtener(arbol, clave_1) == NULL);
	print_test("Borrar es NULL", abb_borrar(arbol, clave_1) == NULL);
	
	print_test("Guardar elemento 1", abb_guardar(arbol, clave_1, dato_1));
	print_test("Obtener elemento 1", abb_obtener(arbol, clave_1) == dato_1);
	print_test("Cantidad es 1", abb_cantidad(arbol) == 1);
	
	print_test("Guardar elemento 2", abb_guardar(arbol, clave_2, dato_2));
	print_test("Obtener elemento 2", abb_obtener(arbol, clave_2) == dato_2);
	print_test("Cantidad es 2", abb_cantidad(arbol) == 2);
	
	print_test("Guardar elemento 3", abb_guardar(arbol, clave_3, dato_3));
	print_test("Obtener elemento 3", abb_obtener(arbol, clave_3) == dato_3);
	print_test("Cantidad es 3", abb_cantidad(arbol) == 3);
	
	print_test("Pertenece 1", abb_pertenece(arbol, clave_1));
	print_test("Pertenece 2", abb_pertenece(arbol, clave_2));
	print_test("Pertenece 3", abb_pertenece(arbol, clave_3));
	
	printf("Inorder: \n");
	abb_in_order(arbol, imprimir, NULL);	
	
	print_test("Borrar 1 (raiz)", abb_borrar(arbol, clave_1) == dato_1);
	print_test("Cantidad es 2", abb_cantidad(arbol) == 2);
	
	printf("Inorder: \n");
	abb_in_order(arbol, imprimir, NULL);	
	
	print_test("Obtener elemento 2", abb_obtener(arbol, clave_2) == dato_2);
	
	print_test("Borrar 2", abb_borrar(arbol, clave_2) == dato_2);
	print_test("Cantidad es 1", abb_cantidad(arbol) == 1);
	
	printf("Inorder: \n");
	abb_in_order(arbol, imprimir, NULL);
	
	print_test("Borrar 3", abb_borrar(arbol, clave_3) == dato_3);
	print_test("Cantidad es 0", abb_cantidad(arbol) == 0);
	
	
	abb_destruir(arbol);
	
}

void test_iter_interno() {
	
	printf("\n");
	printf("INICIO PRUEBAS ARBOL ITER INTERNO\n");
	printf("Agrego 7 elementos - Uso funcion imprimir\n\n");

	char* claves[] = {"10", "5", "20", "8", "15", "2", "22", NULL};
	char* valores[] = {"1", "2", "3", "4", "5", "6", "7", NULL};
	
	abb_t* arbol = abb_crear(comparar_numeros, NULL);
	
	for (int i = 0; i < 7; i++) {
		abb_guardar(arbol, claves[i], valores[i]);
	}
	
	printf("Imprimo todos\n");
	abb_in_order(arbol, imprimir, NULL);
	
	printf("Imprimo con corte\n");
	abb_in_order(arbol, corte_simple, NULL);
	
	printf("Imprimo solo la rama der\n");
	abb_in_order(arbol, solo_derecha, NULL);
	
	printf("Imprimo solo la rama izq\n");
	abb_in_order(arbol, solo_izquierda, NULL);
	
	abb_destruir(arbol);
	
}

void test_iter_externo() {
	
	printf("\n");
	printf("INICIO PRUEBAS ARBOL ITER EXTERNO\n");
	printf("Agrego 7 elementos - Itero hasta el final\n\n");

	char* claves[] = {"10", "5", "20", "8", "15", "2", "22", NULL};
	char* valores[] = {"1", "2", "3", "4", "5", "6", "7", NULL};
	
	abb_t* arbol = abb_crear(comparar_numeros, NULL);
	
	for (int i = 0; i < 7; i++) {
		abb_guardar(arbol, claves[i], valores[i]);
	}
	
	abb_in_order(arbol, NULL, NULL);
	
	abb_iter_t* iter = abb_iter_in_crear(arbol);
	
	while (!abb_iter_in_al_final(iter)) {
		
		printf("%s\n", abb_iter_in_ver_actual(iter));
		
		abb_iter_in_avanzar(iter);
	}
	
	abb_iter_in_destruir(iter);
	
	abb_destruir(arbol);
}

void test_nulls() {
	
	printf("\nINICIO PRUEBAS ARBOL NULLS\n");
	
	abb_t* arbol = abb_crear(comparar_numeros, NULL);
	
	char* clave_1 = "";
	char* dato_1 = NULL;
	
	print_test("Cantidad es 0", abb_cantidad(arbol) == 0);
	
	print_test("Guardar elemento clave vacia y dato NULL en raiz (tiene que poder)", abb_guardar(arbol, clave_1, dato_1));
	
	print_test("Cantidad es 1", abb_cantidad(arbol) == 1);
	
	print_test("Borrar clave vacia", abb_borrar(arbol, clave_1) == NULL);
	
	print_test("Cantidad es 0", abb_cantidad(arbol) == 0);
	
	abb_destruir(arbol);
	
}


void pruebas_abb_alumno() {
	test_crear_1();
	test_crear_2();
	test_iter_interno();
	test_iter_externo();
	test_nulls();
}
