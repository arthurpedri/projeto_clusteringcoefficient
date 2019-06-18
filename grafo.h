#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo tem um nome, que é uma "string"

typedef struct _grafo *grafo;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um vértice
// 
// o vértice tem um nome, que é uma "string"

typedef struct _vertice *vertice;

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// lê um grafo de input.
// um grafo é um arquivo onde cada linha tem zero, uma ou duas
// strings (sequência de caracteres sem "whitespace").
// 
// devolve o grafo lido. Caso o arquivo esteja mal formado o
// comportamento da função é indefinido

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// escreve o grafo g em output, no mesmo formato que o usado por le_grafo()
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve o número de vértices de g

unsigned int n_vertices(grafo g);

//------------------------------------------------------------------------------
// devolve o número de arestas de g

unsigned int n_arestas(grafo g);

//..............................................................................
// as funções primeiro_vertice() e proximo_vertice() a seguir
// implementam um iterador para os vertices de g de maneira que, por
// exemplo, o laço
//
//    for (v=primeiro_vertice(g); v; v=proximo_vertice(v,g)) {
//        processa(v);
//        }
//
// executa processa(v) uma vez para cada vertice v de g
//..............................................................................

//------------------------------------------------------------------------------
// devolve o primeiro vertice de g numa iteração por todos os vertices de g 
//      ou
//         NULL, se g não tem vertices
 
vertice primeiro_vertice(grafo g);

//------------------------------------------------------------------------------
// devolve o vertice seguinte a v em g numa iteração por todos os vertices de g 
//      ou
//         NULL, se v é o último vertice da iteração

vertice proximo_vertice(vertice v, grafo g);

//------------------------------------------------------------------------------
// computa um emparelhamento de tamanho máximo no grafo bipartido g e devolve o
// tamanho (número de arestas) do emparelhamento computado

unsigned int emparelha(grafo g);

//------------------------------------------------------------------------------
// devolve 
//         NULL se v é descoberto pelo emparelhamento M,
//      ou
//         o par de v no emparelhamento M, 
//         isto é, 
//         a outra ponta da aresta de M que cobre v, caso contrário,
//
// onde M é
//         o emparelhamento computado pela última execução de emparelha(g),
//       ou
//         o emparelhamento vazio, se emparelha(g) não foi executado

vertice par(vertice v);
//------------------------------------------------------------------------------
#endif
