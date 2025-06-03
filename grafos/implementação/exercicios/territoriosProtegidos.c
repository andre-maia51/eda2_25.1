#include <stdio.h>
#include <stdlib.h>

#define MAX 200000

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

link newNode(int v, link next) {
    link x = malloc(sizeof(*x));
    if(x == NULL) fprintf(stderr, "Falha na alocação de memória.\n");
    x->v = v; x->next = next;
    return x;
}

Graph graphInit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V; G->E = 0;
    G->adj = malloc(V * sizeof(link));

    for(int i = 0; i < V; i++) G->adj[i] = NULL;

    return G;
}

void graphInsert(Graph G, Edge e) {
    int v = e.v, w = e.w;
    G->adj[v] = newNode(w, G->adj[v]);
    G->adj[w] = newNode(v, G->adj[w]);
    G->E++;
}

static int count, pre[MAX];
int DFSR(Graph G, Edge e) {
    int w = e.w;
    pre[w] = count++;
    int result = w;

    for(link l = G->adj[w]; l != NULL; l = l->next) {
        int t = l->v;
        if(pre[t] == -1) {
            int partialResult = DFSR(G, edge(w, t));
            result = result | partialResult;
        }
    }

    return result;
}

int graphSearch(Graph G) {
    count = 0;
    int conexos = 0, partial, components[MAX];
    int finalResult = 0;


    for(int i = 0; i < G->V; i++) pre[i] = -1;

    for(int i = 0; i < G->V; i++) {
        if(pre[i] == -1) {
            partial = DFSR(G, edge(i, i));
            components[conexos++] = partial;
        }
    }   

    for(int i = 0; i < conexos; i++) {
        finalResult = finalResult ^ components[i];
    }

    return finalResult;
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);

    Graph G = graphInit(N+1);

    int u, v;
    for(int i = 0; i < M; i++) {
        scanf("%d %d", &u, &v);
        graphInsert(G, edge(u, v));
    }

    printf("%d\n", graphSearch(G));
    
    
    return 0;
}