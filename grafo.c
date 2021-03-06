/*  Trabalho de implementação 1 - Grafos 2019/1
 *  Autores: Arthur Pedri Trevisol - apt14 GRR20141784
 *           Sayuri Morikane Ribeiro - smr17 GRR20175881
 *  Professor: Renato Carmo
 *
 */
#define _GNU_SOURCE

#include "grafo.h"
#include <stdlib.h>
#include <string.h>

struct grafo{
  char *nome;
  int tamanho;
  char _COMPILER_PADDING[4]; // padding para alinhamento na memória
  struct vertice *vertices; //lista de vertices do grafo
};

typedef struct aresta{
    struct aresta *prox;
    int visitado;
    char _COMPILER_PADDING[4];
    vertice vizinho;
}aresta;

struct vertice{
  char *nome;
  struct aresta *lista;
  struct vertice *prox;
};

vertice adc_vetice(char* nome, grafo g);
int agrupamento(vertice v, int *triades_totais);
aresta *busca_aresta(vertice v, char *nome);

/*  vertice adc_vetice(char* nome, grafo g);
 *  Adiciona um vértice na lista de vértices do grafo.
 *  Percorre checando se o vértice já existe.
 *  Valor de retorno: endereço do vértice existente ou inserido.
 *
 */
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
    return(novo_vertice);
}

/*  aresta *busca_aresta(vertice v, char *nome);
 *  Busca na lista de arestas de um vértice um vizinho pelo nome.
 *
 *  Valor de retorno: Endereço da aresta, se existe um vizinho com o nome passado por parametro.
 *                    NULL, se não existe vizinho com o nome passado por parametro.
 *
 */
aresta *busca_aresta(vertice v, char *nome){
    aresta *a = v->lista;
    while (a != NULL) {
        if (strcmp(nome, a->vizinho->nome) == 0 ) {
            return(a);
        }
        a = a->prox;
    }
    return(NULL);
}

/*  grafo le_grafo(FILE *input);
 *  Lê o grafo de um arquivo FILE *.
 *  um grafo é um arquivo onde cada linha tem zero, uma ou duas
 *  strings (sequência de caracteres sem "whitespace").
 *  Valor de retorno: Endereço do grafo gerado pelo arquivo input.
 *
 */
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

  // lê palavras
  while(fscanf(input, "%s%c", token, &separador) > 0){
    /* se está separado por whitespace é aresta
    *  senão não é vertice
    */
    vertice v1 = adc_vetice(token, novo_grafo);
    if(separador == ' '){
      fscanf(input, "%s%c", token, &separador);
      vertice v2 = adc_vetice(token, novo_grafo);
      if((busca_aresta(v1, token)) == NULL){
          aresta *aux = malloc(sizeof(struct aresta));
          aux->prox = v1->lista;
          aux->vizinho = v2;
          aux->visitado = 0;
          v1->lista = aux;
          aux = malloc(sizeof(struct aresta));
          aux->prox = v2->lista;
          v2->lista = aux;
          aux->visitado = 0;
          aux->vizinho = v1;
      }
    }
  }
  return(novo_grafo);

}

/*  int destroi_grafo(grafo g);
 *  Desaloca toda memória usada pela estrutura do grafo (vertices, arestas, nomes)
 *  Valor de retorno: 0, se o grafo foi desalocado
 *                    1, se o grafo é nulo
 *
 */
int destroi_grafo(grafo g){
    if (g == NULL) {
        return(1);
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

/*  grafo escreve_grafo(FILE *output, grafo g);
 *  Escreve o grafo g no mesmo formato de entrada no FILE * passado por parâmetro.
 *  Valor de retorno: Nulo, se o grafo for nulo
 *                    Endereço do grafo que foi escrito, se o grafo não for nulo.
 *
 */
grafo escreve_grafo(FILE *output, grafo g){
    if (g == NULL) {
        return(NULL);
    }

    // fprintf(output,"graph %s:\n", g->nome);
    vertice v = g->vertices;
    vertice vprox = NULL;
    while (v != NULL) {
        vprox = v->prox;

        aresta *a = v->lista;
        aresta *aprox = NULL;
        if (a == NULL){ // Se o vertice não tem arestas
          fprintf(output, "%s\n",v->nome);
        }

        //print arestas
        while (a != NULL) {
          aprox = a->prox;
          aresta *aresta_duplicada = busca_aresta(a->vizinho, v->nome);
          if ((a->visitado == 0)&&(aresta_duplicada->visitado == 0)) {
            a->visitado = 1;
            fprintf(output, "%s",v->nome);
            fprintf(output, " %s\n", a->vizinho->nome );
          }
          else if((a->visitado == 1)&&(aresta_duplicada->visitado == 1)){
            a->visitado = 0;
            aresta_duplicada->visitado =0;
          }
            a = aprox;
        };
        v = vprox;
    }
    return(g);
}

/*  int agrupamento(vertice v, int *triades_totais);
 *  Conta o coeficiente de agrupamento de um vertice para todos os seus vizinhos.
 *  Valor de retorno: Quantidade de tríades fechadas contendo o vértice passado.
 *
 */
int agrupamento(vertice v, int *triades_totais){
  int fechadas = 0;
  if (v->lista == NULL) {
    return(0);
  }

  aresta *lista_v = v->lista;

  while (lista_v != NULL){
    vertice v1 = lista_v->vizinho;

    aresta *vizinhos = lista_v->prox;

    while(vizinhos != NULL){
      vertice v2 = vizinhos->vizinho;

      *triades_totais+=1;

      if(busca_aresta(v2, v1->nome) != NULL){
        fechadas++;
      }
      vizinhos = vizinhos->prox;

    }

    lista_v = lista_v->prox;
  }

  return(fechadas);
}

/*  double coeficiente_agrupamento_grafo(grafo g);
 *  Calcula o coeficiente de agrupamento do grafo.
 *  Valor de retorno: Coeficiente de Agrupamento do grafo = triades_fechadas/triades_totais.
 *
 */
double coeficiente_agrupamento_grafo(grafo g){
    int triades = 0;
    int t_fechadas = 0;
    double coeficiente = 0;
    for(vertice v = g->vertices; v != NULL; v = v->prox){
      t_fechadas += agrupamento(v, &triades);
    }

    if (triades)
      coeficiente = (double) t_fechadas / (double) triades;

    return(coeficiente);
}
