// ESSA PARTE NÃO FAZ PARTE DAS RESPOSTAS DA PROVA
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v;
    int w;
} Edge;

typedef struct node *link;
struct node {
    int v;
    link next;
};

typedef struct graph *Graph;
struct graph {
    int V;
    int E;
    link *adj;
};

Edge edge(int v, int w) {
    Edge e;
    e.v = v;
    e.w = w;
    return e;
}

link NEW(int v, link next) {
    link x = malloc(sizeof(*x));
    if(x == NULL) fprintf(stderr, "Falha na alocação de memória.\n");
    x->v = v;
    x->next = next;
    return x;
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

/*
    Q.01-A) A estrutura a ser usada para armazenar as sementes pode ser um
    grafo direcionado, ou seja, uma semente f que é gerada por p não pode 
    gerar p. A estrutura do grafo pode utilizar de matriz ou listas de adja-
    cência, isso depende se o grafo é esparso ou não. Para esse exercício demons-
    traremos com uma matriz de adjacência
*/
// Implementação -> Q.01-A
struct graph {
    int V, E;
    int **adj // Matriz de Adjacência
};

/*
    Q.01-B) Para essa implementação, podemos percorrer a lista de adjacência
    onde verificamos as arestas de cada vértice, se um vértice p está na lista
    de adjacências do vértica q, significa que q é gerador de p, caso seja, mar-
    camos em um array auxiliar que o vértice p temPai. Após isso printamos todos
    os vértices do grafo que não possui valor 1 no array auxiliar, ou seja, ele 
    é primordial. Essa implementação possui custo linear em relação aos vértices
    e arestas O(V+E).
*/
void printPrimordiais(Graph G) {
    int temPai[G->V+1];
    for(int i = 0; i < G->V; i++) temPai[i] = 0;

    for(int i = 0; i < G->V; i++) {
        for(link l = G->adj[i]; l != NULL; l = l->next) {
            temPai[l->v] = 1;
        }
    }

    for(int i = 0; i < G->V; i++) 
        if(!temPai[i]) printf("%d\n", i);
}

/*
    Q.01-C) A implementação dessa questão segue o mesmo raciocínio da questão
    anterior. A diferença está na verificação final na hora da impressão das
    sementes, devemos verificar se a semente possui pai e se ela não é gerado-
    ra de nenhuma outra semente, caso seja verdade para as duas condições, ela
    é final. O custo dessa implementação, assim como na questão anterior, é li-
    near em relação a quantidade de vértices e arestas O(V+E).
*/

void printFinais(Graph G) {
    int temPai[G->V+1];
    for(int i = 0; i < G->V; i++) temPai[0];

    for(int i = 0; i < G->V; i++) {
        for(link l = G->adj[i]; l != NULL; l = l->next){
            temPai[l->v] = 1;
        }
    }

    for(int i = 0; i < G->V; i++)
        if(temPai[i] && G->adj[i] == NULL) printf("%d\n", i);
}

/*
    Q.01-D) 
*/