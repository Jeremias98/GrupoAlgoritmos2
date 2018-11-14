#define _POSIX_C_SOURCE 200809L
#define TAM_CLAVE_ABB 30
#define TAM_CLAVE_HEAP 12

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tp2.h"
#include "strutil.h"
#include "heap.h"
#include "pila.h"

struct vuelo {
    char* flight_n;
    char* airline;
    char* origin_ap;
    char* destination_ap;
    char* tail_n;
    char* priority;
    char* date;
    char* dep_delay;
    char* air_time;
    char* cancelled;
};

vuelo_t* vuelo_crear(char* strv[], vuelo_t* vuelo_actualizar) {

    vuelo_t* vuelo;
    if (vuelo_actualizar) {
        vuelo = vuelo_actualizar;
        free_datos_vuelo(vuelo);
    }
    else {
        vuelo = malloc(sizeof(vuelo_t));
        if (!vuelo) return NULL;
    }

    vuelo->flight_n = strv[0];
    vuelo->airline = strv[1];
    vuelo->origin_ap = strv[2];
    vuelo->destination_ap = strv[3];
    vuelo->tail_n = strv[4];
    vuelo->priority = strv[5];
    vuelo->date = strv[6];
    vuelo->dep_delay = strv[7];
    vuelo->air_time = strv[8];
    vuelo->cancelled = strv[9];

    return vuelo;
}

void free_datos_vuelo(vuelo_t* vuelo) {
    free(vuelo->flight_n);
    free(vuelo->airline);
    free(vuelo->origin_ap);
    free(vuelo->destination_ap);
    free(vuelo->tail_n);
    free(vuelo->priority);
    free(vuelo->date);
    free(vuelo->dep_delay);
    free(vuelo->air_time);
    free(vuelo->cancelled);
}
void vuelo_destruir(vuelo_t* vuelo) {
    free_datos_vuelo(vuelo);
    free(vuelo);
}

char** strv_comandos() {
    char** strv = malloc(sizeof(char*) * 6);
    if (!strv) return NULL;

    strv[0] = strdup("agregar_archivo");
    strv[1] = strdup("ver_tablero");
    strv[2] = strdup("info_vuelo");
    strv[3] = strdup("prioridad_vuelos");
    strv[4] = strdup("borrar");
    strv[5] = NULL;
    return strv;   
}

int num_comando(const char* str, char** strv_operadores) {
    //1:AGR_ARCH; 2:VER TAB; 3: INFO_V; 4:PRIORI; 5:BORRAR; 0:COMANDO INCORRECTO
    for (int i = 0; i < 5; i++) {
        if (strcmp(str, strv_operadores[i]) == 0) return i+1;
    }
    return 0;
}

size_t n_parametros(char* strv[]) {
    size_t i = 0;
    while(strv[i]) i++;
    return i; 
}

bool str_es_num(const char* str) {
    
    for (int i = 0; str[i] != '\0'; i++) {
        if(!isdigit(str[i])) return false;
    }
    return true;
}

bool agregar_archivo(const char* ruta, abb_t* abb, hash_t* hash) {

    FILE* archiv = fopen(ruta, "r");
    if(!archiv) return false;

    char* linea = NULL; 
    size_t capacidad = 0;
       
    while((getline(&linea, &capacidad, archiv)) > 0) { 
        
        linea[strlen(linea) - 1] = '\0';
        char** strv_datos_vuelo = split(linea, ',');
        vuelo_t* vuelo;
        
        if (hash_pertenece(hash, strv_datos_vuelo[0])) {
            
            vuelo = hash_obtener(hash, strv_datos_vuelo[0]);
            
            //BORRO LA CLAVE DEL ABB ANTES DE ACTUALIZAR EL VUELO
            char clave_abb_borrar[TAM_CLAVE_ABB];
            strcpy(clave_abb_borrar, vuelo->date);
            strcat(clave_abb_borrar, " - ");
            strcat(clave_abb_borrar, vuelo->flight_n);
            abb_borrar(abb, clave_abb_borrar);

            vuelo_crear(strv_datos_vuelo, vuelo);
        }
        else {
            vuelo = vuelo_crear(strv_datos_vuelo, NULL);
            if(!hash_guardar(hash, vuelo->flight_n, vuelo)) return false;
        }
        //LA CLAVE ES DEL TIPO "<DATE> - <N VUELO>"
        //NO PUDE HACER UNA FUNCION PARA CREAR UNA CLAVE ABB ESTATICA
        char clave_abb[TAM_CLAVE_ABB];
        strcpy(clave_abb, vuelo->date);
        strcat(clave_abb, " - ");
        strcat(clave_abb, vuelo->flight_n);
        if (!abb_guardar(abb, clave_abb, NULL)) return false;
        
        free(strv_datos_vuelo);   
    }
    free(linea);
    fclose(archiv);
    return true;
}

