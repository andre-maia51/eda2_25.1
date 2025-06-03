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

Graph GRAPHnew(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V, G->E = 0;
    G->adj = malloc(V * sizeof(link));

    for(int i = 0; i < V; i++) G->adj[i] = NULL;

    return G;
}

void GRAPHinsert1(Graph G, Edge e) {
    G->adj[e.v] = NODEnew(e.w, G->adj[e.v]);
    G->E++;
}

void GRAPHinsert2(Graph G, Edge e) {
    G->adj[e.v] = NODEnew(e.w, G->adj[e.v]);
    G->adj[e.w] = NODEnew(e.v, G->adj[e.w]);
    G->E++;
}

int GRAPHdfs(Graph G, int start, int end, int *visited) {
    visited[start] = 1;
    if(start == end) return 1;

    for(link l = G->adj[start]; l != NULL; l = l->next) {
        if(visited[l->v] == 0 && GRAPHdfs(G, l->v, end, visited))
            return 1;
    }

    return 0;
}

void GRAPHsearch(Graph G, int start, int end, int *visited) {
    for(int i = 0; i < G->V; i++) visited[i] = 0;
    int ida = GRAPHdfs(G, start, end, visited);

    for(int i = 0; i < G->V; i++) visited[i] = 0;
    int volta = GRAPHdfs(G, end, start, visited);

    if(ida && volta) printf("Ida e Volta\n");
    else if(ida) printf("Apenas Ida\n");
    else if(volta) printf("Apenas Volta\n");
    else printf("Impossibru\n");
}

int main() {
    int V;
    scanf("%d", &V);

    Graph G = GRAPHnew(V);
    int visited[V];

    int v, w, d;
    while(1) {
        scanf("%d %d %d", &v, &w, &d);

        if(v == 0 && w == 0 && d == 0) break;

        if(d == 1) GRAPHinsert1(G, EDGEnew(v, w));
        else if(d == 2) GRAPHinsert2(G, EDGEnew(v, w));
    }


    int t, x;
    while(scanf("%d %d", &t, &x) != EOF) {
        GRAPHsearch(G, t, x, visited);
    }

    return 0;
}