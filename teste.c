#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {
    FILE *entrada = stdin;
    grafo g = le_grafo(entrada);
    escreve_grafo(stdout, g);
    printf("vertices %d\n", n_vertices(g));
    printf("arestas %d\n", n_arestas(g) );
    destroi_grafo(g);
    return 0;
}
