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
    
    > Representação em Lista de Adjacências:
        - Por que o peso fica na estrutura do 'Node'? Porque na lista de
        adjacências, cada 'Node' da lista encadeada representa a própria
        ARESTA (a conexão). O array 'adj[i]' aponta para o vértice 'i', e
        cada 'Node' em sua lista representa uma aresta que *sai* de 'i' para
        um vizinho. Portanto, o peso, que é uma propriedade da aresta, é
        armazenado logicamente junto da informação do vizinho.

        - Exemplo Curto: Uma aresta de A(0) para B(1) com peso 7 seria
        representada assim na lista de adjacência do vértice 0:

        adj[0] -> [ v: 1 | weight: 7 ] -> NULL
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

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
        pa[i] = -1, mature[i] = false, dist[i] = INT_MAX;
    }

    pa[s] = s, dist[s] = 0;

    while(true) {
        int min = INT_MAX;
        int y; 

        for(int z = 0; z < G->V; z++) {
            if(mature[z]) continue;

            if(dist[z] < min) min = dist[z], y = z;
        }

        if(min == INT_MAX) break;

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

    >> COMO FUNCIONA A HEAP INDEXADA PARA O DIJKSTRA <<

    A Fila de Prioridade (PQ) implementada aqui é a ferramenta que torna o
    Dijkstra eficiente. Em vez de procurar o vértice de menor distância em
    um array toda vez (custo O(V)), a heap nos dá essa informação em
    tempo O(log V). Ela possui três componentes principais:

    1. O HEAP (array 'pq'): É um array que representa uma árvore binária
    de forma compacta. Ele é ordenado de uma maneira especial (propriedade
    do heap) onde o elemento na primeira posição, 'pq[1]', é sempre o
    vértice com a menor distância (a maior prioridade).

    2. O MAPEAMENTO INVERSO (array 'qp'): Este é o "pulo do gato". Ele
    funciona como um índice que nos permite encontrar a posição de QUALQUER
    vértice dentro do heap instantaneamente. 'qp[vertice]' nos retorna
    a posição daquele vértice no array 'pq'. Sem isso, teríamos que
    percorrer o heap para achar um vértice, o que seria lento e anularia
    a vantagem de usá-lo.

    3. AS OPERAÇÕES:
       - fixUp (subir): Quando a prioridade de um item aumenta (sua
         distância diminui), ele pode precisar "subir" na árvore do heap
         para manter a ordem. Essa função cuida disso.
       - fixDown (afundar): Quando um item no topo é substituído por um
         de menor prioridade (maior distância), ele precisa "afundar" na
         árvore até sua posição correta.
       - PQinsert: Adiciona um novo vértice ao heap.
       - PQdelmin: Remove e retorna o vértice de maior prioridade (o do
         topo, com menor distância).
       - PQchange: Usa o 'qp' para encontrar um vértice rapidamente e
         atualiza sua prioridade, chamando 'fixUp' para reordenar.
*/

#define Weight int
#define Vertex int

typedef struct {
    Weight weight; /// armazena o peso
    Vertex vertex; // armazena o vértice
} Item;

#define getWeight(A) (A.weight)
#define less(A, B) (getWeight(A) < getWeight(B))
#define eq(A, B) (getWeight(A) == getWeight(B))
#define swap(A, B) {Item temp = A; A = B; B = temp;}

// Implementação da Heap
typedef struct pq *PQ;
struct pq {
    int size;
    Item *pq;
    int *qp;
};

void PQswap(PQ pq, int i, int j) {
    swap(pq->pq[i], pq->pq[j]);
    pq->qp[pq->pq[i].vertex] = i;
    pq->qp[pq->pq[j].vertex] = j;
}

void fixUp(PQ pq, int k) {
    while(k > 1 && less(pq->pq[k], pq->pq[k/2])) {
        PQswap(pq, k, k/2);
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
    pq->qp[item.vertex] = pq->size;
    pq->pq[pq->size] = item;
    fixUp(pq, pq->size);
}

Item PQdelmin(PQ pq) {
    Item item = pq->pq[1];
    PQswap(pq, 1, pq->size--);
    fixDown(pq, 1);
    pq->qp[item.vertex] = -1;
    return item;
}

void PQchange(PQ pq, Item item) {
    int pos = pq->qp[item.vertex];
    pq->pq[pos].weight = item.weight;
    fixUp(pq, pos);
    fixDown(pq, pos);
}

// Implementação do Dijkstra com Heap
void GRAPHdijkstraHeap(Graph G, int s, int *pa, int *dist) {
    for(int i = 0; i < G->V; i++) {
        dist[i] = INT_MAX;
        pa[i] = -1;
    }

    dist[s] = 0, pa[s] = s;

    PQ pq = PQinit(G->V);
    PQinsert(pq, (Item){.weight = 0, .vertex = s});

    while(!PQempty(pq)) {
        Item item = PQdelmin(pq);
        int v = item.vertex;

        if(item.weight > dist[v]) continue;

        for(Node l = G->adj[v]; l != NULL; l = l->next) {
            if(dist[v] != INT_MAX && dist[v] + l->weight < dist[l->v]) {
                dist[l->v] = dist[v] + l->weight;
                pa[l->v] = v;

                Item i = {.weight = dist[l->v], .vertex = l->v};
                if(PQcontains(pq, l->v)) PQchange(pq, i);
                else PQinsert(pq, i);
            }
        }
    }

    free(pq->pq), free(pq->qp), free(pq);
}

/*
    Aula 10/06
    >> Bellman Ford <<

    Assim como o Dijkstra, o Bellman-Ford também resolve o problema do caminho
    mínimo de origem única.

        > Principais Vantagens: Sua grande força é funcionar com arestas de peso
        negativo e ser capaz de detectar ciclos de peso negativo.

        > Funcionamento: A abordagem é baseada em relaxamento sucessivo. Ele repete
        um laço V-1 vezes e, em cada laço, relaxa TODAS as E arestas do grafo.
        Por que V-1 vezes? Porque o maior caminho simples (sem ciclos) em um grafo
        com V vértices tem, no máximo, V-1 arestas. Com V-1 passadas, o algoritmo
        garante que a distância correta se "propague" da origem até o mais distante.

        > Detecção de Ciclo Negativo: Após as V-1 passadas, o algoritmo faz uma
        V-ésima verificação. Se alguma distância ainda puder ser diminuída, significa
        que o caminho mais "curto" teria V ou mais arestas, o que só é possível se
        ele passar por um ciclo. Se esse ciclo diminui o custo total, ele é um
        ciclo de peso negativo.

        > Otimização com Fila e Sentinela: A implementação que você estudou usa
        uma fila para processar apenas vértices cujas distâncias foram recentemente
        atualizadas. O valor 'V' é usado como um sentinela na fila para contar
        quantas "rodadas" de relaxamento ocorreram, permitindo a detecção de
        ciclos negativos de forma eficiente.
*/

// Implementação da Fila
typedef struct queue *Queue;
struct queue {
    int front, back;
    int size;
    int *items;
};

Queue QUEUEinit(int V) {
    Queue Q = malloc(sizeof(*Q));
    Q->front = 0;
    Q->back = 0;
    Q->size = V;
    Q->items = malloc(V * sizeof(int));
    return Q;
}

int QUEUEfull(Queue Q) {
    return Q->back == Q->size;
}

int QUEUEempty(Queue Q) {
    return Q->front == Q->back;
}

int QUEUEget(Queue Q) {
    return Q->items[Q->front++];
}

void QUEUEput(Queue Q, int V) {
    Q->items[Q->back++] = V;
}

// Implementação do Bellman-Ford
bool GRAPHsearchBF(Graph G, int s, int *pa, int *dist) {
    // é um array vértice-indexado que verifica se o vértice ta na fila
    bool onqueue[1000];

    for(int i = 0; i < G->V; i++) {
        pa[i] = -1, dist[i] = INT_MAX, onqueue[i] = false;
    }

    pa[s] = s, dist[s] = 0;

    Queue Q = QUEUEinit(G->V);
    QUEUEput(Q, s);
    onqueue[s] = true;

    int V = G->V, k = 0;
    QUEUEput(Q, V);

    while(true) {
        int x = QUEUEget(Q);
        if(x < V) {
            for(Node l = G->adj[x]; l != NULL; l = l->next) {
                if(dist[x] + l->weight < dist[l->v]) {
                    dist[l->v] = dist[x] + l->weight;
                    pa[l->v] = x;

                    if(onqueue[l->v] == false) {
                        QUEUEput(Q, l->v);
                        onqueue[l->v] = true;
                    }
                }
            }
        } else {
            if(QUEUEempty(Q)) return true;
            if(++k >= G->V) return false;
            QUEUEput(Q, V);
            for(int i = 0; i < G->V; i++) onqueue[i] = false;
        }
    }
}

/*
    >> Comparativo
        > Complexidades: 
            - Dijkstra com Heap: O(E*log(V))
            - Bellman-Ford: O(V*E)

        > Pesos Negativos:
            - Dijkstra com Heap: Não suporta
            - Bellman-Ford: Suporta
        
        > Ciclos Negativos: 
            - Dijkstra com Heap: Não detecta
            - Bellman-Ford: Detecta
        
        > Abordagem:
            - Dijkstra com Heap: Gulosa (Greedy)
            - Bellman-Ford: Programação Dinâmica
*/

/*
    - Aula 12/06
    >> ÁRVORES GERADORAS <<

    - Uma árvore geradora de um grafo concectado é um subconjunto de arestas que conecta
    todos os vértices sem formar ciclos. 
    - Imagine que o grafo é uma cidade com vários pontos (vértices) e várias ruas (arestas)
    conectando-os. Uma árvore geradora seria o conjunto mínimo de ruas que você precisa manter
    abertas para que seja possível ir de qualquer ponto a qualquer outro, sem que haja um caminho
    circular (um ciclo).
    - Para um grafo com V vértices, uma árvore geradora sempre terá V-1 arestas.

    > Conceitos Principais
        > Propriedade dos Circuitos: 
            - Ao pegar uma árvore geradora e adicionar uma aresta que não
            pertencia a ela, inevitavelmente, criaremos um ciclo. Para restaurar a árvore, podemos
            remover qualquer aresta desse ciclo.

        > Propriedade dos Cortes: 
            - Ao remover uma aresta de uma árvore geradora, ela se dividirá
            em duas partes (dois conjuntos de vértices). Um corte é o conjunto de todas as arestas
            que conectam essas duas partes. Para reconectar a árvore, podemos adicionar qualquer aresta
            do corte.

    >> ÁRVORES GERADORAS DE CUSTO MÍNIMO (MST) <<

    - Uma árvore geradora mínima de G, um grafo não dirigido com custos nas arestas, é qualquer árvore
    geradora de G que tenha custo mínimo. Em outras palavras, uma árvore geradora T de G é mínima se 
    nenhuma outra árvore geradora tem custo menor que o de T (minimum spanning tree - MST). 
    - Problema da MST: Dado um grafo não dirigido com custos nas arestas, encontrar uma MST do grafo.
    - É claro que esse problema tem solução se, e somente se, o grafo é conexo. Outra observação óbvia
    é que se todas as arestas tiverem o mesmo custo, então toda árvore geradora é uma MST.

    > Critério de minimalidade baseado em circuitos e em cortes
        - Dada uma MST de um grafo, não é necessariamente verdade que todas as arestas baratas estão na
        MST e todas as caras estão fora. 
        - A propriedade dos circuitos e dos cortes levam a prova dessa afirmação.

    - Os algoritmos que resolvem o problema da MST possuem caráter guloso, ou seja, a cada passo, eles
    fazem uma escolha que parece a melhor naquele momento, sem se preocupar com as consequências futu-
    ras. Para o problema da MST, essa estratégias funciona e leva a solução ótima.

    - Os dois principais algoritmos que usam a estratégia gulosa para encontrar a MST são:
        1. Algoritmo de Prim -> que faz crescer uma árvore até que ela se torne geradora;
        2. Algoritmo de Kruskal -> que faz crescer uma floresta geradora até que ela se torne
        uma árvore.

    - Eles não consomem mais que O(E*logV).
*/

/*
    >> ALGORITMO DE PRIM <<
    
    - O algoritmo constrói a árvore aresta por aresta, começando por um vértice arbitrário e, a cada
    passo, adicionando a aresta de menor custo que conecta um vértice que já está na árvore a um vér-
    tice que ainda não está.
    
    > Funcionamento
        - O algoritmo é guloso e segue o seguinte passo a passo:
            1. Começa com um vértice qualquer, que forma a árvore inicial.
            2. A cada passo, o algoritmo olha pra a franja e escolhe a aresta de menor custo da franja.
            3. Adiciona essa aresta e o novo vértice à árvore.
            4. Repete o processo até que todos os vértices estejam na árvore.
    
    > Franja
        - É a fronteira da árvore em construção. Representa todas as possíveis expansões da árvore em
        um determinado momento.
        - É o conjunto de todas as arestas que conectam um vértice que já pertence à árvore a um vér-
        tice que ainda não pertence a ela.
    
    - Para garantir eficiencia, o algoritmo de Prim utiliza de uma fila de prioridades para escolher
    o menor vértice da franja a cada passo, semelhante a estratégia do algoritmo de Dijkstra.
*/

// Implementação do Algoritmo de Prim com Heap
void MSTprimHeap(Graph G, int s, int *pa) {
    bool tree[1000];
    int price[1000];

    for(int i = 0; i < G->V; i++) {
        pa[i] = -2, tree[i] = false, price[i] = INT_MAX;
    }

    pa[s] = s, tree[s] = true, price[s] = 0;
    
    for(Node l = G->adj[s]; l != NULL; l = l->next) {
        pa[l->v] = s, price[l->v] = l->weight;
    }

    PQ pq = PQinit(G->V);
    for(int i = 0; i < G->V; i++) {
        PQinsert(pq, (Item){.vertex = i, .weight = price[i]});
    }

    while(!PQempty(pq)) {
        int vertex = PQdelmin(pq).vertex;
        if(price[vertex] == INT_MAX) break;

        tree[vertex] = true;

        for(Node l = G->adj[vertex]; l != NULL; l = l->next) {
            if(!tree[l->v] && l->weight < price[l->v]) {
                price[l->v] = l->weight;
                PQchange(pq, (Item){.vertex = l->v, .weight = price[l->v]});
                pa[l->v] = vertex;
            }
        }
    }
}
