#define _POSIX_C_SOURCE 200809L

#include "strutil.h"
#include <string.h>
#include <stdlib.h>

char** split(const char* str, char sep) {
    
    size_t contador_sep = 0;
    size_t str_max = 0;
    size_t str_actual = 1;
    for(int i = 0; str[i] != '\0'; i ++) {
        str_actual ++;
        if (str[i] == sep) {
            if (str_actual > str_max) str_max = str_actual;
            contador_sep ++; 
            str_actual = 1;
        }
    }
    if (str_actual > str_max) str_max = str_actual;

    char **strv = malloc(sizeof(char*) * (contador_sep + 2));
    if(!strv) return NULL;

    size_t idx_strv = 0;
    size_t idx_porc = 0;
    char porcion[str_max];
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == sep) {
            porcion[idx_porc] = '\0';
            strv[idx_strv] = strdup(porcion);
            idx_strv ++;
            idx_porc = 0;
        }
        else {
            porcion[idx_porc] = str[i];
            idx_porc ++;
        }
    }
    porcion[idx_porc] = '\0';
    strv[idx_strv] = strdup(porcion);
    idx_strv ++;   
    strv[idx_strv] = NULL;
    return strv;
}

char* join(char** strv, char sep) {
    
    size_t idx_strv = 0;
    size_t idx_res = 0;
    while(strv[idx_strv]) {
        for(int i = 0; strv[idx_strv][i] != '\0'; i++) {
            idx_res++;
        }
        idx_res++;
        idx_strv++;
    }
    if (strv[0]) idx_res--;    
    
    char *resultado = malloc(sizeof(char) * (idx_res + 1));
    if(!resultado) return NULL;

    idx_strv = 0;
    idx_res = 0;
    while(strv[idx_strv]) {
        for(int i = 0; strv[idx_strv][i] != '\0'; i++) {
            resultado[idx_res] = strv[idx_strv][i];
            idx_res++;
        }
        resultado[idx_res] = sep;
        idx_res++;
        idx_strv++;
    }
    if (strv[0]) idx_res--;
    resultado[idx_res] = '\0';
     
    return resultado;
}

void free_strv(char** strv) {
    size_t idx_strv = 0;
    while (strv[idx_strv]) {
        free(strv[idx_strv]);
        idx_strv ++;
    }
    free(strv);
}