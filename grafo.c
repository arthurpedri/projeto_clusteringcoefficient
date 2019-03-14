#define _GNU_SOURCE

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
  char *token = NULL;
  int nos = 0;
  if (input == NULL) {
    return(NULL);
  }

//lê linhas pegando os token=palavras
  while(getline(&linha,(size_t *)&t_linha, input) != -1){
    token = strtok(linha, " \t\n");
    while(token != NULL){
      nos++;
      printf("%s", token);
      token = strtok(NULL, " \t\n");
    }
    if (nos > 1){
      printf("aresta\n");
      nos = 0;
    }
    else if(nos == 1){
      printf("nó\n");
      nos = 0;
    }
  }

  free(linha);
  free(token);
  return(NULL);

}
