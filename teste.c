#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {
    FILE *entrada = stdin;
    grafo g = le_grafo(entrada);
    escreve_grafo(stdout, g);
    printf("Coeficiente: %f\n" ,coeficiente_agrupamento_grafo(g));
    destroi_grafo(g);
    return 0;
}
