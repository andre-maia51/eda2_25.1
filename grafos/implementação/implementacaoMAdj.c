/*
    Aula 06/05
    >> GRAFOS <<
    
    O que é um grafo?
        - Um conjunto te vértices com um conjunto de arestas
    - Vértices são numerados de 0 a V-1

    -- Grafo Completo > Um grafo com V vértices possui no máximo (V * (V-1))/2
    arestas
    
    Um caminho no grafo é um sequência de vértices em que cada vértice sucessivo é 
    adjacente ao predecessir no caminho

    -- Caminho Simples > Os vértices e arestas são distintos
        - Exemplo: [0, 1, 2, 3]

    -- Ciclo > É um caminho que é simples exceto pelo primeiro e último vértice que
    são os mesmos
        - Exemplo: [0, 1, 2, 0]

    -- Grafo Conexo > Um grafo é conexo se há um caminho de cada vértice para todo
    outro vértice no grafo
        - Um grafo que não é conexo consiste de um conjunto de componentes conexos

    -- Grafo Acíclico > Um grafo acíclico é um grafo que não possui ciclos. Um grafo
    conexo acíclico também é chamado de Árvore
        - Um conjunto de Árvores é chamado de Floresta
*/


/*
    Matriz de Adjacência
    > É uma matriz adj[V][V] onde adj[i][j] = 1 se existe uma aresta entre i e j, caso
    o valor seja 0, não existe aresta entre eles.
*/

// Implementação
#include <stdio.h>
#include <stdlib.h>

// Estrutura de uma aresta
typedef struct {
    int v;
    int w;
} Edge;

// Estrutura do Grafo
struct graph{
    int V;
    int E;
    int **mAdj;
    int **tc; // matriz que armazena o fecho transitivo -> transitive closure
};

typedef struct graph *Graph;

// Função para Inicializar a Matriz de Adjacências
int **MatrixInit(int a, int b, int n) {
    int **matrix = malloc(a * sizeof(int *));
    for(int i = 0; i < a; i++) {
        matrix[i] = malloc(b * sizeof(int));
    }

    for( int i = 0; i < a; i++) {
        for(int j = 0; j < b; j++) {
            matrix[i][j] = n;
        }
    }

    return matrix;
}

// Função que cria uma aresta entre dois vértices 
Edge edge(int v, int w) {
    Edge e;
    e.v = v;
    e.w = w;
    return e;
}

// Função que incializa um Grafo com V vértices
Graph GraphInit(int V) {
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->mAdj = MatrixInit(V, V, 0);
    return G;
}

// Procedimento que insere uma aresta entre dois vértices
void GraphInsertE(Graph G, Edge e) {
    int v = e.v, w = e.w;

    if(G->mAdj[v][w] == 0) G->E++;
    G->mAdj[v][w] = 1;
    G->mAdj[w][v] = 1;
}

// Procedimento que remove uma aresta entre dois vértices
void GraphRemoveE(Graph G, Edge e) {
    int v = e.v, w = e.w;
    if(G->mAdj[v][w] == 1) G->E--;
    G->mAdj[v][w] = 0;
    G->mAdj[w][v] = 0;
}

// Função que retorna a quantidade de aresta de um Grafo
int GraphEdges(Edge a[], Graph G) {
    int v, w, E = 0;
    for(v = 0; v < G->V; v++) {
        for(w = v+1; w < G->V; w++) {
            if(G->mAdj[v][w] == 1) {
                a[E++] = edge(v, w);
            }
        }
    }

    return E;
}

/*
      Aula 08/05
    >> Busca em Profundidade - DFS <<
    -- É um algoritmo de busca em grafos que explora o mais profundamente
    possível cada ramo antes de fazer o backtrack. 

    -- O algoritmo começa por um vértice e vai visitando os vizinhos, descendo
    o mais fundo que conseguir por um caminho, e só depois vollta para tentar
    outros caminhos.

    -- O algoritmo de DFS pode ser implementado de forma recursiva ou usando pilha.
    
    >> Busca em Largura - BFS <<

    -- É um algoritmo de exploração de grafos que visita os vértices por "camadas",
    indo mais "lateralmente" antes de ir fundo.
    
    -- Começamos por um vértice de origem, visitamos todos os vizinhos imediatos 
    (ou seja, a 1 passo de distância) e depois visitamos os vizinhos dos vizinhos.

    -- Usa uma fila para guardar os vértices a visitar na ordem certa.
*/

#define MAXV 1000

static int cnt, pre[MAXV];

// Implementação do DFS
void graphDFSSearch(Graph G) {
    int v;
    int conexos = 0;
    cnt = 0;

    for(v = 0; v < G->V; v++) {
        pre[v] = -1;
    }

    for(v = 0; v < G->V; v++) {
        if(pre[v] == -1) {
            dfsR(G, edge(v, v));
            conexos++;
        }
    }
}

void dfsR(Graph G, Edge e) {
    int t, w = e.w;
    pre[w] = cnt++;

    for(t = 0; t < G->V; t++) {
        if(G->mAdj[w][t] != 0) {
            if(pre[t] == -1) {
                dfsR(G, edge(w, t));
            }
        }
    }
}

// Implementação da Fila
#define MAX_SIZE 1000

