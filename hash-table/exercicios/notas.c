// #include <stdio.h>

// #define MAXGRADE 1001

// int main() {
//     int N;
//     scanf("%d", &N);

//     int grades[MAXGRADE] = {0};

//     for(int i = 0; i < N; i++) {
//         int x;
//         scanf("%d", &x);
//         grades[x]++;
//     }

//     int count = 0, maxGrade = 0;

//     for(int i = 0; i < MAXGRADE; i++) {
//         if(grades[i] > count || (grades[i] == count && i > maxGrade)) {
//             maxGrade = i;
//             count = grades[i];
//         }
//     }

//     printf("%d\n", maxGrade);

//     return 0;
// }


#define hash(A, M) (A % M)
#define MAXCOL 10
#define NULLItem (Item){.key = -1, .data = -1}

typedef struct {
    int key;
    int data;
} Item;

typedef struct {
    Item *itens;
    int M;
} HashT;

HashT *HT;

void HTInit(int Max) {
    HT = malloc(Max * sizeof(Item));
    HT->M = Max;

    for(int i = 0; i < HT->M; i++) {
        HT->itens[i] = NULLItem;
    }
}