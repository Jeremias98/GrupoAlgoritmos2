#define _POSIX_C_SOURCE 200809L

#include "lista.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>

#define LARGO_INI 20 //EJEMPLO

typedef struct hash_campo {
    char *clave;
    void *valor;
} hash_campo_t;

struct hash {
    lista_t **tabla;
    size_t cantidad;
    size_t largo;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter {
    hash_campo_t* actual;
    lista_iter_t* lista_iter;
    size_t idx_tabla;
    const hash_t* hash;
};


/********************* FUNCIONES AUXILIARES *****************************/

hash_campo_t* hash_campo_crear(char* clave, void* dato) {
    
    hash_campo_t* hash_campo = malloc(sizeof(hash_campo_t));
    if (!hash_campo) return NULL;

    hash_campo->clave = clave; 
    hash_campo->valor = dato;
    return hash_campo;
}

size_t funcion_hash (const char* str, size_t modulo) {
    if(str[0] == '\0') return 0;
  
    size_t res = (size_t) str[0] % modulo;
    for (int i = 1; str[i] != '\0'; i++) {
    res = ((res * 256) + (size_t) str[i]) % modulo;
    }
    return res;
}

lista_iter_t* crear_y_posicionar_iterador(const hash_t* hash, const char* clave) { 
    
    size_t idx_tabla = funcion_hash(clave, hash->largo);                // SI HAY MATCH CON LA CLAVE ES EL ACTUAL DEL ITERADOR
    lista_iter_t* l_iter = lista_iter_crear(hash->tabla[idx_tabla]);    // SI NO EXISTE LA CLAVE EL ITERADOR ITERA HASTA EL FINAL Y APUNTA A NULL.

    hash_campo_t* campo_act = lista_iter_ver_actual(l_iter);             
    while (campo_act && (!strcmp(campo_act->clave, clave) == 0)) {      // FUNCION UTIL PARA PRIMITIVAS GUARDAR, BORRAR, OBTENER, PERTENECE
        lista_iter_avanzar(l_iter);                                     
        campo_act = lista_iter_ver_actual(l_iter);
    }
    return l_iter;    
}

/************************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
    
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    hash->tabla = malloc(sizeof(lista_t*) * LARGO_INI);
    if (!hash->tabla) {
        free(hash);
        return NULL;
    }
    hash->cantidad = 0;
    hash->largo = LARGO_INI;
    hash->destruir_dato = destruir_dato;

    for (size_t i = 0; i < hash->largo; i++) {
        hash->tabla[i] = lista_crear();
    }
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {

    lista_iter_t* l_iter = crear_y_posicionar_iterador(hash, clave);
    hash_campo_t* campo_act = lista_iter_ver_actual(l_iter);
    char* clave_cpy = strdup(clave);
    if (!clave_cpy) return false;

    if(!campo_act) {
        if(!lista_iter_insertar(l_iter, hash_campo_crear(clave_cpy, dato))) {
            free(clave_cpy);
            return false;
        }
    }
    else {   
        campo_act->clave = clave_cpy;
        campo_act->valor = dato;
    }
    lista_iter_destruir(l_iter);
    hash->cantidad ++;
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    
    lista_iter_t* l_iter = crear_y_posicionar_iterador(hash, clave);
    hash_campo_t* campo_borrado = lista_iter_borrar(l_iter);
    lista_iter_destruir(l_iter);

    if(!campo_borrado) return NULL;

    else {
        void* dato_borrado = campo_borrado->valor;
        free(campo_borrado->clave); 
        free(campo_borrado);
        hash->cantidad --;
        return dato_borrado;
    }   
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    
    lista_iter_t* l_iter = crear_y_posicionar_iterador(hash, clave);
    hash_campo_t* campo_act = lista_iter_ver_actual(l_iter);
    lista_iter_destruir(l_iter);
    if (!campo_act) return NULL;
    else return campo_act->valor; 
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    
    lista_iter_t* l_iter = crear_y_posicionar_iterador(hash, clave);
    bool cond = (lista_iter_ver_actual(l_iter));
    lista_iter_destruir(l_iter);
    return cond;    
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
    hash_destruir_dato_t destruir_dato = hash->destruir_dato;
    
    for (size_t i = 0; i < hash->largo; i++) {
        hash_campo_t* campo_borrado = lista_borrar_primero(hash->tabla[i]);
        while(campo_borrado) {
            destruir_dato(campo_borrado);
            campo_borrado = lista_borrar_primero(hash->tabla[i]);
        }
        lista_destruir(hash->tabla[i], NULL);
    }
    free(hash->tabla);
    free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;

    iter->hash = hash;
    hash_campo_t* primer_campo = NULL;
    lista_iter_t* lista_iter = NULL;
    
    for (size_t i = 0; i < iter->hash->largo; i++) {
        lista_t* lista = iter->hash->tabla[i];
        primer_campo = lista_ver_primero(lista);
        if (primer_campo) {
            lista_iter = lista_iter_crear(lista);
            iter->idx_tabla = i;
            break;
        }
    }
    iter->actual = primer_campo;
    iter->lista_iter = lista_iter;
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){ 
    if (!iter->actual) return false;

    lista_iter_avanzar(iter->lista_iter);
    hash_campo_t* campo_act = lista_iter_ver_actual(iter->lista_iter);
    
    if (!campo_act) {
        lista_iter_destruir(iter->lista_iter);
        iter->lista_iter = NULL;
        
        for (size_t i = iter->idx_tabla + 1; i < iter->hash->largo; i++) {
            lista_t* lista = iter->hash->tabla[i];
            campo_act = lista_ver_primero(lista);
            if (campo_act) {
                iter->lista_iter = lista_iter_crear(lista);
                iter->idx_tabla = i;
                break;   
            }
        }
    } 
    iter->actual = campo_act;
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(!iter->actual) return NULL;
    return iter->actual->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    if (!iter->actual) return false;
    return true;
}

void hash_iter_destruir(hash_iter_t* iter) {
    lista_iter_destruir(iter->lista_iter);
    free(iter);
}