void imprimir_pila_asc(pila_t* p_result, bool asc, bool liberar) {

    if (asc) {
        
        char* result = pila_desapilar(p_result);
        if (!pila_esta_vacia(p_result)) imprimir_pila_asc(p_result, true, liberar);
        if (result) fprintf(stdout, "%s\n", result);
        if (liberar) free(result);
        return;
    }
    while(!pila_esta_vacia(p_result)) {
        
        char* result = pila_desapilar(p_result);
        fprintf(stdout, "%s\n", result);
        if (liberar) free(result);
    }
    return;
}

bool ver_tablero(char* k, char* modo, char* desde, char* hasta, abb_t* abb) {

    if (!str_es_num(k) || strcmp(k, "0") == 0) return false;
    if (!strcmp(modo, "asc") == 0 && !strcmp(modo, "desc") == 0) return false;
    if (strcmp(desde, hasta) > 0) return false;

    int cont = 0;
    int k_vuelos = atoi(k);
    pila_t* p_result = pila_crear();
    abb_iter_t* iter = abb_iter_in_crear(abb);

    while(!abb_iter_in_al_final(iter)) {
        
        const char* date = abb_iter_in_ver_actual(iter);
        
        if ((strcmp(date, hasta) > 0) || cont == k_vuelos) break;
        if (strcmp(date, desde) > 0) {
            pila_apilar(p_result, (void*) date);
            cont ++;
        }
        abb_iter_in_avanzar(iter);
    }

    if (strcmp(modo, "asc") == 0) imprimir_pila_asc(p_result, true, false);
    else imprimir_pila_asc(p_result, false, false);

    pila_destruir(p_result);
    abb_iter_in_destruir(iter);
    return true;
}

bool imprimir_info_vuelo(const char* flight_n, hash_t* hash) {
    
    vuelo_t* vuelo = hash_obtener(hash, flight_n);
    if (!vuelo) return false;
    
    fprintf(stdout, "%s %s %s %s %s %s %s %s %s %s\n", vuelo->flight_n, vuelo->airline,
    vuelo->origin_ap, vuelo->destination_ap, vuelo->tail_n, vuelo->priority, vuelo->date,
    vuelo->dep_delay, vuelo->air_time, vuelo->cancelled);
    return true;
}        

int cmp_prioridad (const void* str1, const void* str2) {
    
    char** strv1 = split(str1, '-');
    char** strv2 = split(str2, '-');

    int prioridad1 = atoi(strv1[0]);
    int prioridad2 = atoi(strv2[0]);
    int res;

    if (prioridad1 < prioridad2) res =  1;
    else if (prioridad1 > prioridad2) res = -1;   
    else res = strcmp(strv1[1], strv2[1]);
    
    free_strv(strv1);
    free_strv(strv2);
    return res;
}

bool prioridad_vuelos(char* k, hash_t* hash) {

    if (!str_es_num(k) || strcmp(k, "0") == 0) return false;

    heap_t* heap = heap_crear(cmp_prioridad);
    pila_t* pila = pila_crear();
    int k_vuelos = atoi(k);

    int cont = 0;
    hash_iter_t* iter = hash_iter_crear(hash);
    
    while (!hash_iter_al_final(iter)) {

        const char* n_vuelo = hash_iter_ver_actual(iter);
        vuelo_t* vuelo = hash_obtener(hash, n_vuelo);
        
        char* clave = malloc(sizeof(char) * TAM_CLAVE_HEAP);
        if (!clave) return false;
        strcpy(clave, vuelo->priority);
        strcat(clave, " - ");
        strcat(clave, vuelo->flight_n);
        
        heap_encolar(heap, (void*) clave);
        hash_iter_avanzar(iter);
        cont ++;

        if (cont > k_vuelos) free(heap_desencolar(heap));  
    }
    while (!heap_esta_vacio(heap)) pila_apilar(pila, heap_desencolar(heap));

    imprimir_pila_asc(pila, false, true);

    hash_iter_destruir(iter);
    heap_destruir(heap, free);
    pila_destruir(pila);
    return true;
}

