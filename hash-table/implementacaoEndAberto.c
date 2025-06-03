#include <stdio.h>
#include <stdlib.h>

typedef int Item;

#define less(A,B) (A < B)
#define eq(A,B) (A == B)
#define swap(A, B) {Item temp = A; A = B; B = temp;}
#define NULLItem (Item){-1}

#define hash(A, M) (A % M)
#define MAXCOL 10

Item *ht;
int M;

void STInit(int max) {
    ht = malloc(max * sizeof(Item));
    M = max;

    for(int i = 0; i < max; i++) {
        ht[i] = NULLItem;
    }
}

int HTInsert(Item new) {
    int h = hash(new, M);
    // printf("%d\n", h);
    int c = MAXCOL;

    while(c && !eq(ht[h], NULLItem)) {
        c--;
        h = (h+1) % M;
    }

    if(!c) return 0;
    ht[h] = new;
    return 1;
}

void STInsert(Item new) {
    if(!HTInsert(new)) {
        // HTGrow
    }
}

Item HTSearch(Item x) {
    int h = hash(x, M);
    int c = MAXCOL;

    while(c && !eq(x, ht[h])) {
        c--;
        h = (h+1) % M;
    }

    if(!c) return NULLItem;
    return ht[h];
}

void HTRemove(Item x) {
    int h = hash(x, M);
    int c = MAXCOL;

    while(c && !eq(x, ht[h]))
        c--, h = (h+1) % M;
    
    if(!c) return;
    ht[h] = NULLItem;
}

void HTGrow(int nMax) {
    Item *newHt = malloc(nMax * sizeof(Item));
    Item *oldHt = ht;
    ht = newHt;

    int oldM = M;
    M = nMax;

    for(int i = 0; i < M; i++) {
        ht[i] = NULLItem;
    }

    for(int i = 0; i < oldM; i++) {
        if(eq(oldHt[i], NULLItem)) continue;
        HTInsert(oldHt[i]);
    }

    free(oldHt);
}

int main() {
    STInit(10);
    int x = HTInsert(100);
    x = HTInsert(9);
    x = HTInsert(101);

    for(int i = 0; i < M; i++) {
        printf("%d ", ht[i]);
    }

    printf("\n");

    HTRemove(101);


    for(int i = 0; i < M; i++) {
        printf("%d ", ht[i]);
    }

    printf("\n");

    printf("%d\n", HTSearch(103));

    return 0;
}

