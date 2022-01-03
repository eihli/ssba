#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <stdio.h>
#include <stdlib.h>

int main() {
    int i, j;
    static int x[8000][8000];

    clock_t start = clock();

    for (i = 0; i < 8000; i++) {
        for (j = 0; j < 8000; j++) {
            x[i][j] = i + j;
        }
    }
    printf("Version 1 took %ld seconds.\n", clock() - start);

    start = clock();
    for (i = 0; i < 8000; i++) {
        for (j = 0; j < 8000; j++) {
            x[j][i] = i + j;
        }
    }
    printf("Version 1 took %ld seconds.\n", clock() - start);
}
