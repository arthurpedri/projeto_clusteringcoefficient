#define _GNU_SOURCE

#include "grafo.h"
#include <stdlib.h>
#include <string.h>

struct grafo{
  char *nome;
  int tamanho;
  char _COMPILER_PADDING[4];
  struct vertice *vertices; //head da lista
};

typedef struct aresta{
    struct aresta *prox;
    vertice vizinho;
}aresta;

struct vertice{
  char *nome;
  struct aresta *lista;
  struct vertice *prox;
};

vertice adc_vetice(char* nome, grafo g);

int busca_aresta(vertice v, char *nome);

vertice adc_vetice(char* nome, grafo g){
    size_t nome_size = strlen(nome);
    if (nome == NULL || nome_size == 0) {
        return NULL;
    }
    vertice novo_vertice = NULL;
    g->tamanho++;
    for (vertice i = g->vertices; i != NULL; i = i->prox) {
        if (strcmp(nome, i->nome) == 0) {
            novo_vertice = i;
        }
    }
    if(novo_vertice == NULL){
        novo_vertice = (vertice) malloc(sizeof(struct vertice));
        novo_vertice->nome = (char *) malloc(sizeof(char)*(nome_size+1));
        strncpy(novo_vertice->nome, nome, nome_size+1);
        novo_vertice->prox = g->vertices;
        g->vertices = novo_vertice;
        novo_vertice->lista = NULL;
    }
    printf("%s\n",nome );
    return(novo_vertice);
}
//retorna 1 se achou, 0 se não achou
int busca_aresta(vertice v, char *nome){
    aresta *a = v->lista;
    while (a != NULL) {
        if (strcmp(nome, a->vizinho->nome) == 0 ) {
            return(1);
        }
        a = a->prox;
    }
    return(0);
}
grafo le_grafo(FILE *input){
  grafo novo_grafo = NULL;
  char token[1025];
  char separador;
  if (input == NULL) {
      return(NULL);
  }
  novo_grafo =(grafo) malloc(sizeof(struct grafo));
  novo_grafo->vertices = NULL;
  novo_grafo->nome= NULL;
  novo_grafo->tamanho = 0;

  //lê linhas pegando os token=palavras
  while(fscanf(input, "%s%c", token, &separador) > 0){
    vertice v1 = adc_vetice(token, novo_grafo);
    if(separador == ' '){
    // se tem mais de uma string na linhas adc aresta
      fscanf(input, "%s%c", token, &separador);
      vertice v2 = adc_vetice(token, novo_grafo);
      if(!(busca_aresta(v1, token))){
          aresta *aux = malloc(sizeof(struct aresta));
          aux->prox = v1->lista;
          aux->vizinho = v2;
          v1->lista = aux;
          aux = malloc(sizeof(struct aresta));
          aux->prox = v2->lista;
          v2->lista = aux;
          aux->vizinho = v1;
      }
    }
  }
  return(novo_grafo);

}
int destroi_grafo(grafo g){
    if (g == NULL) {
        return(0);
    }

    vertice v = g->vertices;
    vertice vprox = NULL;
    if (v != NULL)
        vprox = v->prox;
    //libera vertices
    while (v != NULL) {
        aresta *a = v->lista;
        aresta *aprox = NULL;
        if (a != NULL)
            aprox = a->prox;
        //libera arestas
        while (a != NULL) {
            free(a);
            a = aprox;
            if (a != NULL)
                aprox = a->prox;
        };
        free(v->nome);
        free(v);
        v = vprox;
        if (v != NULL)
            vprox = v->prox;
    }
    //libera grafo
    free(g);
    return(0);
}

grafo escreve_grafo(FILE *output, grafo g){
    if (g == NULL) {
        return(NULL);
    }

    fprintf(output,"graph %s:\n", g->nome);
    vertice v = g->vertices;
    vertice vprox = NULL;
    if (v != NULL)
        vprox = v->prox;
    //print vertices
    while (v != NULL) {
        fprintf(output, "\t%s:",v->nome);
        aresta *a = v->lista;
        aresta *aprox = NULL;
        if (a != NULL)
            aprox = a->prox;
        //print arestas
        while (a != NULL) {
            fprintf(output, " '%s'", a->vizinho->nome );
            a = aprox;
            if (a != NULL)
                aprox = a->prox;
        };
        putchar(10);
        v = vprox;
        if (v != NULL)
            vprox = v->prox;
    }
    return(g);
}

double coeficiente_agrupamento_grafo(grafo g){
// 3* nro de triangulos /nro de trios
    int t_abertas = 1;
    int t_fechadas = 0;
    






    return(0);
}
