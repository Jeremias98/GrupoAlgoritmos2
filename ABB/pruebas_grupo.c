#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_crear() {
	
	abb_t* arbol = abb_crear(strcmp, NULL);
	
	char* clave_1 = "14";
	char* dato_1 = "Perro";
	
	char* clave_2 = "9";
	char* dato_2 = "Gato";
	
	print_test("Obtener es NULL", abb_obtener(arbol, clave_1) == NULL);
	
	print_test("Guardar elemento 1", abb_guardar(arbol, clave_1, dato_1));
	print_test("Obtener elemento 1", abb_obtener(arbol, clave_1) == dato_1);
	print_test("Cantidad es 1", abb_cantidad(arbol) == 1);
	
	print_test("Guardar elemento 2", abb_guardar(arbol, clave_2, dato_2));
	print_test("Obtener elemento 2", abb_obtener(arbol, clave_2) == dato_2);
	
	abb_destruir(arbol);
	
}


void pruebas_abb_alumno() {
	test_crear();
}
