#include <stdio.h>
#include <stdlib.h>

#define key int
#define data int

#define getKey(A) (A.k)
#define less(A, B) (getKey(A) < getKey(B))
#define eq(A, B) (getKey(A) == getKey(B))
#define swap(A, B) {Item temp = A; A = B; B = temp;}
#define isRed(A) (A->color == RED)
#define NULLItem (Item) {-1}

typedef struct STNode *link;
enum color {RED, BLACK};

typedef struct {
    key k;
    data d;
} Item;

struct STNode {
    Item item;
    link l, r;
    int N;
    enum color color;
};

link h, z;

link new(Item item, link l, link r, int N) {
    link x = malloc(sizeof(*x));
    x->item = item;
    x->l = l;
    x->r = r;
    x->N = N;
    x->color = RED;

    return x;
}

void STInit() {
    h = (z = (new(NULLItem, 0, 0, 0)));
    z->color = BLACK;
    z->l = z->r = z;
}

int STCount() {
    return h->N;
}

link rotateLeft(link r) {
    link x = r->r;
    r->r = x->l;
    x->l = r;
    x->color = r->color;
    r->color = RED;
    x->N = r->N;
    r->N = 1 + r->l->N + r->r->N;

    return x;
}

link rotateRight(link r) {
    link x = r->l;
    r->l = x->r;
    x->r = r;
    x->color = r->color;
    r->color = RED;
    x->N = r->N;
    r->N = 1 + r->l->N + r->r->N;

    return x;
}

void flipColors(link r) {
    r->l->color = BLACK;
    r->r->color = BLACK;
    r->color = RED;
}

link insertR(link r, Item item) {
    if(r == z) return new(item, z, z, 1);

    if(less(item, r->item)) r->l = insertR(r->l, item);
    else r->r = insertR(r->r, item);

    if(isRed(r->r) && !isRed(r->l)) r = rotateLeft(r);
    if(isRed(r->l) && isRed(r->l->l)) r = rotateRight(r);
    if(isRed(r->l) && isRed(r->r)) flipColors(r);
    r->N = r->l->N + r->r->N + 1;

    return r;
}

link STSearch(link r, Item item) {
    if(r == z || eq(r->item, item)) return r;

    if(less(item, r->item)) return STSearch(r->l, item);
    else return STSearch(r->r, item);
}

void inOrder(link r) {
    if(r != z) {
        inOrder(r->l);
        printf("%d ", r->item.k);  
        inOrder(r->r);
    }
}

void preOrder(link r) {
    if(r != z) {
        printf("%d ", r->item.k);  
        preOrder(r->l);
        preOrder(r->r);
    }
}

void postOrder(link r) {
    if(r != z) {
        postOrder(r->l);
        postOrder(r->r);
        printf("%d ", r->item.k);  
    }
}

// Essa Main foi gerada pelo ChatGPT apenas pra testar a implementação
int main() {
    STInit();

    int valores[] = {10, 20, 30, 15, 5, 25};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        Item novo;
        novo.k = valores[i];
        novo.d = 0; // valor arbitrário para o dado

        h = insertR(h, novo); // insere o item na árvore
        h->color = BLACK; // a raiz sempre deve ser preta

        printf("\nApós inserir %d:\n", novo.k);
        printf("In-order: ");
        inOrder(h);
        printf("\n");

        printf("Pre-order: ");
        preOrder(h);
        printf("\n");

        printf("Post-order: ");
        postOrder(h);
        printf("\n");
    }

    return 0;
}
