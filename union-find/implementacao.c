/*
    >> ESTRUTURA UNION-FIND <<

    - A estrutura de dados Union-Find foi projetada para gerenciar uma coleção de con-
    juntos disjuntos - que não possuem elementos em comum. Ela permite determinar com
    eficiência a qual conjunto um elemento pertence e uní-los em seguida.

    - O conceito é introduzido no contexto de um algoritmo para calcular o número de 
    componentes de um grupo. Esse ADT é usado por implementações eficientes de vários
    algoritmos interessantes, como o de Kruskal (para o problema da MST). 

    - Esse ADT dispões de duas operações:
        > FIND: Recebe um vértice v e devolve o diretor da componente a que v pertence.
        > UNION: recebe os diretores, r e s, de duas componentes distintas, faz a fusão
        das duas componentes, e designa r ou s como diretor da nova componente.
    
    - A implementação das operações FIND e UNION pode precisar de alguma estrutura de
    dados auxiliar, que é preparada e inicializada pela operação INITIALIZE.

    1. Implementação Ingênua
        - Ideia: Um vetor id[] armazena o representante de cada elemento.
        - Find: Rápido, basta acessar id[i].
        - Union: Lento. Para unir o conjunto p e q, é preciso percorrer todo o vetor e 
        atualizar o representante de todos os membros de um dos conjuntos.
    
    2. Implementação com Árvores
        - Ideia: Os conjuntos são representados como árvores. Cada elemento aponta para
        seu pai. O representante do conjunto é a raíz da árvore.
        - Find: Segue a cadeia de pais até encontrar a raíz. No pior caso, a árvore po-
        de ser uma tripa, tornando a busca linear.
        - Union: Rápido. Apenas faz a raíz de uma árvore apontar para a raíz da outra.
    
    3. Otimização - Union-By-Rank
        - Ideia: Para evitar que as árvores fiquem muito altas (o que torna o Find len-
        to), a árvore menor é sempre anexada à raíz da árvore maior durante a operação
        Union. Isso mantém as árvores mais achatadas.
        - Isso garante que a altura de qualquer árvore seja no maximo logN.
    
    4. Otimização - Path Compression
        - Ideia: Durante a operação Find, após encontrar a raíz, fazemos com que todos os
        nós no caminho percorrido apontem diretamente para a raíz.
        - Isso achata a árvore drasticamente durante o uso, tornando as buscas futuras
        muito mais rápidas.
    
    - A combinação de Union-By-Rank e Path Compression resulta na implementação mais efi-
    ciente, com um tempo amortizado por operação.
*/

// Implementação
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct unionFind *UF;
struct unionFind {
    int *parent; // armazena o pai de cada elemento
    int *rank; // armazena a altura de cada conjunto
};

UF UFinit(int V) {
    UF uf = malloc(sizeof(*uf));

    uf->parent = malloc(V * sizeof(int));
    uf->rank = malloc(V * sizeof(int));

    for(int i = 0; i < V; i++) {
        // cada elemento começa como seu próprio pai (conj. individual)
        uf->parent[i] = i;

        // a altura vai ser 0
        uf->rank[i] = 0;
    }

    return uf;
}

int UFfind(UF uf, int i) {
    /* 
        se i não é a raíz da sua árvore, sobre recursivamente e faz todos os
        nós no caminho apontarem diretamente para a raíz
     */

    if(uf->parent[i] != i) {
        uf->parent[i] = UFfind(uf, uf->parent[i]);
    }

    return uf->parent[i];
}

void UFunion(UF uf, int p, int q) {
    int rootP = UFfind(uf, p), rootQ = UFfind(uf, q);
    if(rootP == rootQ) return;

    if(uf->rank[rootP] > uf->rank[rootQ]) {
        uf->parent[rootQ] = rootP;
    } else if(uf->rank[rootP] < uf->rank[rootQ]) {
        uf->parent[rootP] = rootQ;
    } else {
        uf->parent[rootQ] = rootP;
        uf->rank[rootP]++;
    }
}

bool UFconnected(UF uf, int p, int q) {
    return UFfind(uf, p) == UFfind(uf, q);
}

void UFfree(UF uf) {
    free(uf->parent), free(uf->rank), free(uf);
}