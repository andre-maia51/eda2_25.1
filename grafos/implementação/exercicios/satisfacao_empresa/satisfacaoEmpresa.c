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

Edge EDGE(int v, int w) {
    Edge e;
    e.v = v, e.w = w;
    return e;
}

link NODEnew(int v, link next) {
    link x = malloc(sizeof(*x));
    if(x == NULL) fprintf(stderr, "Falha na alocação de memória.\n");
    x->v = v, x->next = next;
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
    int v = e.v, w = e.w;
    G->adj[v] = NODEnew(w, G->adj[v]);
    G->E++;
}

int GRAPHcount(Graph G, int *salary) {
    int res = 0;

    for(int i = 0; i < G->V; i++) {
        for(link l = G->adj[i]; l != NULL; l = l->next) {
            if(salary[i] < salary[l->v]) {
                res++;
                break;
            }
        }
    }

    return res;
}

int main() {
    int N;
    scanf("%d", &N);

    Graph G = GRAPHinit(N+1);
    int salaries[N+1];

    for(int i = 1; i <= N; i++) {
        int boss, salary;
        scanf("%d %d", &boss, &salary);

        GRAPHinsert(G, EDGE(boss, i));
        salaries[i] = salary;
    }

    printf("%d\n", GRAPHcount(G, salaries));

    int A;
    scanf("%d", &A);
    for(int i = 0; i < A; i++) {
        int j, x;
        scanf("%d %d", &j, &x);
        salaries[j] = x;
        printf("%d\n", GRAPHcount(G, salaries));
    }

    return 0;
}