#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 101

typedef struct {
    int v, w;
    int weight;
} Edge;

typedef struct node *Node;
struct node {
    int v, weight;
    Node next;
};

typedef struct graph *Graph;
struct graph {
    int V, E;
    Node *adj;
};

Edge EDGEnew(int v, int w, int weight) {
    Edge e;
    e.v = v; e.w = w, e.weight = weight;
    return e;
}

Node NODEnew(int v, Node next, int weight) {
    Node x = malloc(sizeof(*x));
    x->v = v, x->weight = weight, x->next = next;
    return x;
}

Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V, G->E = 0;
    G->adj = malloc(V * sizeof(Node));

    for(int i = 0; i < V; i++) G->adj[i] = NULL;

    return G;
}

void GRAPHinsert(Graph G, Edge e) {
    G->adj[e.v] = NODEnew(e.w, G->adj[e.v], e.weight);
    G->adj[e.w] = NODEnew(e.v, G->adj[e.w], e.weight);
    G->E++;
}

// Implementação da Min-Heap
typedef struct {
    int weight, vertex;
} Item;

#define getWeight(A) (A.weight)
#define less(A, B) (getWeight(A) < getWeight(B))
#define eq(A, B) (getWeight(A) == getWeight(B))
#define swap(A, B) {Item temp = A; A = B; B = temp;}

typedef struct pq *PQ;
struct pq {
    int N;
    Item *pq;
    int *qp;
};

void PQswap(PQ PQ, int i, int j) {
    swap(PQ->pq[i], PQ->pq[j]);
    PQ->qp[PQ->pq[i].vertex] = i;
    PQ->qp[PQ->pq[j].vertex] = j;
}

void fixUp(PQ PQ, int k) {
    while( k > 1 && less(PQ->pq[k], PQ->pq[k/2])) {
        PQswap(PQ, k, k/2);
        k = k/2;
    }
}

void fixDown(PQ PQ, int k) {
    while(2*k <= PQ->N) {
        int j = 2*k;
        if(j < PQ->N && less(PQ->pq[j+1], PQ->pq[j])) j++;
        if(less(PQ->pq[k], PQ->pq[j])) break;
        PQswap(PQ, k, j);
        k = j;
    }
}

PQ PQinit(int V) {
    PQ PQ = malloc(sizeof(*PQ));
    PQ->N = 0;
    PQ->pq = malloc((V + 1) * sizeof(Item));
    PQ->qp = malloc((V + 1) * sizeof(int));

    for(int i = 0; i < V; i++) PQ->qp[i] = -1;

    return PQ;
}

int PQempty(PQ PQ) {
    return PQ->N == 0;
}

int PQcontains(PQ PQ, int i) {
    return PQ->qp[i] != -1;
}

void PQinsert(PQ PQ, Item v) {
    PQ->N++;
    PQ->qp[v.vertex] = PQ->N;
    PQ->pq[PQ->N] = v;
    fixUp(PQ, PQ->N);
}

Item PQdelMin(PQ PQ) {
    Item i = PQ->pq[1];
    PQswap(PQ, 1, PQ->N--);
    fixDown(PQ, 1);
    PQ->qp[i.vertex] = -1;
    return i;
}

void PQchange(PQ PQ, Item i) {
    int pos = PQ->qp[i.vertex];
    PQ->pq[pos].weight = i.weight;
    fixUp(PQ, pos);
    fixDown(PQ, pos);
}

void DijkstraHeap(Graph G, int start, int *pa, int *dist) {
    for(int i = 0; i < G->V; i++)
        dist[i] = INT_MAX, pa[i] = -1;
    
    pa[start] = start, dist[start] = 0;

    PQ PQ = PQinit(G->V);
    PQinsert(PQ, (Item){.weight = 0, .vertex = start});

    while(!PQempty(PQ)) {
        Item item = PQdelMin(PQ);
        int v = item.vertex;

        if(item.weight > dist[v]) continue;

        for(Node l = G->adj[v]; l != NULL; l = l->next) {
            if(dist[v] != INT_MAX && dist[v] + l->weight < dist[l->v]) {
                dist[l->v] = dist[v] + l->weight;
                pa[l->v] = v;

                Item i = {.weight = dist[l->v], .vertex = l->v};
                if(PQcontains(PQ, l->v)) PQchange(PQ, i);
                else PQinsert(PQ, i);
            }
        }
    }
}

int main(void) {
    int N, M;
    scanf("%d %d", &N, &M);

    Graph G = GRAPHinit(N+1);

    for(int i = 1; i <= M; i++) {
        int A, B, C;
        scanf("%d %d %d", &A, &B, &C);
        GRAPHinsert(G, EDGEnew(A, B, C));
    }

    int pa[MAX_N], dist[MAX_N];
    DijkstraHeap(G, 1, pa, dist);

    printf("%d\n", dist[N]);
}
