#include <stdio.h>
#include <stdlib.h>

typedef int Item;

#define less(A,B) (A < B)
#define eq(A,B) (A == B)
#define swap(A, B) {Item temp = A; A = B; B = temp;}
#define NULLItem (Item){-1}

#define hash(A, M) (A % M)
#define doubleHash(A) (A % 97 + 1)
#define MAXCOL 10

Item *ht;
int M;

void STInit(int max) {
    ht = malloc(max * sizeof(Item));
    M = max;

    for(int i = 0; i < M; i++) {
        ht[i] = NULLItem;
    }
}

int HTInsert(Item new) {
    int h = hash(new, M);
    int h2 = doubleHash(new);
    int hn = -1;

    for(int c = MAXCOL; c > 0; c--) {
        if(eq(ht[h], NULLItem)) {
            hn = h;
        } else if(eq(ht[h], new)) {
            ht[h] = new;
            return 1;
        }

        h = (h + h2) % M;
    }

    if(hn == -1) return 0;
    ht[hn] = new;
    return 1;
}

void STInsert(Item new) {
    if(!HTInsert(new)) {
        // HTGrow()
    }
}

Item HTSearch(Item x) {
    int h = hash(x, M);
    int h2 = doubleHash(x);
    int c = MAXCOL;

    while(c && !eq(ht[h], x)) {
        c--, h = (h+h2) % M;
    }

    if(!c) return NULLItem;
    return ht[h];
}

void HTRemove(Item x) {
    int h = hash(x, M);
    int h2 = doubleHash(x);
    int c = MAXCOL;

    while(c && !eq(ht[h], x)) {
        c--, h= (h+h2) % M;
    }

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
