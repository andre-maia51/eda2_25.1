#include <stdio.h>
#include <stdlib.h>

#define MAX 500

int N, M;
char matrix[MAX][MAX];

int main() {
    scanf("%d %d", &N, &M);

    for (int i = 0; i < N; i++) {
        scanf("%s", matrix[i]);
    }

    int changed;
    do {
        changed = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (matrix[i][j] != '.') continue;

                if (i > 0 && matrix[i - 1][j] == 'o') {
                    matrix[i][j] = 'o';
                    changed = 1;
                }
                else if (j > 0 && i + 1 < N &&
                         matrix[i][j - 1] == 'o' &&
                         matrix[i + 1][j - 1] == '#') {
                    matrix[i][j] = 'o';
                    changed = 1;
                }
                else if (j + 1 < M && i + 1 < N &&
                         matrix[i][j + 1] == 'o' &&
                         matrix[i + 1][j + 1] == '#') {
                    matrix[i][j] = 'o';
                    changed = 1;
                }
            }
        }
    } while (changed);

    for (int i = 0; i < N; i++) {
        printf("%s\n", matrix[i]);
    }

    return 0;
}
