#include <stdio.h>
#include <stdlib.h>

#define MAX 2501

typedef struct {
    int v, w;
} Edge;

typedef struct graph *Graph;
struct graph {
    int V, E;
    int **adj;
};

Edge newEDGE(int v, int w) {
    Edge e;
    e.v = v, e.w = w;
    return e;
}

int **MATRIXinit(int a, int b, int n) {
    int **m = malloc(a * sizeof(int*));

    for(int i = 0; i < a; i++)
        m[i] = malloc(b * sizeof(int));
    
    for(int i = 0; i < a; i++) 
        for(int j = 0; j < b; j++)
            m[i][j] = n;
    
    return m;
}

Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V, G->E = 0;
    G->adj = MATRIXinit(V, V, 0);
    return G;
}

void GRAPHinsert(Graph G, Edge e) {
    int v = e.v, w = e.w;
    if(G->adj[v][w] == 0) G->E++;
    G->adj[v][w] = 1;
    G->adj[w][v] = 1;
}

static int pre[MAX], cnt, componente[MAX], conexos;

void GRAPHdfs(Graph G, Edge e) {
    int w = e.w;
    pre[w] = cnt++;
    componente[w] = conexos;

    for(int i = 0; i < G->V; i++) {
        if(G->adj[w][i] != 0) {
            if(pre[i] == -1) {
                GRAPHdfs(G, newEDGE(w, i));
            }
        }
    }
}

void GRAPHsearch(Graph G) {
    conexos = 0;
    cnt = 0;

    for(int i = 0; i < G->V; i++) pre[i] = -1, componente[i] = -1;

    for(int i = 0; i < G->V; i++) {
        if(pre[i] == -1) {
            GRAPHdfs(G, newEDGE(i, i));
            conexos++;
        }
    }
}

int main() {
    int N;
    scanf("%d", &N);

    Graph G = GRAPHinit(N);

    char str[N+1];
    for(int i = 0; i < N; i++) {
        scanf("%s", str);
        for(int j = 0; j < N; j++) {
            if(str[j] == '1')
                GRAPHinsert(G, newEDGE(i, j));
        }
    }

    GRAPHsearch(G);

    int E;
    scanf("%d", &E);

    int participantes[MAX], contComponentes[MAX];

    for(int i = 0; i < E; i++) {
        int A;
        scanf("%d", &A);
        for(int j = 0; j < A; j++) {
            scanf("%d", &participantes[j]);
            participantes[j]--;
        }

        for(int j = 0; j < N; j++) contComponentes[j] = 0;

        char ans = 'N';
        for(int x = 0; x < A; x++) {
            int component = componente[participantes[x]];
            contComponentes[component]++;

            if(contComponentes[component] > 1) {
                ans = 'S';
                break;
            } else {
                contComponentes[component]++;
            }

            if(ans == 'S') break;
        }

        printf("%c\n", ans);
    }

    return 0;
}