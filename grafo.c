#include "grafo.h"
#include <stdlib.h>
#include <string.h>

struct grafo{
  char *nome;
  vertice *vertices; //head da lista
};

struct vertice{
  char *nome;
  struct vertice *vizinho;
  struct vertice *prox_vizinho;
};


grafo le_grafo(FILE *input){
  size_t t_linha;
  char *linha = NULL;

  if (input == NULL) {
    return(NULL);
  }

  while(getline(&linha, &t_linha, input) != -1){
    printf("%s ", linha);
    //strtok(linha, &);
  }
  return(NULL);

}
