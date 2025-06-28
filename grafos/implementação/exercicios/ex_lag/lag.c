#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_SIZE 2001

// Estrutura básica do Grafo
typedef struct {
    int v, w, weight;
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
    e.v = v, e.w = w, e.weight = weight;
    return e;
}

Node NODEnew(int v, Node next, int weight) {
    Node n = malloc(sizeof(*n));
    n->v = v;
    n->weight = weight;
    n->next = next;

    return n;
}

Graph GRAPHnew(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V, G->E = 0;
    G->adj = malloc(V * sizeof(Node));

    for(int i = 0; i < V; i++) G->adj[i] = NULL;
    
    return G;
}

void GRAPHinsertDirected(Graph G, Edge e) {
    G->adj[e.v] = NODEnew(e.w, G->adj[e.v], e.weight);
    G->E++;
}

void GRAPHinsertUndirected(Graph G, Edge e) {
    G->adj[e.v] = NODEnew(e.w, G->adj[e.v], e.weight);
    G->adj[e.w] = NODEnew(e.v, G->adj[e.w], e.weight);
    G->E++;
}

// Estrutura da Fila e implementação do Bellman-Ford
typedef struct queue *Queue;
struct queue {
    int front, back;
    int size;
    int *item;
};

Queue QUEUEinit(int V) {
    Queue Q = malloc(sizeof(*Q));
    Q->front = 0, Q->back = 0;
    Q->size = V;
    Q->item = malloc(V * sizeof(int));
    return Q;
}

int QUEUEfull(Queue Q) {
    return Q->back == Q->size;
}

int QUEUEempty(Queue Q) {
    return Q->front == Q->back; 
}

int QUEUEget(Queue Q) {
    return Q->item[Q->front++];
}

void QUEUEinsert(Queue Q, int X) {
    Q->item[Q->back++] = X;
}

void QUEUEdelete(Queue Q) {
    free(Q->item);
    free(Q);
}

bool GRAPHbellmanFord(Graph G, int s, int *pa, int *dist) {
    int count[MAX_SIZE];
    bool onqueue[MAX_SIZE];
    for(int i = 0; i < G->V; i++) {
        pa[i] = -1;
        dist[i] = INT_MAX;
        onqueue[i] = false;
        count[i] = 0;
    }

    pa[s] = s;
    dist[s] = 0;
    
    Queue Q = QUEUEinit(G->V * 2);

    QUEUEinsert(Q, s);
    onqueue[s] = true;
    count[s]++;

    while(!QUEUEempty(Q)) {
        int u = QUEUEget(Q);
        onqueue[u] = false;

        for(Node l = G->adj[u]; l != NULL; l = l->next) {
            if (dist[u] != INT_MAX && dist[u] + l->weight < dist[l->v]) {
                dist[l->v] = dist[u] + l->weight;
                pa[l->v] = u;

                if (!onqueue[l->v]) {
                    QUEUEinsert(Q, l->v);
                    onqueue[l->v] = true;
                    count[l->v]++;
                    
                    if (count[l->v] >= G->V) {
                        QUEUEdelete(Q);
                        return false; 
                    }
                }
            }
        }
    }

    QUEUEdelete(Q);
    return true;
}

// Estrutura da Heap e implementação do Dijkstra
typedef struct {
    int weight;
    int vertex;
}Item;

#define getWeight(A) (A.weight)
#define less(A, B) (getWeight(A) < getWeight(B))
#define swap(A, B) {Item temp = A; A = B; B = temp;}

typedef struct pq *PQ;
struct pq {
    int size;
    Item *pq;
    int *qp;
};

void PQswap(PQ PQ, int i, int j) {
    swap(PQ->pq[i], PQ->pq[j]);
    PQ->qp[PQ->pq[i].vertex] = i;
    PQ->qp[PQ->pq[j].vertex] = j;
}

void fixUp(PQ PQ, int k) {
    while(k > 1 && less(PQ->pq[k], PQ->pq[k/2])) {
        PQswap(PQ, k, k/2);
        k = k/2;
    }
}

void fixDown(PQ PQ, int k) {
    int j;

    while(2*k <= PQ->size) {
        j = 2*k;

        if(j < PQ->size && less(PQ->pq[j+1], PQ->pq[j])) j++;
        if(less(PQ->pq[k], PQ->pq[j])) break;
        PQswap(PQ, k, j);
        k = j;
    }
}

PQ PQinit(int V) {
    PQ pq = malloc(sizeof(*pq));
    pq->size = 0;
    pq->pq = malloc((V+1) * sizeof(Item));
    pq->qp = malloc((V+1) * sizeof(int));

    for(int i = 0; i < V; i++) pq->qp[i] = -1;

    return pq;
}

