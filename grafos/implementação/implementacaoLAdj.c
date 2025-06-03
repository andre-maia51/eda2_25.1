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
    Lista de Adjacências
    > É uma lista de links onde cada índice representa um vértice, e a lista associada
    a esse índice contém os vértices adjacentes. 
*/

// Implementação
#include <stdio.h>
#include <stdlib.h>

// Estrutura de uma aresta
typedef struct {
    int v;
    int w;
} Edge;

// Estrutura da Lista Encadeada
typedef struct node *link;

struct node {
    int v;
    link next;
};

// Estrutura do Grafo
typedef struct graph *Graph;

struct graph {
    int V;
    int E;
    link *adj;
};

// Função que cria uma aresta entre dois vértices 
Edge edge(int v, int w) {
    Edge e;
    e.v = v;
    e.w = w;
    return e;
}

link NEW(int v, link next) {
    link x = malloc(sizeof(*x));
    if(x == NULL) tela_azul();
    x->v = v;
    x->next = next;
    return x;
}

void tela_azul() {
    fprintf(stderr, "Falha na alocação de memória.\n");
}

Graph GraphInit(int V) {
    int v;
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->adj = malloc(V * sizeof(link));

    for(v = 0; v < V; v++) {
        G->adj[v] = NULL;
    }

    return G;
}

void GraphInsert(Graph G, Edge e) {
    int v = e.v, w = e.w;
    G->adj[v] = NEW(w, G->adj[v]);
    G->adj[w] = NEW(v, G->adj[w]);
    G->E++; 
}

