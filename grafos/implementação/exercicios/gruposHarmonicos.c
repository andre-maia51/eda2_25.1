#include <stdio.h>
#include <stdlib.h>

#define MAX 100000

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
    x->v = v;
    x->next = next;
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

// Implementação da Fila
struct queue
{
    int front;
    int back;
    int items[MAX];
};

typedef struct queue *Queue;

void QueueInit(Queue q) {
    q->front = 0;
    q->back = 0;
}

int QueueEmpty(Queue q) {
    return q->front == q->back;
}

int QueueFull(Queue q) {
    return q->back == MAX;
}

int QueueGet(Queue q) {
    return q->items[q->front++];
}

void QueuePut(Queue q, int v) {
    q->items[q->back++] = v;
}

static int count, pre[MAX], color[MAX];

int BFS(Graph G, Edge e) {
    Queue Q = malloc(sizeof(*Q));
    QueueInit(Q);

    int v = e.v;

    QueuePut(Q, v);
    pre[v] = count++;
    color[v] = 0;

    while(!QueueEmpty(Q)) {
        int x = QueueGet(Q);

        for(link l = G->adj[x]; l != NULL; l = l->next) {
            int t = l->v;

            if(pre[t] == -1) {
                color[t] = !color[x];
                QueuePut(Q, t);
                pre[t] = count++;
            } else if(color[t] == color[x]) return 0;
        }
    }

    free(Q);
    return 1;
   
}

int graphSearch(Graph G) {
    count = 0;

    for(int i = 0; i < G->V; i++) {
        pre[i] = -1;
        color[i] = -1;
    }

    for(int i = 0; i < G->V; i++) {
        if(pre[i] == -1) {
            if(BFS(G, edge(i, i)) == 0) return 0;
        }
    }

    return 1;
}


 
int main() {
    int N, M;
    scanf("%d %d", &N, &M);

    Graph G = graphInit(N);

    int u, v;
    for(int i = 0; i < M; i++) {
        scanf("%d %d", &u, &v);
        graphInsert(G, edge(u, v));
    }

    if(graphSearch(G) == 0) printf("Nao\n");
    else printf("Sim\n");

    return 0;
}