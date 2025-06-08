/*
    Aula 05/06
    >> Grafos Ponderados <<
    - Um Grafo com Peso é um grafo onde a cada aresta está associado um valor numé-
    rico, que é denominado peso ou custo. 

        > Definição: enquanto um grafo não ponderado indica apenas se existe uma
        aresta entre dois vértices, o grafo ponderado quantifica essa conexão.

        > Função do Peso: o peso representa uma métrica específica. Por exemplo,
        o peso represena a distância em quilômetros entre diferentes localidades. 
        Outras métricas comuns são tempo, custo monetário ou largura de banda. 
        
        > Aplicação: A principal utilidade dos pesos é permitir a modelagem e a 
        solução de problemas de otimização. O objetivo frequentemente é encontrar
        um caminho entre vértices que minimize - ou maximize - a soma dos pesos 
        das arestas percorridas.  

    - Nesse arquivo, será implementado apenas a abordagem de grafos representados
    por lista de adjacências. No caso de matrizes de adjacências, um grafo ponde-
    rado armazena o peso da aresta na célula da matriz correspondente aos dois vér-
    tices. Por exemplo:  

            0   1   2
        0 { 0,  40, 20  }
        1 { 40, 0,  30  } 
        2 { 20, 30, 0   }

        > Nesse caso, o 0 representa a ausência de arestas e os outros valores os 
        pesos das arestas existentes (não consideramos a existência de peso nega-
        tivo no momento). 
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define key int
#define data int

typedef struct {
    key k; /// armazena o peso
    data v; // armazena o vértice
} Item;

#define getKey(A) (A.k)
#define less(A, B) (getKey(A) < getKey(B))
#define eq(A, B) (getKey(A) == getKey(B))
#define swap(A, B) {Item temp = A; A = B; B = temp;}

// Implementação da estrutura de dados
typedef struct {
    int v, w;
    int weight;
} Edge;

typedef struct node *Node;
struct node {
    int v;
    int weight;
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

Node NODEnew(int v, int weight, Node next) {
    Node N = malloc(sizeof(*N));
    if(N == NULL) fprintf(stderr, "Falha na alocação de memória.\n");
    N->v = v;
    N->weight = weight;
    N->next = next;
    return N;
}

Graph GRAPHnew(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V, G->E = 0;
    G->adj = malloc(V * sizeof(Node));

    for(int i = 0; i < V; i++) G->adj[i] = NULL;

    return G;
}

void GRAPHinsert(Graph G, Edge e) {
    G->adj[e.v] = NODEnew(e.w, e.weight, G->adj[e.v]);
    G->E++;
}

/*
    >> Algoritmo de Dijkstra <<
    - O objetivo do algoritmo de Dijkstra é resolver o problema do caminho míni-
    mo de origem. Ou seja, ele calcula a menor distância (ou menor custo) de um
    vértice inicial para todos os outros vértices do grafo. 
    - O algoritmo de Dijkstra possui uma regra crucial: Só funciona corretamente
    se todos os pesos das arestas forem POSITIVOS. 

    > Funcionamento:
        - Ele opera de forma gulosa, sempre escolhendo o caminho que parece mais
        promissor no momento. Para fazer isso, ele mantém três principais informa-
        ções: 
            - dist[]: um array que armazena a menor distância conhecida até agora
            da origem até cada vértice;
            - mature[]: um array booleano que nos diz se o caminho para um vértice
            já foi finalizado. Uma vez que um vértice se torna maduro, significa que
            encontramos o caminho mais curto definitivo para ele e não vamos mais
            alterá-lo;
            - pa[]: um array que armazena o predecessor de cada vértice no caminho
            mais curto, permitindo reconstruir o caminho no final. 
*/

