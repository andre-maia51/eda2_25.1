#include <stdio.h>
#include <stdlib.h>
#define MAXN 2001

typedef struct {
    int v, w;
} Edge;

typedef struct node *link;

struct node {
    int v;
    link next;
};



struct graph {
    int V, E;
    link *adj;
};

typedef struct graph *Graph;

Edge edge(int v, int w) {
    Edge e;
    e.v = v;
    e.w = w;
    return e;
}

void tela_azul() {
    fprintf(stderr, "Falha na alocação de memória.\n");
}

link NEW(int v, link next) {
    link x = malloc(sizeof(*x));
    if(x == NULL) tela_azul();
    x->v = v;
    x->next = next;
    return x;
}

Graph GraphInit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->adj = malloc(V * sizeof(link));

    for(int i = 0; i < V; i++) {
        G->adj[i] = NULL;
    }

    return G;
}

void GraphInsert(Graph G, Edge e) {
    int v = e.v;
    int w = e.w;

    G->adj[v] = NEW(w, G->adj[v]);
    G->E++;
}


int main () {
    int N, M, J;
    scanf("%d%d%d", &N, &M, &J);

    Graph G = GraphInit(N);
    int julianoRef[MAXN] = {0};

    for(int i = 0; i < N; i++) {
        int A;
        scanf("%d", &A);
        for(int j = 0; j < A; j++) {
            int X;
            scanf("%d", &X);
            GraphInsert(G, edge(i, X));
        }
    }

    for(int i = 0; i < M; i++) {
        int l;
        scanf("%d", &l);

        julianoRef[l] = 1;

        for(link t = G->adj[l]; t != NULL; t = t->next) {
            julianoRef[t->v] = 1;
        }
    }

    for(int i = 0; i < J; i++) {
        int z;
        scanf("%d", &z);

        if(julianoRef[z] == 1) printf("Eu vou estar la\n");
        else printf("Nao vou estar la\n");
    }

    return 0;
}