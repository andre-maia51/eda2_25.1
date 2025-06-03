#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v, w;
} Edge;

typedef struct node *link;
struct node {
    int v;
    link next;
};

typedef struct graph *Graph;
struct graph {
    int V, E;
    link *adj;
};

Edge EDGEnew(int v, int w) {
    Edge e;
    e.v = v, e.w = w;
    return e;
}

link NODEnew(int v, link next) {
    link x = malloc(sizeof(*x));
    if(x == NULL) fprintf(stderr, "falha na alocação de memória.\n");
    x->v = v;
    x->next = next;
    return x;
}

Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V, G->E = 0;
    G->adj = malloc(V * sizeof(link));

    for(int i = 0; i < V; i++) G->adj[i] = NULL;

    return G;
}

void GRAPHinsert(Graph G, Edge e) {
    G->adj[e.v] = NODEnew(e.w, G->adj[e.v]);
    G->E++;
}

/*
    Q.06-A) Eu utilizaria uma lista de adjacência pois, o grafo pode
    ser muito esparso, principalmente por ser direcionado, aumentando
    o custo de algumas operações. O uso da lista ao invés da matriz 
    possui a desvantagem na hora da operação para saber se existe uma
    aresta no grafo, pois o custo é maior que da matriz, sendo o linear
    em relação ao grau do vértice, visto que devemos percorrer a lista
    de adjacência para realizar a operação O(grau(V)). 

    Q.06-B) Podemos realizar uma BFS ou DFS para determinar se existe 
    um caminho de um vértice A para um vértice B. A complexidade de
    ambas as buscas é O(V+E) nas listas de adjacência e ambas as buscas
    necessitam de uma estrutura auxiliar, a BFS da fila e a DFS da pilha
    (que pode ser representada pela pilha de recursão, tornando a imple-
    mentação mais simples).

    Q.06-C) O custo dessa operação é linear O(V+E), pois devemos percorrer,
    para cada vértice, sua lista de adjacências e verificar se possui alguma
    aresta para o vértice que queremos descobrir se é deserto. 

*/

// Implementação Q.06-B
static int *visited;
int isPossible(Graph G, int start, int end) {
    int res;
    visited = malloc(G->V * sizeof(int));

    for(int i = 0; i < G->V; i++) visited[i] = 0;

    res = GRAPHdfs(G, start, end);
    
    free(visited);
    return res;
} 

int GRAPHdfs(Graph G, int start, int end) {
    visited[start] = 1;

    if(start == end) return 1;

    for(link l = G->adj[start]; l != NULL; l = l->next) {
        if(visited[l->v] == 0 && GRAPHdfs(G, l->v, end))
            return 1;
    }

    return 0;
}

// Implementação Q.06-C

int isIsolated(Graph G, int V) {
    int res;

    for(int i = 0; i < G->V; i++) {
        for(link l = G->adj[i]; l != NULL; l = l->next) {
            if(l->v == V) return 0;
        }
    }

    return 1;
}