// Implementação Dijkstra Ingênuo
void GRAPHdijkstra1(Graph G, int s, int *pa, int *dist) {
    bool mature[G->V + 1];

    for(int i = 0; i < G->V; i++) {
        pa[i] = -1, mature[i] = false, dist[i] = __INT_MAX__;
    }

    pa[s] = s, dist[s] = 0;

    while(true) {
        int min = __INT_MAX__;
        int y; 

        for(int z = 0; z < G->V; z++) {
            if(mature[z]) continue;

            if(dist[z] < min) min = dist[z], y = z;
        }

        if(min == __INT_MAX__) break;

        for(Node a = G->adj[y]; a != NULL; a = a->next) {
            if(mature[a->v]) continue;
            if(dist[y] + a->weight < dist[a->v]) {
                dist[a->v] = dist[y] + a->weight;
                pa[a->v] = y;
            }
        }

        mature[y] = true;
    }   
}

/* 
    Essa implementação custa muito, por conta da parte que procuramos o vértice
    não maduro com a menor distância. Para isso, podemos implementar uma heap p/
    diminuir o custo do dijktra. 
*/

// Implementação da Heap
typedef struct pq *PQ;
struct pq {
    int size;
    Item *pq;
    int *qp;
};

void PQswap(PQ pq, int i, int j) {
    swap(pq->pq[i], pq->pq[j]);
    pq->qp[pq->pq[i].v] = i;
    pq->qp[pq->pq[j].v] = j;
}

void fixUp(PQ pq, int k) {
    while(k > 1 && less(pq->pq[k], pq->pq[k/2])) {
        PQswap(pq, k, k/2);
        k = k/2;
    }
}

void fixDown(PQ pq, int k) {
    while(2 * k <= pq->size) {
        int j = 2 * k;
        if(j < pq->size && less(pq->pq[j+1], pq->pq[j])) j++;
        if(!less(pq->pq[j], pq->pq[k])) break;
        PQswap(pq, k, j);
        k = j;
    }
}

PQ PQinit(int V) {
    PQ pq = malloc(sizeof(*pq));
    pq->size = 0; 
    pq->pq = malloc((V+1) * sizeof(Item));
    pq->qp = malloc(V * sizeof(int));
    for(int i = 0; i < V; i++) pq->qp[i] = -1;
    return pq;
}

int PQempty(PQ pq) {
    return pq->size == 0;
}

int PQcontains(PQ pq, int v) {
    return pq->qp[v] != -1;
}

void PQinsert(PQ pq, Item item) {
    pq->size++;
    pq->qp[item.v] = pq->size;
    pq->pq[pq->size] = item;
    fixUp(pq, pq->size);
}

Item PQdelmin(PQ pq) {
    Item item = pq->pq[1];
    PQswap(pq, 1, pq->size--);
    fixDown(pq, 1);
    pq->qp[item.v] = -1;
    return item;
}

void PQchange(PQ pq, Item item) {
    int pos = pq->qp[item.v];
    pq->pq[pos].k = item.k;
    fixUp(pq, pos);
    fixDown(pq, pos);
}

// Implementação do Dijkstra com Heap
void GRAPHdijkstraHeap(Graph G, int s, int *pa, int *dist) {
    for(int i = 0; i < G->V; i++) {
        dist[i] = __INT_MAX__;
        pa[i] = -1;
    }

    dist[s] = 0, pa[s] = s;

    PQ pq = PQinit(G->V);
    PQinsert(pq, (Item){.k = 0, .v = s});

    while(!PQempty(pq)) {
        Item item = PQdelmin(pq);
        int v = item.v;

        if(item.k < dist[v]) continue;

        for(Node l = G->adj[v]; l != NULL; l = l->next) {
            if(dist[v] != __INT_MAX__ && dist[v] + l->weight < dist[l->v]) {
                dist[l->v] = dist[v] + l->weight;
                pa[l->v] = v;

                Item i = {.k = dist[l->v], .v = l->v};
                if(PQcontains(pq, l->v)) PQchange(pq, i);
                else PQinsert(pq, i);
            }
        }
    }

    free(pq->pq), free(pq->qp), free(pq);
}