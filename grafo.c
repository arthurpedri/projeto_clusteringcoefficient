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
}listav;

struct _vertice{
  char *nome;
  struct aresta *lista;
  struct _vertice *prox;
  int grau;
  char _COMPILER_PADDING[4];
};

vertice adc_vertice(char* nome, grafo g);
int agrupamento(vertice v, int *triades_totais);
aresta *busca_aresta(vertice v, char *nome);
unsigned int vertices_impares(grafo g);
aresta *remove_aresta(vertice v1, aresta *v2);
void adc_na_trilha(listav *lista, vertice v);
listav *encontra_ciclo(vertice r);
listav *naLista(listav *u, listav *v);
/*  vertice adc_vertice(char* nome, grafo g);
 *  Adiciona um vértice na lista de vértices do grafo.
 *  Percorre checando se o vértice já existe.
 *  Valor de retorno: endereço do vértice existente ou inserido.
 *
 */
vertice adc_vertice(char* nome, grafo g){
    size_t nome_size = strlen(nome);
    if (nome == NULL || nome_size == 0) {
        return NULL;
    }
    vertice novo_vertice = vertice_nome(g, nome);
    if(novo_vertice == NULL){
        novo_vertice = (vertice) malloc(sizeof(struct _vertice));
        novo_vertice->nome = (char *) malloc(sizeof(char)*(nome_size+1));
        strncpy(novo_vertice->nome, nome, nome_size+1);
        novo_vertice->prox = g->vertices;
        g->vertices = novo_vertice;
        g->tamanho++;
        novo_vertice->grau = 0;
        novo_vertice->lista = NULL;
    }
    return(novo_vertice);
}

// devolve ponteiro para aresta-prox
aresta *remove_aresta(vertice v1, aresta *v2){
  if (v2 == NULL) {
    return(NULL);
  }
  if (v2->vizinho == v1) {
    aresta *aux = v2->prox;
    free(v2);
    return(aux);
  }
  else{
    v2->prox = remove_aresta(v1, v2->prox);
    return(v2);
  }
}

/*
 * adciona vertice na trilha
 */
void adc_na_trilha(listav *lista, vertice v){
  if (!v) {
      return;
  }
  //aloca vertice
  listav *novo;

  novo = malloc(sizeof(struct listav));
  novo->vertice = v;
  novo->prox = NULL;


  if (lista->vertice) {
    while (lista->prox != NULL) {
      lista = lista->prox;
    }
    lista->prox = novo;
  }
  else{
      lista = novo;
  }
  return;
}
/*
 * Retorna o nome do vertice v
 */
char *nome(vertice v){
  return(v->nome);
}

/*
 *  devolve o número de vértices de g
 */
unsigned int n_vertices(grafo g){
  return((unsigned int)g->tamanho);
}

/*
 * devolve o número de arestas de g
 */
unsigned int n_arestas(grafo g){
  int soma = 0;
  for (vertice i = g->vertices; i != NULL ; i = i->prox) {
    soma = soma + i->grau;
  }
  return((unsigned int)(soma/2));
}

/*
 * Retorna o ponteiro para o vértice com esse nome no grafo g
 * Se o vértice nao existir retorna NULL
 */
