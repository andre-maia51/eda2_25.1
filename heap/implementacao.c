/*
    >> Fila de Prioridade <<
    - Filas de pioridade são ADT que manipulam conjuntos de itens. Um item k é máximo
    se nenhum item é estritamente maior que k e mínimo se nenhum item é estritamente
    menor que k. Podemos ter mais de um item máximo e mais de um item mínimo. 

    > Fila de prioridade DECRESCENTE - PQ máxima
    - Uma fila de prioridade DECRESCENTE é uma ADT que manipula um conjunto de itens
    por meio de duas operações fundamentais: 
        - Inserção; 
        - Remoção. 
    
    > Fila de prioridade CRESCENTE - PQ mínima
    - Uma fila de prioridade CRESCENTE é uma ADT que funciona de maneira análoga a
    PQ máxima.

    > Implementação da Fila de Prioridades
    - Listas Encadeadas: 
        - Se a inserção for ordenada, o custo é proporcional a O(N), mas a remoção
        é rápida, O(1). 
        - Se a inserção não for ordenada, é barato O(1), mas a remoção é O(N). 
    
    - Vetores: Seguem o mesmo caso das listas encadeadas. 

    - A melhor opção é a Heap, pois todas as operações são log(N). 

    >> Heap <<
    - É um tipo de árvore binária para implementar fila de prioridade eficientemente. 
    - Existem dois tipos de Heaps: 
        - Decrescente - máximo fica no topo (raíz).
        - Crescente - mínimo fica no topo (raíz).

    - Heap decrescente: O item de qualquer nó é menor ou igual que o item do pai
    - Heap crescente: O item de qualquer nó é maior ou igual que o item do pai

    - Representação de Heap: 
        - Utilizamos um vetor v[1...N] (o índice 0 não é utilizado). 
        - Os dois filhos de um nó no índice k são, respectivamente, 2*k e 2*k+1. 
        - Reciprocamente, o pai de qualquer índice k é k/2. 
    
    >> Fila de Prioridade Baseada em Índice <<
    - É uma variação que permite modificar a prioridade de um item que já está na
    fila de forma eficiente. Ela usa dois vetores adicionais para rastrear as posi-
    ções dos itens: 
        - pq: Armazena os índices dos itens na heap. 
        - qp: Mapeia o índice de um item à sua posição na heap - basicamente funciona
        como uma tabela hash. 
    
    - Para implementar essa fila de prioridades, necessitamos de algumas alterações: 
        - Adicionar o vetor qp na struct da PQ.
        - Realizar a função PQswap, que realiza a troca necessária.
*/

#include <stdio.h>
#include <stdlib.h>

// Implementação das Macros e estruturas básicas
#define key int
#define data int

typedef struct {
    key k;
    data d;
} Item;

#define getKey(A) (A.k)
#define less(A, B) (getKey(A) < getKey(B))
#define eq(A, B) (getKey(A) == getKey(B))
#define swap(A, B) {Item temp = A; A = B; B = temp;}

typedef struct pq *PQ;
struct pq {
    Item *pq;
    int *qp; // mapeia o ID de item (data) à sua posição na heap
    int N;
};

//  Função de troca da Heap
void PQswap(PQ PQ, int i, int j) {
    swap(PQ->pq[i], PQ->pq[j]);
    PQ->qp[PQ->pq[i].d] = i;
    PQ->qp[PQ->pq[j].d] = j;
}

// Funções de correção da Heap
void fixUp(PQ PQ, int k) {
    while(k > 1 && less(PQ->pq[k], PQ->pq[k/2])) {
        PQswap(PQ, k, k/2);
        k = k/2;
    }
}

void fixDown(PQ PQ, int k) {
    int j;

    while(2*k <= PQ->N) {
        j = 2*k;

        if(j < PQ->N && less(PQ->pq[j+1], PQ->pq[j])) j++;
        if(less(PQ->pq[k], PQ->pq[j])) break;
        PQswap(PQ, k, j);
        k = j;
    }
}

// Implementação da Heap Crescente - Menor elemento na raíz
void PQinit(PQ PQ, int max) {
    PQ->pq = malloc(sizeof(Item) * (max + 1));
    PQ->qp = malloc(sizeof(int) * (max + 1));
    PQ->N = 0;

    for(int i = 0; i <= max; i++) PQ->qp[i] = -1;
}

int PQempty(PQ PQ) {
    return PQ->N == 0;
}

void PQinsert(PQ PQ, Item n) {
    PQ->N++;
    PQ->pq[PQ->N] = n;
    PQ->qp[n.d] = PQ->N;
    fixUp(PQ, PQ->N);
}

Item PQdelMin(PQ PQ) {
    Item item = PQ->pq[1];
    PQswap(PQ, 1, PQ->N);
    PQ->N--;
    fixDown(PQ, 1);
    PQ->qp[item.d] = -1;
    return item;
}

// Permite alterar a prioridade de um item usando seu ID (data). 
void PQchange(PQ PQ, Item item, key new) {
    int pos = PQ->qp[item.d];

    if(pos == -1) return;

    PQ->pq[pos].k = new;
    
    fixUp(PQ, pos);
    fixDown(PQ, pos);
}