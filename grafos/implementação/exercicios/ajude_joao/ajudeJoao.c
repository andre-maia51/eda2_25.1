#include <stdio.h>
#include <stdlib.h>

#define MAX 5001

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

Edge edge(int v, int w) {
    Edge e;
    e.v = v; e.w = w;
    return e;
}

link NewNode(int v, link next) {
    link n = malloc(sizeof(*n));
    if(n == NULL) fprintf(stderr, "Falha na alocação de memória\n");
    n->v = v;
    n->next = next;
    return n;
}

Graph GraphInit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V; G->E = 0;
    G->adj = malloc(V * sizeof(link));

    for(int i = 0; i < V; i++) {
        G->adj[i] = NULL;
    }

    return G;
}

void GraphInsert(Graph G, Edge E) {
    int v = E.v, w = E.w;

    G->adj[v] = NewNode(w, G->adj[v]);
    G->adj[w] = NewNode(v, G->adj[w]);
}

static int count, pre[MAX];

void DFSR(Graph G, Edge e) {
    int w = e.w;
    pre[w] = count++;

    for(link l = G->adj[w]; l != NULL; l = l->next) {
        int t = l->v;
        if(pre[t] == -1) {
            DFSR(G, edge(w, t));
        }
    }

}

int GraphSearch(Graph G, Edge e) {
    int conexos = 0;
    count = 0;

    for(int i = 0; i < G->V; i++) pre[i] = -1;

    for(int i = 0; i < G->V; i++) {
        if(pre[i] == -1) {
            DFSR(G, edge(i, i));
            conexos++;
        }
    }
}

int main() {
    int V, J;
    scanf("%d %d", &V, &J);

    Graph G = GraphInit(V);

    int v, w;
    while(scanf("%d %d", &v, &w) != EOF) {
        GraphInsert(G, edge(v, w));
    }





    return 0;
}