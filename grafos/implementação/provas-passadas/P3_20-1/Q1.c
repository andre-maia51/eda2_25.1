#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v, w;
} Edge;

typedef struct node *Link;
struct node {
    int v;
    Link next;
};

typedef struct graph *Graph;
struct graph {
    int V, E;
    Link *adj;
};

Edge EDGEnew(int v, int w) {
    Edge e;
    e.v = v, e.w = w;
    return e;
}

Link NODEnew(int v, Link next) {
    Link x = malloc(sizeof(*x));
    if(x == NULL) fprintf(stderr, "Falha na alocação de memória.\n");
    x->v = v;
    x->next = next;
    return x;
}

Graph GRAPHnew(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V, G->E = 0;
    G->adj = malloc(V * sizeof(Link));

    for(int i = 0; i < V; i++)
        G->adj[i] = NULL;
    
    return G;
}

void GRAPHinsert(Graph G, Edge e) {
    int v = e.v, w = e.w;
    G->adj[v] = NODEnew(w, G->adj[v]);
    G->adj[w] = NODEnew(v, G->adj[w]);
    G->E++;
}

void GRAPHdfs(Graph G, Edge e, int *visited) {
    int w = e.w;
    visited[w] = 1;

    for(Link l = G->adj[w]; l != NULL; l = l->next) {
        int t = l->v;
        if(visited[t] == 0)
            GRAPHdfs(G, EDGEnew(w, t), visited);
    }
}

int GRAPHsearch(Graph G, int *visited) {
    int conexos = 0;

    for(int i = 0; i < G->V; i++) visited[i] = 0;
    for(int i = 0; i < G->V; i++) {
        if(visited[i] == 0) {
            GRAPHdfs(G, EDGEnew(i, i), visited);
            conexos++;
        }
    }

    return conexos;
}

int main() {
    int V;
    scanf("%d", &V);

    Graph G = GRAPHnew(V);

    int v, w;
    while (scanf("%d %d", &v, &w) != EOF) {
        GRAPHinsert(G, EDGEnew(v, w));
    }

    int visited[V];
    int res = GRAPHsearch(G, visited);

    printf("%d\n", res);

    return 0;
}