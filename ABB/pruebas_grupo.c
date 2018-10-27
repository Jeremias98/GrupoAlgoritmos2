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
	print_test("Pertenece elemento 1", abb_pertenece(arbol, clave_1) == true);
	print_test("Pertenece elemento 2", abb_pertenece(arbol, clave_2) == true);
	print_test("Guardar elemento 3", abb_guardar(arbol, clave_3, dato_3));
	print_test("Guardar elemento 4", abb_guardar(arbol, clave_4, dato_4));
	print_test("Guardar elemento 5", abb_guardar(arbol, clave_5, dato_5));
	print_test("Guardar elemento 6", abb_guardar(arbol, clave_6, dato_6));	
	print_test("Pertenece elemento 3", abb_pertenece(arbol, clave_3) == true);
	print_test("Pertenece elemento 4", abb_pertenece(arbol, clave_4) == true);
	print_test("Pertenece elemento 5", abb_pertenece(arbol, clave_5) == true);			
	print_test("Pertenece elemento 6", abb_pertenece(arbol, clave_6) == true);
	print_test("Cantidad es 6", abb_cantidad(arbol) == 6);
	print_test("Borrar elemento 1 (raiz)", abb_borrar(arbol, clave_1) == dato_1);	
	print_test("Borrar elemento 2", abb_borrar(arbol, clave_2) == dato_2);
	print_test("Borrar elemento 3", abb_borrar(arbol, clave_3) == dato_3);
	print_test("Borrar elemento 4", abb_borrar(arbol, clave_4) == dato_4);//PROBAR EN DIFERENTE ORDEN
	print_test("Borrar elemento 5", abb_borrar(arbol, clave_5) == dato_5);
	print_test("Borrar elemento 6", abb_borrar(arbol, clave_6) == dato_6);
	print_test("Cantidad es 0", abb_cantidad(arbol) == 0);
	print_test("No pertenece elemento 1", abb_pertenece(arbol, clave_1) == false);
	print_test("No pertenece elemento 2", abb_pertenece(arbol, clave_2) == false);
	print_test("No pertenece elemento 3", abb_pertenece(arbol, clave_3) == false);			
	print_test("No pertenece elemento 4", abb_pertenece(arbol, clave_4) == false);
	print_test("No pertenece elemento 5", abb_pertenece(arbol, clave_5) == false);
	print_test("No pertenece elemento 6", abb_pertenece(arbol, clave_6) == false);		

	abb_destruir(arbol);
	
}


void pruebas_abb_alumno() {
	test_crear();
}
