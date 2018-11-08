#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tp2.h"
#include "strutil.h"
#include "heap.h"

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

bool agregar_archivo(const char* ruta, abb_t* abb, hash_t* hash) {

    FILE* archiv = fopen(ruta, "r");
    if(!archiv) return false;

    char* linea = NULL; 
    size_t capacidad = 0;
       
    while((getline(&linea, &capacidad, archiv)) > 0) { 
        
        linea[strlen(linea) - 1] = '\0';
        char** strv_datos_vuelo = split(linea, ',');
        
        if (hash_pertenece(hash, strv_datos_vuelo[0])) {
            vuelo_crear(strv_datos_vuelo, (vuelo_t*) hash_obtener(hash, strv_datos_vuelo[0]));
        }
        else {
            vuelo_t* vuelo = vuelo_crear(strv_datos_vuelo, NULL);
            if (!abb_guardar(abb, vuelo->date, vuelo) || !hash_guardar(hash, vuelo->flight_n, vuelo)) return false;
        }
        free(strv_datos_vuelo);   
    }
    free(linea);
    fclose(archiv);
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

int main(int argc, char* argr[]) {

    if (argc != 1) {
        fprintf(stderr, "%s\n", "ERROR");
        return 0;
    }

    // ESTRUCUTURAS DONDE SE VAN A GUARDAR LOS STRUCT VUELO
    // ABB PARA ORDENAR LA TABLA POR DATE Y EL HASH PARA OBTENER LOS DATOS
    // DEL VUELO EN O(1).

    abb_t* abb = abb_crear(strcmp, NULL);
    hash_t* hash = hash_crear((hash_destruir_dato_t) vuelo_destruir);

    char* linea = NULL; 
    size_t capacidad = 0;
    char** strv_comands = strv_comandos();
    
    while((getline(&linea, &capacidad, stdin)) > 0) { 

        linea[strlen(linea) - 1] = '\0';
        char** strv_linea = split(linea, ' ');
        
        //CASO EL COMANDO ES AGREGAR ARCHIVO
        if(strcmp(strv_linea[0], strv_comands[0]) == 0) {
            
            if (!strv_linea[1] || !agregar_archivo(strv_linea[1], abb, hash)) {
                fprintf(stderr, "%s\n", "Error en comando agregar_archivo");
            }
            else fprintf(stdout, "%s\n", "OK");
        }

        //CASO COMANDO ES VER TABLERO
        else if(strcmp(strv_linea[0], strv_comands[1]) == 0) {
            continue;
        }

        //CASO COMANDO INFO VUELO
        else if(strcmp(strv_linea[0], strv_comands[2]) == 0) {
            
            if (!strv_linea[1] || !imprimir_info_vuelo(strv_linea[1], hash)) {
                fprintf(stderr, "%s\n", "Error en comando info_vuelo");
            }
            else fprintf(stdout, "%s\n", "OK");
        }
        
        //CASO PRIORIDAD VUELOS
        else if(strcmp(strv_linea[0], strv_comands[3]) == 0) {
            continue;
        } 

        //CASO COMANDO ES BORRAR
        else if(strcmp(strv_linea[0], strv_comands[4]) == 0) {
            continue;
        }

        //CASO NO ES COMANDO VÁLIDO
        else{
            fprintf(stderr, "%s\n", "Error comando inválido");
            continue;
        }
        free_strv(strv_linea);
    }
    free(linea);
    free_strv(strv_comands);
    abb_destruir(abb);
    hash_destruir(hash);     
    return 0;
}