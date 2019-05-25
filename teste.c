#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {
    FILE *entrada = stdin;
    grafo g = le_grafo(entrada);
    printf("vertices %d\n", n_vertices(g));
    printf("arestas %d\n", n_arestas(g) );
    escreve_grafo(stdout, g);
    printf("novo grafo \n" );
    escreve_grafo(stdout, g);
    destroi_grafo(g);
    return 0;
}
