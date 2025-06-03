#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int color;
    int count;
} Item;

#define getColor(A) (A.color)
#define less(A,B) (A < B)
#define eq(A,B) (A == B)
#define swap(A, B) {Item temp = A; A = B; B = temp;}
#define NULLItem (Item){.color = -1, .count = 0}

#define hash(A, M) (A % M)
#define MAXCOL 10
#define HTSIZE 5009

Item *HT;
int M = HTSIZE;

void HTInit() {
    HT = malloc(M * sizeof(Item));
    for(int i = 0; i < M; i++)
        HT[i] = NULLItem;

}

int HTInsert(int color) {
    int h = hash(color, M);
    int hn = -1;

    for(int c = MAXCOL; c > 0; c--) {
        if(eq(getColor(HT[h]), getColor(NULLItem))) {
            hn = h;
        } else if(eq(getColor(HT[h]), color)) {
            HT[h].count++;
            return 1;
        }

        h = (h + 1) % M;
    }

    if(hn == -1) return 0;
    HT[hn].color = color;
    HT[hn].count = 1;
    return 1;
}

int HTSearch(Item color) {
    int h = hash(getColor(color), M);
    int c = MAXCOL;

    while(c && !eq(getColor(HT[h]), getColor(color))) 
        c--, h = (h + 1) % M;
    
    if(!c) return -1;
    return 1;
}

int HTMajorColor(int N) {
    for(int i = 0; i < M; i++) {
        if(HT[i].count > N/2) {
            return HT[i].color;
        }
    }

    return;
}

int main() {
    int N;
    while(scanf("%d", &N) && N != 0) {
        HTInit();
        int a;
        for(int i = 0; i < N; i++) {
            scanf("%d", &a);
            if(HTInsert(a) == 0)  fprintf(stderr, "Falha ao inserir a cor %d\n", a);
        }

        printf("%d\n", HTMajorColor(N));
    }

    return 0;
}