int PQempty(PQ PQ) {
    return PQ->size == 0;
}

int PQcontains(PQ PQ, int V) {
    return PQ->qp[V] != -1;
}

void PQinsert(PQ PQ, Item item) {
    PQ->size++;
    PQ->qp[item.vertex] = PQ->size;
    PQ->pq[PQ->size] = item;
    fixUp(PQ, PQ->size);
}

Item PQdelMin(PQ PQ) {
    Item item = PQ->pq[1];
    PQswap(PQ, 1, PQ->size--);
    fixDown(PQ, 1);
    PQ->qp[item.vertex] = -1;
    return item;
}

void PQchange(PQ PQ, Item item) {
    int pos = PQ->qp[item.vertex];
    PQ->pq[pos].weight = item.weight;
    fixUp(PQ, pos);
    fixDown(PQ, pos);
}

void PQdelete(PQ PQ) {
    free(PQ->pq), free(PQ->qp), free(PQ);
}

void GRAPHdijkstra(Graph G, int s, int *pa, int *dist) {
    for(int i = 0; i < G->V; i++) {
        pa[i] = -1, dist[i] = INT_MAX;
    }

    dist[s] = 0, pa[s] = s;
    PQ PQ = PQinit(G->V);
    PQinsert(PQ, (Item){.vertex = s, .weight = 0});

    while(!PQempty(PQ)) {
        Item item = PQdelMin(PQ);
        int v = item.vertex;

        if(item.weight > dist[v]) continue;

        for(Node l = G->adj[v]; l != NULL; l = l->next) {
            if(dist[v] != INT_MAX && dist[v] + l->weight < dist[l->v]) {
                dist[l->v] = dist[v] + l->weight;
                pa[l->v] = v;

                Item item = {.weight = dist[l->v], .vertex = l->v};
                if(PQcontains(PQ, item.vertex)) PQchange(PQ, item);
                else PQinsert(PQ, item);
            }
        }
    }
    PQdelete(PQ);
}


int main(void) {
    int V;
    scanf("%d", &V);
    
    bool dijkstraCalculated[MAX_SIZE], negativeCycle[MAX_SIZE], BFcalculated[MAX_SIZE];
    int **distDijkstra = malloc(V * sizeof(int*));
    int **distBellmanFord = malloc(V * sizeof(int*));

    for(int i = 0; i < V; i++) {
        distDijkstra[i] = malloc(V * sizeof(int));
        distBellmanFord[i] = malloc(V * sizeof(int));
    }

    for(int i = 0; i < V; i++) {
        dijkstraCalculated[i] = false, negativeCycle[i] = false, BFcalculated[i] = false;
    }

    Graph GraphSC = GRAPHnew(V);
    Graph GraphWithoutSC = GRAPHnew(V);

    int v, w, c, t;

    do {
        scanf("%d %d %d %d", &v, &w, &c, &t);
        if(v == 0 && w == 0 && c == 0 && t == 0) break;

        if(t == 0) {
            GRAPHinsertUndirected(GraphSC, EDGEnew(v, w, c));
            GRAPHinsertUndirected(GraphWithoutSC, EDGEnew(v, w, c));
        } else {
            GRAPHinsertDirected(GraphSC, EDGEnew(v, w, c));
        }
 
    } while(v != 0 || w != 0 || c != 0 || t != 0);
    
    int o, x;
    while(scanf("%d %d", &o, &x) != EOF) {
        if(!dijkstraCalculated[o]) {
            int paD[V+1], distD[V+1];
            GRAPHdijkstra(GraphWithoutSC, o, paD, distD);
            for(int i = 0; i < V; i++) distDijkstra[o][i] = distD[i];
            dijkstraCalculated[o] = true;
        }

        if(!negativeCycle[o]) {
            if(!BFcalculated[o]) {
                int paBF[V+1], distBF[V+1];
                bool resBF = GRAPHbellmanFord(GraphSC, o, paBF, distBF);
    
                if(!resBF) negativeCycle[o] = true;
                else {
                    for(int i = 0; i < V; i++) distBellmanFord[o][i] = distBF[i];
                }
            }
        }

        if(negativeCycle[o]) printf("Impossibru\n");
        else {
            if(distDijkstra[o][x] == INT_MAX || distBellmanFord[o][x] == INT_MAX) printf("Impossibru\n");
            else printf("%d %d\n", distDijkstra[o][x], distBellmanFord[o][x]);
        }
    }
    
    
    return 0;
}