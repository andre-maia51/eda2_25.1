// >> Questão 1
/* 
    >> Conceito de ciclo em um Grafo (não-dirigido)
    Um ciclo em um grafo é um caminho onde o primeiro e o último vértice do caminho
    são iguais. Ou seja, o caminho 1-2-3-1 é um ciclo pois iniciamos o caminho a partir
    do vértice 1, passamos por outros vértices e voltamos para o vértice que começamos.
*/

// Implementação da função que determina se há um ciclo em um grafo
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v;
    int w;
} Edge;

typedef struct node *link;
struct node {
    int v;
    link next;
};

typedef struct graph *Graph;
struct graph {
    int V;
    int E;
    link *adj;
};

Edge edge(int v, int w) {
    Edge e;
    e.v = v;
    e.w = w;
    return e;
}

link NEW(int v, link next) {
    link x = malloc(sizeof(*x));
    if(x == NULL) fprintf(stderr, "Falha na alocação de memória.\n");
    x->v = v;
    x->next = next;
    return x;
}

Graph GraphInit(int V) {
    int v;
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->adj = malloc(V * sizeof(link));

    for(v = 0; v < V; v++) {
        G->adj[v] = NULL;
    }

    return G;
}

void GraphInsert(Graph G, Edge e) {
    int v = e.v, w = e.w;
    G->adj[v] = NEW(w, G->adj[v]);
    G->adj[w] = NEW(v, G->adj[w]);
    G->E++; 
}

#define MAX 1000
static int cnt, pre[MAX];
int GRAPHsearch(Graph G) {
    cnt = 0;
    for(int i = 0; i < G->V; i++) pre[i] = -1;

    for(int i = 0; i < G->V; i++) 
        if(pre[i] == -1)
            if(GRAPHdfs(G, edge(-1, i))) return 1;
    
    return 0;
}

int GRAPHdfs(Graph G, Edge e) {
    int w = e.w;
    pre[w] = cnt++;

    for(link l = G->adj[w]; l != NULL; l = l->next) {
        int t = l->v;
        if(t != e.v)
            if(pre[t] != -1 || pre[t] == -1 && GRAPHdfs(G, edge(w, t))) return 1;
    }

    return 0;
}

// >> Questão 02
/*
    Q.02-A) Seria utilizado uma lista de adjacência pra esse caso pois, como dito no enunciado, os
    pontos de ônibus ou metrô não necessariamente podem estar conectados entre si. Dessa forma, o
    grafo resultante das conexões pode ser muito esparso, dependendo do caso, o que pode ser proble-
    mático em desempenho ao realizar algumas operações. Contudo, é importante ressaltar que, a verifi-
    cação de uma aresta entre dois vértices custa mais em listas de adjacência O(grau(u)) enquanto na
    matriz de ajacência custa O(1).

    Q.02-B) Podemos implementar uma busca em largura ou uma busca em profundidade para esse caso. A
    BFS possui a vantagem de fornecer o menor caminho, porém necessita da implementação de uma fila
    para o seu funcionamento. Em relação ao custo, ambas as operações, no caso da lista de adjacência,
    gasta O(V+E). Para o exercício, será implementada uma DFS pois o menor caminho não é um requisito
    apresentado no enunciado.

    Q.02-C) Podemos apenas rodar a DFS invertendo os vértices de início e de fim para verificarmos tanto
    se existe um caminho da ida quanto para a volta. 

    Q.02-D) Para fazer isso, basta verificar se o vértice possui aresta para qualquer outro vértice. Para
    isso basta verificar se na lista de adjacência do vértice possui link para algum outro vértice, se não
    há, ou seja, a lista está vazia, ele está sozinho no mapa sem conexão com nenhum outro ponto. O custo
    para realizar essa operação é O(1) pois realizamos apenas uma comparação independente do tamanho do grafo.
*/

// Q.02-B -> Implementação
static int cnt2, pre2[MAX];
int GRAPHsearchQ2(Graph G, int start, int end) {
    cnt2 = 0;
    for(int i = 0; i < G->V; i++) pre2[i] = -1;

    return GRAPHdfsQ2(G, start, end);
}

int GRAPHdfsQ2(Graph G, int start, int end) {
    pre2[start] = cnt2++;

    if(start == end) return 1;

    for(link l = G->adj[start]; l != NULL; l = l->next) {
        int t = l->v;
        if(pre2[t] == -1 && GRAPHdfsQ2(G, t, end)) return 1;
    }

    return 0;
}

// Q.02-C -> Implementação
static int cnt3, pre3[MAX];
int GRAPHsearchQ3(Graph G, int start, int end) {
    cnt3 = 0;
    for(int i = 0; i < G->V; i++) pre3[i] = -1;

    if(!GRAPHdfsQ3(G, start, end)) return 0;
    else {
        for(int i = 0; i < G->V; i++) pre3[i] = -1;
        cnt3 = 0;

        if(!GRAPHdfsQ3(G, end, start)) return 0;
    }

    return 1;
}

int GRAPHdfsQ3(Graph G, int start, int end) {
    pre3[start] = cnt3++;

    if(start == end) return 1;

    for(link l = G->adj[start]; l != NULL; l = l->next) {
        int t = l->v;
        if(pre3[t] == -1 && GRAPHdfsQ3(G, t, end)) return 1;
    }

    return 0;
}

// Q.02-D -> Implementação
int isAlone(Graph G, int v) {
    if(G->adj[v] == NULL) return 1;
    return 0;
}