int GraphEdges(Edge a[], Graph G) {
    int v, E = 0;
    link t;

    for(v = 0; v < G->V; v++) {
        for(t = G->adj[v]; t != NULL; t = t->next) {
            if(v < t->v) a[E++] = edge(v, t->v);
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
void GraphSearch(Graph G) {
    int v;
    int conexos = 0;
    cnt = 0;

    for(v = 0; v < G->V; v++) {
        pre[v] = -1;
    }

    for(v = 0; v < G->V; v++) {
        if(pre[v] == -1) {
            dfsR(G, edge(v, v));
        }
    }
}

void dfsR(Graph G, Edge e) {
    int t, w = e.w;
    link l;
    pre[w] = cnt++;

    for(l = G->adj[w]; l != NULL; l = l->next) {
        t = l->v;
        if(pre[t] == -1) {
            dfsR(G, edge(w, t));
        }
    }
}

// Implementação da Fila
#define MAX_SIZE 1000

struct queue
{
    int front;
    int back;
    int *items;
    int size;
};

typedef struct queue *Queue;

Queue QueueInit(int V) {
    Queue Q = malloc(sizeof(*Q));
    Q->front = 0;
    Q->back = 0;
    Q->items = malloc(V * sizeof(int));
    Q->size = V;
    return Q;
}

int QueueEmpty(Queue q) {
    return q->front == q->back;
}

int QueueFull(Queue q) {
    return q->back == q->size;
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
    Queue Q = QueueInit(MAX_SIZE);

    int v, t;
    link l;

    QueuePut(Q, e.v);
    pre[e.v] = cnt++;

    while(!QueueEmpty(Q)) {
        v = QueueGet(Q);

        for(l = G->adj[v]; l != NULL; l = l->next) {
            t = l->v;
            if(pre[t] == -1) {
                QueuePut(Q, t);
                pre[t] = cnt++;
            }
        }
    }

    free(Q);
}

/*
    Aula 15/05
    >> Grafos Dirigidos <<
    - Um Grafo Dirigido (Digrafo) é um conjunto de arestas dirigidas que
    conectam um par de vértices (sem arestas duplicadas). Dizemos que uma
    aresta vai DE um primeiro vértice PARA o seu segundo vértice. 

    - Caminho Dirigido: 
        > O caminho dirigido em um digrafo é uma lista de vértices no qual
        existe uma aresta dirigida conectando cada vértice da lista a seu
        sucessor. 
        > Dizemos que um vértice T é alcançávl de um vértice S se
        existe um caminho dirigido de S a T. 
*/


// >> Grafo Reverso
Graph GraphReverse(Graph G) {
    int v;
    link t;

    Graph R = GraphInit(G->V);
    for(v = 0; v < G->V; v++) {
        for(t = G->adj[v]; t != NULL; t = t->next) {
            GraphInsert(R, edge(t->v, v));
        } 
    }

    return R;
}

/*
    >> Grafo Dirigido Acíclico (DAG - directed acyclic graph)
    - É um grafo dirigido que não possui ciclos.
    - Toda árvore é um DAG. 

    >> Grafo Dirigido Fortmente Conexo
    - Um grafo dirigido é fortemente conexo se todos os vértices
    são alcançáveis a partir de todos os vértices. 

    >> Alcançabilidade e Fecho Transitivo
    - Fecho Transitivo: 
        > O fecho transitivo de um grafo dirigido é um grafo diri-
        gido com o mesmo conjunto de vértices, mas com uma aresta
        de S a T no fecho transitivo se, e somente se, existe um 
        caminho dirigido de S a T no grafo dirigido. 
*/

/*
    Aula 22/05
    >> DAG e Ordenação Topológica
    - Podemos modelar problemas onde queremos realizar várias tare-
    fas, mas existem pendências.
    - Para realizar uma tarefa, precisamos primeiro realizar todas
    as tarefas que elas dependem - senso de pré-requisitos.

    > DAG Binário
        - Definição: Um DAG binário é um grafo dirigido acíclico com
        duas arestas saindo de cada nó, identificados como arestas da
        esquerda e da direita, uma delas, ou ambas podem ser NULL. 
        - A distinção entre DAG binário e árvore binária é que o DAG
        binário pode ter mais de um link apontando para um nó. DAGs bi-
        nários fornecem uma maneira compacta de representar árvores bi-
        nárias em algumas aplicações.
    
    > Ordenação Topológica
        - O objetivo é processar os vértices de um DAG de tal forma que
        cada vértice é processado antes de todos os vértices para o qual
        aponta. 
    
        > Ordenação Topológica (Renomear)
            - Dado um DAG, renomeie os vértices de forma que toda aresta di-
            rigida aponta de um vértice de número menor para um número maior.
        
        > Ordenação Topológica (Reorganizar)
            - Dado um DAG, reorganize os vértices em uma linha horizontal de
            forma que as arestas dirigidas apontem da esquerda para a direita.

        > Reorganização <-> Renomeação
            - Reorganização -> Renomeação
            for(i = 0; i < V; i++) tsI[ts[i]] = i;
            
            - Renomeação -> Reorganização
            for(i = 0; i < V; i++) ts[tsI[i]] = i;
    
        - Não existe somente uma ordenação topológica. Em problemas de realiza-
        ção de tarefas isto fica muito claro, especialmente quando uma tarefa não
        possui dependência direta, ou indireta, de outra tarefa e, então, podem
        ser realizadas em qualquer ordem, ou até mesmo em paralelo. 

        - Por exemplo, os vértices podem representar os termos definidos em um
        livro, com uma aresta de s a t se a definição s usa a t. Neste caso, se-
        ria útil encontrar uma ordenação com a propriedade que cada termo é defi-
        nido antes de ser usado por outra definição. Usando esta ordenação corres-
        ponde a posicionar os vértices em uma linha tal que as arestas vão da dir
        para esquerda - ordenação topológica reversa. 

        - Para fazer a ordenação topológica reversa usamos DFS. Quando a entrada
        é um DAG, a numeração pos order coloca os vértices em ordenação topológica
        reversa. Ou seja, numeramos cada vértice como a ação final de função recur-
        siva do DFS.
*/

void DAGtsReverse(Graph G, int *ts) {
    cnt = 0;

    for(int v = 0; v < G->V; v++) {
        ts[v] = -1, pre[v] = -1;
    }

    for(int v = 0; v < G->V; v++) {
        if(pre[v] == -1) DAGdfsReverse(G, v, ts);
    }
}

void DAGdfsReverse(Graph G, int v, int *ts) {
    pre[v] = 0;
    for(link l = G->adj[v]; l != NULL; l = l->next) {
        if(pre[l->v] == -1) DAGdfsReverse(G, l->v, ts);
    }
    ts[cnt++] = v;
}

/*
    Como produzir a ordenação topológica?
    > Forma 1: Gerar TS com DFS -> Ta na implementação de matriz de adjacência

    > Forma 2: 
        - Propriedade: Todo DAG possui pelo menos 1 source e pelo menos 1 sink.

        - Usando esta propriedade podemos fazer uma TS usando algo parecido com
        uma BFS:
            - Mantendo um vetor vértice-indexado que mantém a contagem de entrada
            em cada vértice.
                - Vértices com grau de entrada 0 são sources, então inicializamos
                uma fila em uma passada no DAG então realizamos as seguintes opera-
                ções até que a fila fique vazia:
                    1. Remove uma source da fila e a rotule.
                    2. Decremente o grau de entra dos vértices de destino.
                    3. Se decrementar causa o grau de entrada do vértice X ficar
                    em 0, insira este vértice na fila. 
*/

static int in[MAXV];

void DAGts(Graph G, int *ts) {
    for(int v = 0; v < G->V; v++) {
        in[v] = 0, ts[v] = -1;
    }

    for(int v = 0; v < G->V; v++) {
        for(link l = G->adj[v]; l != NULL; l = l->next)
            in[l->v]++;
    }

    Queue Q = QueueInit(G->V);

    for(int i = 0; i < G->V; i++) {
        if(in[i] == 0) QueuePut(Q, i);
    }

    int v;
    for(int i = 0; !QueueEmpty(Q); i++) {
        ts[i] = (v = QueueGet(Q));
        for(link l = G->adj[v]; l != NULL; l = l->next) {
            if(--in[l->v] == 0) QueuePut(Q, l->v);
        }
    }
}