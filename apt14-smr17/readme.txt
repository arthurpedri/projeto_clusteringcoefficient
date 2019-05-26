Trabalho de implementação 2 - Grafos 2019/1
 Autores: Arthur Pedri Trevisol - apt14 GRR20141784
              Sayuri Morikane Ribeiro - smr17 GRR20175881
 Professor: Renato Carmo

Estruturas:

struct _grafo{
 char *nome;
 int tamanho;
 char _COMPILER_PADDING[4]; // padding para alinhamento na memória
 struct _vertice *vertices; //lista de vertices do grafo
};

typedef struct aresta{
   struct aresta *prox;
   int visitado;
   char _COMPILER_PADDING[4];
   vertice vizinho;
}aresta;

typedef struct listav{
   struct listav *prox;
   vertice vertice;
   struct listav *prox_list;
}listav;

Usamos essa estrutura de listas para fazer uma lista de vértices sem interferir
nas suas relações internas, e assim também podemos manter uma lista de trilhas
diferentes para melhor organizar as procuras de vértices em comum.

struct _vertice{
 char *nome;
 struct aresta *lista;
 struct _vertice *prox;
 int grau;
 char _COMPILER_PADDING[4];
};

O programa funciona usando a função encontra_ciclo(), que procura por um ciclo
euleriano usando ordem alfabética para encontrar um ciclo euleriano no grafo e 
remover as arestas que já estão em uma trilha. Se o grafo não for euleriano ele
usa o vértice “falso” 0, que é vizinho de todos os vértices de grau ímpar. No
retorno dessa função, nós verificamos se ainda existem vértices com grau > 0,
e se existem chamamos encontra_ciclo() para esses vértices. Se as trilhas
encontradas a partir desses vértices tem algum vértice em comum com a trilha
principal, nós fazemos a incorporação delas na trilha principal. Se existirem
ciclos que não têm vértice em comum com a primeira chamada da trilha principal
nós iteramos sobre uma lista de trilhas extras até que o número de trilhas
extras = 0, isso faz com que os segmentos sejam maximais. Após incorporado
todas as trilhas, um separa as trilhas maximais usando o vértice falso como
delimitador. Feito isso alocamos na matriz cobertura os vértices. E o valor de
retorno da função cobertura_por_trilhas() é a quantidade mínima de trilhas
para se cobrir o grafo (k).