struct queue
{
    int front;
    int back;
    int items[MAX_SIZE];
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
    return q->back == MAX_SIZE;
}

int QueueGet(Queue q) {
    return q->items[q->front++];
}

void QueuePut(Queue q, int v) {
    q->items[q->back++] = v;
}

// Implementação do BFS
void graphBFSSearch(Graph G) {
    cnt = 0;
    int v;

    for(v = 0; v < G->V; v++) {
        pre[v] = -1;
    }

    for(v = 0; v < G->V; v++) {
        if(pre[v] == -1) {
            bfs(G, edge(v, v));
        }
    }
}

void bfs(Graph G, Edge e) {
    Queue Q = malloc(sizeof(*Q));
    QueueInit(Q);

    int v;

    QueuePut(Q, e.v);
    pre[e.v] = cnt++;

    while (!QueueEmpty(Q)) {
        v = QueueGet(Q);

        for(int t = 0; t < G->V; t++) {
            if(G->mAdj[v][t] != 0) {
                if(pre[t] == -1) {
                    QueuePut(Q, t);
                    pre[t] = cnt++;
                }
            }
        }
    }

    free(Q);  
}



// Função Main gerada pelo ChatGPT pra testar as funções de implementação
int main() {
    // Cria um grafo com 4 vértices
    Graph G = GraphInit(4);

    // Insere algumas arestas
    GraphInsertE(G, edge(0, 1));
    GraphInsertE(G, edge(0, 2));
    GraphInsertE(G, edge(1, 3));
    GraphInsertE(G, edge(2, 3));

    printf("Arestas do grafo:\n");

    // Vetor para armazenar as arestas
    Edge edges[100];  // tamanho arbitrário, pode ajustar

    // Pega as arestas
    int total = GraphEdges(edges, G);

    // Imprime as arestas
    for (int i = 0; i < total; i++) {
        printf("(%d, %d)\n", edges[i].v, edges[i].w);
    }

    // Remove uma aresta
    GraphRemoveE(G, edge(0, 2));

    printf("\nArestas após remover (0,2):\n");

    total = GraphEdges(edges, G);

    for (int i = 0; i < total; i++) {
        printf("(%d, %d)\n", edges[i].v, edges[i].w);
    }

    return 0;
}

/*
    >> Alcançabilidade
    - Em um grafo dirigido, dizemos que um vértice t é alcançavel a partir de um
    vértice s se existir um caminho dirigido que leva de s até t. 
    
    >> Fecho transitivo
    - O fecho transitivo de um grafo dirigido G é um novo grafo dirigido G' que
    possui o mesmo conjunto de vértices que G. No entanto, em G', existirá uma
    aresta de um vértice s para um vértice t se, e somente se, houver um caminho
    dirigido de s para t no grafo original G. 
    - Essencialmente, o fecho transitivo mostra todas as possíveis alcançabilidades
    (diretas e indiretas) entre os pares de vértices. 

    >> Algoritmo de FLoyd-Warshall
    - É um algoritmo usado para encontrar o fecho transitivo de um grafo dirigido. 
    Ele determina para todos os pares de vértices (s, t) se existe um caminho de s
    para t. 
        > Funcionamento
        - Uma cópia da matriz de adj é criada e é definido para todo tc[s][s] = 1,
        indicando que um vértice é sempre alcançável a partir de si mesmo. 

        > Iteração
        - O algoritmo usa três laços aninhados para considerar todos os possíveis
        vértices intermediários (i), todos os possíveis vértices de origem (s) e 
        todos os possíveis vértices de destino (t). 
        - Para cada combinação ele verifica: 
            - Se existe um camminho de s para i E se existe um caminho de i para t. 
            Se sim, marca que existe um caminho de s para t. 
    
    - Segue o principio da transitividade matemática: 
        tc[s][i] == 1 AND tc[i][t] == 1 -> tc[s][t] == 1
*/

void GraphTC(Graph G) {
    int i, s, t;

    G->tc = MatrixInit(G->V, G->V, 0);
    for(s = 0; s < G->V; s++)
        for(t = 0; t < G->V; t++) 
            G->tc[s][t] = G->mAdj[s][t];

    for(s = 0; s < G->V; s++) G->tc[s][s] = 1;

    for(i = 0; i < G->V; i++) 
        for(s = 0; s < G->V; s++)
            if(G->tc[s][i] == 1)
                for(t = 0; t < G->V; t++)
                    if(G->tc[i][t] == 1) 
                        G->tc[s][t] = 1;

}

int GraphReach(Graph G, int s, int t) {
    return G->tc[s][t];
}

/*
    >> Ordenação Topológica com DFS
*/

void DAGts(Graph G, int *ts) {
    cnt = 0;
    for(int i = 0; i < G->V; i++) {
        pre[i] = -1, ts[i] = -1;
    }

    for(int i = 0; i < G->V; i++) {
        if(pre[i] == -1) TSdfs(G, i, ts);
    }
}

void TSdfs(Graph G, int v, int *ts) {
    pre[v] = 0;
    for(int w = 0; w < G->V; w++) {
        if(G->mAdj[w][v] != 0)
            if(pre[w] == -1) TSdfs(G, w, ts);
    }

    ts[cnt++] = v;
}