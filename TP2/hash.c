#define _POSIX_C_SOURCE 200809L

#include "lista.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>

#define LARGO_INI 50          
#define FACTOR_REDIMENSION 4     
#define MULTIP_REDIMENSION 3     

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
    
    size_t idx_tabla = funcion_hash(clave, hash->largo);              // SI HAY MATCH CON LA CLAVE ES EL ACTUAL DEL ITERADOR
    lista_iter_t* l_iter = lista_iter_crear(hash->tabla[idx_tabla]);  // SI NO EXISTE LA CLAVE EL ITERADOR ITERA HASTA EL FINAL Y APUNTA A NULL.

    hash_campo_t* campo_act = lista_iter_ver_actual(l_iter);             
    while (campo_act && (!strcmp(campo_act->clave, clave) == 0)) {    // FUNCION UTIL PARA PRIMITIVAS GUARDAR, BORRAR, OBTENER, PERTENECE
        lista_iter_avanzar(l_iter);                                     
        campo_act = lista_iter_ver_actual(l_iter);
    }
    return l_iter;    
}

// Devuelve el siguiente campo de la tabla de hash
hash_campo_t* obtener_siguiente_campo(hash_iter_t *iter) {
	
	hash_campo_t* campo = NULL;
	
	for (size_t i = iter->idx_tabla + 1; i < iter->hash->largo && !campo; i++) {
		lista_t* lista = iter->hash->tabla[i];
		campo = lista_ver_primero(lista);
		if (campo) {
			iter->lista_iter = lista_iter_crear(lista);
			iter->idx_tabla = i;   
		}
	}
	
	return campo;
}

// Funcion para limpiar la tabla. Si reubucar = true, los datos se guardaran en hash_nuevo pasado por parametro
bool limpiar_tabla(lista_t** tabla, hash_destruir_dato_t destruir_dato, size_t largo, bool reubicar, hash_t* hash_nuevo) {
	
	bool limpio_ok = true;
	
    for (size_t i = 0; i < largo; i++) {
		
        hash_campo_t* campo = lista_borrar_primero(tabla[i]);
        while(campo && limpio_ok) {
			
            if (reubicar) {
                limpio_ok = hash_guardar(hash_nuevo, campo->clave, campo->valor);
            }    
            else if(destruir_dato) destruir_dato(campo->valor);
            
            free(campo->clave);
            free(campo);
            campo = lista_borrar_primero(tabla[i]);
        }
        lista_destruir(tabla[i], NULL);
    }   
    
    free(tabla);
    
    return limpio_ok;
    
}

lista_t** crear_tabla(size_t largo) {
	lista_t** tabla = malloc(sizeof(lista_t*) * largo);
	if (!tabla) return NULL;
	for (size_t i = 0; i < largo; i++) {
        tabla[i] = lista_crear();
        if (!tabla[i]) return NULL;
    }
    return tabla;
}

bool hash_redimensionar(hash_t* hash) {
    
    size_t nuevo_largo = hash->largo * MULTIP_REDIMENSION;
    
    lista_t** tabla_ant = hash->tabla;
    
    hash->tabla = crear_tabla(nuevo_largo);
    
    size_t largo_ant = hash->largo;
    hash->largo *= MULTIP_REDIMENSION;
    hash->cantidad = 0;
    
    // Se chequea tambien que la tabla se haya creado. La limpieza se debe hacer antes 
    return limpiar_tabla(tabla_ant, hash->destruir_dato, largo_ant, true, hash) && hash->tabla;
}

/************************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
    
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    hash->tabla = crear_tabla(LARGO_INI);
    if (!hash->tabla) {
        free(hash);
        return NULL;
    }
    
    hash->cantidad = 0;
    hash->largo = LARGO_INI;
    hash->destruir_dato = destruir_dato;
    
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	
    hash_destruir_dato_t destruir_dato = hash->destruir_dato;
    
    lista_iter_t* l_iter = crear_y_posicionar_iterador(hash, clave);
    hash_campo_t* campo_act = lista_iter_ver_actual(l_iter);

    if(!campo_act) {
        char* clave_cpy = strdup(clave);
        if (!clave_cpy) return false;
        if (!lista_iter_insertar(l_iter, hash_campo_crear(clave_cpy, dato))) {
            free(clave_cpy);
            return false;
        }
        hash->cantidad ++;
        if (hash->cantidad / hash->largo == FACTOR_REDIMENSION) {
            if (!hash_redimensionar(hash)) return false;
        }
    }
    else {   
        if (destruir_dato) destruir_dato(campo_act->valor);
        campo_act->valor = dato;
    }
    lista_iter_destruir(l_iter);
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
	
    limpiar_tabla(hash->tabla, hash->destruir_dato, hash->largo, false, NULL);
    
    free(hash);
    
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;

    iter->hash = hash;
    iter->lista_iter = NULL;
    iter->idx_tabla = -1;
    iter->actual = obtener_siguiente_campo(iter);
    
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){ 
	
    if (!iter->actual) return false;

    lista_iter_avanzar(iter->lista_iter);
    hash_campo_t* campo_act = lista_iter_ver_actual(iter->lista_iter);
    
    if (!campo_act) {
        lista_iter_destruir(iter->lista_iter);
        iter->lista_iter = NULL;
        campo_act = obtener_siguiente_campo(iter);
    } 
    
    iter->actual = campo_act;
    
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	return !iter->actual ? NULL : iter->actual->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return (!iter->actual);
}

void hash_iter_destruir(hash_iter_t* iter) {
    lista_iter_destruir(iter->lista_iter);
    free(iter);
}
