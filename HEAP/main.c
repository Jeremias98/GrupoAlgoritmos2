#include "testing.h"
#include <stdlib.h>
#include <stdio.h>

void pruebas_heap_alumno(void);
void pruebas_heap_catedra(void);
void pruebas_heap_volumen_catedra(size_t, bool);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main(int argc, char *argv[])
{
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    pruebas_heap_alumno();

    return failure_count() > 0;
}
