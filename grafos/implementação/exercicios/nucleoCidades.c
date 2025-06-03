#include <stdio.h>
#include <stdlib.h>

#define MAX_CITY 5001

typedef struct {
    int v, w;
} Edge;

typedef struct node *link;
struct node {
    int x;
    link next;
};

typedef struct graph *Graph;
struct graph {
    int V, E;
    link *adj;
};

Edge edge(int v, int w) {
    Edge e;
    e.v = v, e.w = w;
    return e;
}

link newNode(int x, link next) {
    link a = malloc(sizeof(*a));
    if(a == NULL) fprintf(stderr, "Falha na alocação da memória\n");
    a->x = x;
    a->next = next;
    return a;
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
    int v = e.v, w = e.w;

    G->adj[v] = newNode(w, G->adj[v]);
    G->adj[w] = newNode(v, G->adj[w]);
    G->E++;
}

static int cnt, pre[MAX_CITY];

void DFSR(Graph, Edge);

int GraphSearch(Graph G) {
    int conexos = 0;
    cnt = 0;

    for(int i = 0; i < G->V; i++) pre[i] = -1;

    for(int i = 0; i < G->V; i++) {
        if(pre[i] == -1) {
            DFSR(G, edge(i, i));
            conexos++;
        }
    }

    return conexos;

}

void DFSR(Graph G, Edge e) {
    int w = e.w;
    pre[w] = cnt++;

    for(link l = G->adj[w]; l != NULL; l = l->next) {
        int t = l->x;
        if(pre[t] == -1) DFSR(G, edge(w, t));
    }
}

int main() {
    int C;
    scanf("%d", &C);

    Graph G = GraphInit(C);

    int v, w;
    while(scanf("%d %d", &v, &w) != EOF) {
        GraphInsert(G, edge(v, w));
    }

    printf("%d\n", GraphSearch(G));

    return 0;
}