vertice vertice_nome(grafo g, char *nome){
  vertice novo_vertice = NULL;
  for (vertice i = g->vertices; i != NULL; i = i->prox) {
      if (strcmp(nome, i->nome) == 0) {
          novo_vertice = i;
      }
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
  novo_grafo =(grafo) malloc(sizeof(struct _grafo));
  novo_grafo->vertices = NULL;
  novo_grafo->nome= NULL;
  novo_grafo->tamanho = 0;

  // lê palavras
  while(fscanf(input, "%s%c", token, &separador) > 0){
    /* se está separado por whitespace é aresta
    *  senão não é vertice
    */
    vertice v1 = adc_vertice(token, novo_grafo);
    if(separador == ' '){
      fscanf(input, "%s%c", token, &separador);
      vertice v2 = adc_vertice(token, novo_grafo);
      if((busca_aresta(v1, token)) == NULL){
          aresta *aux = malloc(sizeof(struct aresta));
          aux->prox = v1->lista;
          aux->vizinho = v2;
          aux->visitado = 0;
          v1->lista = aux;
          v1->grau++;
          aux = malloc(sizeof(struct aresta));
          aux->prox = v2->lista;
          v2->lista = aux;
          v2->grau++;
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
          else if((a->visitado == 1)||(aresta_duplicada->visitado == 1)){
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

// Verifica se listas tem vertice em comum

listav *naLista(listav *u, listav *v){
    if(!u) return NULL;
    if(!v) return NULL;
    for (listav *i = v ; i != NULL; i = i->prox) {
        for (listav *j = u ; j != NULL; j = j->prox) {
            if (strcmp(j->vertice->nome, i->vertice->nome) == 0) {
                return j;
            }
        }
    }
    return NULL;
}

/*
 * Conta quantidade de vértices com grau ímpar
 *
 */
unsigned int vertices_impares(grafo g){
  unsigned int impares = 0;
  for (vertice i = g->vertices ; i !=NULL ; i= i->prox) {
    if ((i->grau % 2)!= 0 ) {
      impares++;
    }
  }

  return(impares/2);
}

listav *encontra_ciclo(vertice r){
  listav *ciclo = malloc(sizeof(struct listav));
  ciclo->vertice = r;
  ciclo->prox = NULL;
  vertice proximo = NULL;
  // adc_na_trilha(ciclo, r);
  vertice pai = r;
while (pai->lista != NULL) {
    proximo = pai->lista->vizinho; // primeiro vizinho do pai
    // acha prox vertice por ordem alfabética
    for (aresta *i = pai->lista->prox; i != NULL ; i = i->prox ) { // trocado r por pai
      int compara = strcmp(proximo->nome, i->vizinho->nome);// se >0 proximo > i->vizinho
      if ((compara > 0) ) {
        proximo = i->vizinho;
      }
    }
    pai->lista= remove_aresta(proximo, pai->lista);
    --pai->grau;
    proximo->lista = remove_aresta(pai, proximo->lista);
    --proximo->grau;

    adc_na_trilha(ciclo, proximo);
    pai = proximo;
  }
  //printf("%d", ciclo);
  return(ciclo);
}

/*
 *devolve o número mínimo k de trilhas necessárias para cobrir o grafo g
 *
 *observe queda forma que o Professor pediu
 *  k = 1, se g é euleriano, ou
 *  k = (número de vértices de grau ímpar em g)/2, caso contrário
 *
 *aloca e preenche o vetor cobertura de maneira que cobertura[i] contém uma
 *trilha em G para cada 0 <= i < k.
 *
 *cada trilha é um sequência de vertices terminada por NULL, isto é, para cada 0 <= i < k
 *a sequência
 *
 *    (cobertura[i][0], cobertura[i][1], ..., cobertura[i][l-1], cobertura[i][l])
 *
 *é tal que
 *
 *    1. cobertura[i][j] é um vertice, para cada 0 <= j < l,
 *    2. cobertura[i][l] == NULL, e
 *    3. cobertura[i][j-1] é vizinho de cobertura[i][j] em g, para cada 0 < j < l
 */
unsigned int cobertura_por_trilhas(grafo g, vertice **cobertura[]){
  unsigned int k = vertices_impares(g);
  listav **aux = malloc(sizeof(struct listav)*k*3);
  //listav aux[g->tamanho];
  listav *cicloEncontrado;
  int trilhasExtras = 0;
  listav *trilhaPrincipal;
  char stringZero[2] = "0";
  if (k == 0) {
    k = 1;
    vertice primeiro = g->vertices;
    for (vertice vert = primeiro->prox; vert != NULL; vert = vert->prox) {
      int compara = strcmp(primeiro->nome, vert->nome);// se >0 proximo > i->vizinho
      if ((compara > 0) ) {
        primeiro = vert;
      }
    }
    trilhaPrincipal = encontra_ciclo(primeiro);
  }
  else{ // adc o vértice e arestas com todos os vértices de grau ímpar
    vertice v1 = adc_vertice(stringZero,g);
    //adiciona arestas ligando vertice novo nos vertices de grau ímpar
    for (vertice i = g->vertices ; i != NULL; i = i->prox) {
        if ((i->grau % 2)!= 0 ) {  // Se for ímpar adc aresta
          aresta *nova_aresta = malloc(sizeof(struct aresta));
          nova_aresta->prox = v1->lista;
          nova_aresta->vizinho = i;
          nova_aresta->visitado = 0;
          v1->lista = nova_aresta;
          v1->grau++;
          nova_aresta = malloc(sizeof(struct aresta));
          nova_aresta->prox = i->lista;
          i->lista = nova_aresta;
          i->grau++;
          nova_aresta->visitado = 0;
          nova_aresta->vizinho = v1;
        }
    }
    trilhaPrincipal = encontra_ciclo(v1);

}

  for (listav *i = trilhaPrincipal ; i != NULL; i = i->prox) {
    printf("[nome %s]", i->vertice->nome);
  }
  printf("\n" );

  for (vertice i = g->vertices ; i != NULL; i = i->prox) {
      if (i->grau > 0) {  // Se vertice ainda tiver arestas
          cicloEncontrado = encontra_ciclo(i);
          aux[trilhasExtras] = naLista(cicloEncontrado, trilhaPrincipal); // retorna vertice do encontra_ciclo em comum com trilhaPrincipal
          // printf("aux %s\n", aux[trilhasExtras]->vertice->nome);
          // for (listav *p = cicloEncontrado ; p != NULL; p = p->prox) {
          //   printf("[nome %s]", p->vertice->nome);
          // }
          // printf("\n" );
          if(aux[trilhasExtras]){
              listav *comum_principal = trilhaPrincipal; // vertice trilhaPrincipal em comum_principal com trilha
              while (strcmp(comum_principal->vertice->nome, aux[trilhasExtras]->vertice->nome) != 0) { // nao deve dar loop infinito, mas cuidado
                  comum_principal = comum_principal->prox;
              }
              // printf("vertice %s, %s / auxprox %s, jprox %s\n", comum_principal->vertice->nome, aux[trilhasExtras]->vertice->nome,aux[trilhasExtras]->prox->vertice->nome, comum_principal->prox->vertice->nome);

              listav *continuacao = comum_principal->prox; // e
              listav *comeco = comum_principal;//d na trilha principal
              comum_principal = aux[trilhasExtras]->prox; // i na trilha extra

              listav *cicloA = aux[trilhasExtras];//d na trilha exta
              listav *proximo = aux[trilhasExtras]->prox;
              //inverte a fila até o final
              while(proximo != NULL){
                  proximo = cicloA->prox;
                  cicloA->prox = continuacao;
                  continuacao = cicloA;
                  cicloA = proximo;
                }
                // volta no começo da fila e vai até o vertice em comum deles

              for(cicloA = cicloEncontrado->prox; (aux[trilhasExtras] != cicloA); cicloA = proximo){
                  proximo = cicloA->prox;
                  cicloA->prox= continuacao;
                  continuacao =cicloA;
              }
              comeco->prox = continuacao;


          } else {

              aux[trilhasExtras] = cicloEncontrado; // se nao tem vertice em comum com trilhaPrincipal guarda nas trilhas extras e incrementa o contador
              trilhasExtras++;
          }
      }

    }
    // int falta = trilhasExtras;
//     while(falta){
//       for(int i = 0 ; i <= trilhasExtras ; i++){
//         listav *ciclo_restante = naLista(aux[i],trilhaPrincipal);
//         if (ciclo_restante) {
//           listav *comum_principal = trilhaPrincipal; // vertice trilhaPrincipal em comum_principal com trilha
//           while (strcmp(comum_principal->vertice->nome, ciclo_restante->vertice->nome) != 0) { // nao deve dar loop infinito, mas cuidado
//               comum_principal = comum_principal->prox;
//           }
//           listav *continuacao = comum_principal->prox; // e
//           listav *comeco = comum_principal;//d na trilha principal
//
//           listav *cicloA = ciclo_restante;//d na trilha exta
//           listav *proximo = ciclo_restante->prox;
//           //inverte a fila até o final
//           while(proximo != NULL){
//               proximo = cicloA->prox;
//               cicloA->prox = continuacao;
//               continuacao = cicloA;
//               cicloA = proximo;
//             }
//             // volta no começo da fila e vai até o vertice em comum deles
//           for(cicloA = aux[i]->prox; (ciclo_restante != cicloA); cicloA = proximo){
//               proximo = cicloA->prox;
//               cicloA->prox= continuacao;
//               continuacao =cicloA;
//           }
//           comeco->prox = continuacao;
//           falta--;
//         }
//       }
//    }


  cobertura = malloc(sizeof(vertice**)*k);
  //  **cobertura = malloc(sizeof(vertice*)*k);

   printf("\n" );
   for (unsigned int linha = 0 ; linha < k; ++linha) {
     if (strcmp(trilhaPrincipal->vertice->nome, stringZero )== 0 ){
       trilhaPrincipal = trilhaPrincipal->prox;
     }
     unsigned int coluna = 0;
     listav *salvo= trilhaPrincipal;
     while(trilhaPrincipal != NULL && (strcmp(trilhaPrincipal->vertice->nome, stringZero )!= 0 )){
       coluna++;
       trilhaPrincipal = trilhaPrincipal->prox;
     }
     (cobertura[linha]) = (vertice**)malloc(sizeof(vertice*)*(coluna+1));
     for (unsigned int op = 0; op < coluna+1 ; op++) {
       cobertura[linha][op] = (vertice*)malloc(sizeof(vertice));
       *(cobertura[linha][op]) = salvo->vertice;
       printf("cobertura %d %d %s\n",linha,op,((*cobertura[linha][op])->nome));
       salvo= salvo->prox;
     }
      cobertura[linha][coluna+1] = (vertice*)malloc(sizeof(vertice));
     *(cobertura[linha][coluna+1]) = NULL;
     }

  for (listav *i = trilhaPrincipal ; i != NULL; i = i->prox) {
    printf("[nome %s]", i->vertice->nome);
  }
  printf("\n" );

  // Passar trilhas para cobertura[] ?!?!
  // for (listav *cicloEuclidiano = trilhaPrincipal) {
  //   /* code */
  // }

  return(k);
}
