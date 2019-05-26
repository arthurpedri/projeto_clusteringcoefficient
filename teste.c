#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {
    FILE *entrada = stdin;
    grafo g = le_grafo(entrada);
    vertice **cobertura;
    printf("vertices %d\n", n_vertices(g));
    printf("arestas %d\n", n_arestas(g) );
    // escreve_grafo(stdout, g);
    unsigned int a =  cobertura_por_trilhas( g, &cobertura);
    printf("trilhas %u\n", a);
    escreve_grafo(stdout, g);

    destroi_grafo(g);
    return 0;
}