char* obtener_n_vuelo(const char* clave_date_abb) {

    char vuelo [6];
    int j = 0;
    for (int i = 22; clave_date_abb[i] != '\0'; i++) {
        vuelo[j] = clave_date_abb[i];
        j++;
    }
    vuelo[j] = '\0';
    return strdup(vuelo);
}  

bool borrar(char* fecha_desde, char* fecha_hasta, hash_t* hash, abb_t* abb) {
	
    pila_t* p_borrar = pila_crear();
    abb_iter_t* iter = abb_iter_in_crear(abb);

    while(!abb_iter_in_al_final(iter)) {
        
        const char* date = abb_iter_in_ver_actual(iter);
        
        if (strcmp(date, fecha_hasta) > 0) break;
        if (strcmp(date, fecha_desde) > 0) {
            
            char* n_vuelo = obtener_n_vuelo(date);
            imprimir_info_vuelo(n_vuelo, hash);
            vuelo_destruir(hash_borrar(hash, n_vuelo));
            pila_apilar(p_borrar, (void*) date);
            free(n_vuelo);
        }
        abb_iter_in_avanzar(iter);
    }
    while (!pila_esta_vacia(p_borrar)) {
        abb_borrar(abb, (const char*)pila_desapilar(p_borrar));
    }
    pila_destruir(p_borrar);
    abb_iter_in_destruir(iter);
    return true;
}

int main(int argc, char* argr[]) {

    if (argc != 1) {
        fprintf(stderr, "%s\n", "ERROR");
        return 0;
    }

    // ESTRUCUTURAS UTILES PARA LOS COMANDOS
    // ABB PARA ORDENAR LA TABLA POR DATE (CLAVE) (DATOS null) Y EL HASH (DATOS struct vuelo) 
    // PARA OBTENER LOS DATOS DEL VUELO EN O(1).

    abb_t* abb = abb_crear(strcmp, NULL);
    hash_t* hash = hash_crear((hash_destruir_dato_t) vuelo_destruir);

    char* linea = NULL; 
    size_t capacidad = 0;
    char** strv_comands = strv_comandos();
    
    while((getline(&linea, &capacidad, stdin)) > 0) { 

        bool comand_error = false;
        linea[strlen(linea) - 1] = '\0';
        char** strv_linea = split(linea, ' ');
        if (!strv_linea[0]) continue;  //SI NO SE INGRESA NINGUN COMANDO (?)

        //CASO EL COMANDO ES AGREGAR ARCHIVO
        if(strcmp(strv_linea[0], strv_comands[0]) == 0) {
            
            if (n_parametros(strv_linea) != 2 || !agregar_archivo(strv_linea[1], abb, hash)) {
                comand_error = true;
            }
        }

        //CASO COMANDO ES VER TABLERO
        else if(strcmp(strv_linea[0], strv_comands[1]) == 0) {

            if (n_parametros(strv_linea) != 5 || !ver_tablero(strv_linea[1], strv_linea[2], strv_linea[3], strv_linea[4], abb)) {
                comand_error = true;
            }
        }

        //CASO COMANDO INFO VUELO
        else if(strcmp(strv_linea[0], strv_comands[2]) == 0) {
            
            if (n_parametros(strv_linea) != 2 || !imprimir_info_vuelo(strv_linea[1], hash)) {
                comand_error = true;
            }
        }
        
        //CASO PRIORIDAD VUELOS
        else if(strcmp(strv_linea[0], strv_comands[3]) == 0) {
            
            if (n_parametros(strv_linea) != 2 || !prioridad_vuelos(strv_linea[1], hash)) {
                comand_error = true;
            }
        } 

        //CASO COMANDO ES BORRAR
        else if(strcmp(strv_linea[0], strv_comands[4]) == 0) {
			
			if (n_parametros(strv_linea) != 3 || !borrar(strv_linea[1], strv_linea[2], hash, abb)) {
                comand_error = true;
            }
        }

        //CASO NO ES COMANDO VÁLIDO
        else comand_error = true;

        if (comand_error) fprintf(stderr, "%s %s\n", "Error en comando", strv_linea[0]);
        else fprintf(stdout, "%s\n", "OK");
        free_strv(strv_linea);
    }
    free(linea);
    free_strv(strv_comands);
    abb_destruir(abb);
    hash_destruir(hash);     
    return 0;
}
