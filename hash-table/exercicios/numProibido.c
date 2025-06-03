// #include <stdio.h>
// #include <stdlib.h>

// #define M 140009  // Tamanho da tabela hash (primo)
// #define NULL_KEY -1

// typedef struct Node {
//     int key;
//     struct Node *next;
// } Node;

// typedef struct {
//     Node *head;
// } HashList;

// HashList *HT;

// void HTInit() {
//     HT = malloc(M * sizeof(HashList));
//     for (int i = 0; i < M; i++) {
//         HT[i].head = NULL;
//     }
// }

// int hash(int k) {
//     return k % M;
// }

// void HTInsert(int key) {
//     int h = hash(key);
//     Node *new = malloc(sizeof(Node));
//     new->key = key;
//     new->next = HT[h].head;
//     HT[h].head = new;
// }

// int HTSearch(int key) {
//     int h = hash(key);
//     Node *curr = HT[h].head;
//     while (curr) {
//         if (curr->key == key) return 1;
//         curr = curr->next;
//     }
//     return 0;
// }

// int main() {
//     HTInit();

//     int N;
//     scanf("%d", &N);

//     for (int i = 0; i < N; i++) {
//         int key;
//         scanf("%d", &key);
//         HTInsert(key);
//     }

//     int query;
//     while (scanf("%d", &query) == 1) {
//         if (HTSearch(query)) printf("sim\n");
//         else printf("nao\n");
//     }

//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>

#define Key(A) (A.key)
#define less(A,B) (Key(A) < Key(B))
#define eq(A,B) (Key(A) == Key(B))
#define swap(A, B) {Item temp = A; A = B; B = temp;}
#define NULLItem (Item){-1}

#define hash(A, M) (A % M)
#define doubleHash(A, M) (A % 97 + 1)
#define MAXCOL 10

typedef struct {
    int key;
} Item;

int M = 140009;
Item *HT;

void HTInit() {
    HT = malloc(M * sizeof(Item));
    for(int i = 0; i < M; i++) {
        HT[i].key = NULLItem.key;
    }
}

int HTInsert(Item new) {
    int h = hash(Key(new), M);
    int h2 = doubleHash(Key(new), M);
    int hn = -1;

    for(int c = MAXCOL; c > 0; c--) {
        if(eq(HT[h], NULLItem)) {
            hn = h;
        } else if(eq(HT[h], new)) {
            HT[h] = new;
            return 1;
        }

        h = (h+h2) % M;
    }

    if(hn == -1) return 0;
    HT[hn] = new;
    return 1;
}

int HTSearch(Item x) {
    int h = hash(Key(x), M);
    int h2 = doubleHash(Key(x), M);
    int c = MAXCOL;

    while(c && !eq(HT[h], x))
        c--, h = (h+h2) % M;
    
    if(!c) return 0;
    return 1;
}

int main() {
    HTInit();

    int N;
    scanf("%d", &N);

    for(int i = 0; i < N; i++) {
        Item j;
        scanf("%d", &j.key);
        if(HTInsert(j) == 0) fprintf(stderr, "Falha ao inserir o numero %d\n", j.key);
    }

    Item num;
    while(scanf("%d", &num.key) == 1) {
        if(!HTSearch(num)) printf("nao\n");
        else printf("sim\n");
    }

    return 0;
}