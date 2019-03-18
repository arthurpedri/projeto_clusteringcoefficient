Trabalho de implementação 1 - Grafos 2019/1
 Autores: Arthur Pedri Trevisol - apt14 GRR20141784
              Sayuri Morikane Ribeiro - smr17 GRR20175881
 Professor: Renato Carmo

Estruturas:

struct grafo{
 char *nome;
 int tamanho;
 char _COMPILER_PADDING[4]; // padding para alinhamento na memória
 struct vertice *vertices; // head da lista de vertices do grafo
};

typedef struct aresta{
   struct aresta *prox;
   int visitado;
   char _COMPILER_PADDING[4];
   vertice vizinho;
}aresta;

struct vertice{
 char *nome;
 struct aresta *lista; // head da lista de arestas
 struct vertice *prox;
};

Nós implementamos o grafo com uma lista de vértices, onde o vértice
que está na estrutura do grafo é o head da lista, e adicionamos novos
vértices usando esse head. Cada vértice tem um apontador para o próximo
vértice da lista e para a head da sua própria lista de arestas.
As arestas tem o apontador para a próxima aresta da lista de arestas do
vértice e o seu vizinho.

Coeficiente:

Para calcular o coeficiente usamos uma função que calcula as tríades
abertas e fechadas de cada vértice. Ela funciona da seguinte forma:

Iteramos sobre a lista de arestas do vértice principal, e em cada
iteração nós fixamos um v1 que é o vizinho da primeira aresta do vértice
principal, e iteramos sobre os próximos vizinhos do vértice principal.
Se existem vizinhos já podemos contar uma tríade, e em cada um desses
vizinhos nós buscamos uma aresta deles com o v1 fixado, caso essa aresta
exista contamos mais uma tríade fechada.
