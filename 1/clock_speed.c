#include <stdio.h>
#include <time.h>

int main() {
    clock_t start = clock(), elapsed;
    unsigned long i, iters;
    i = 0;
    iters = 1e9;
    while (i < iters) {
        i++;
    }
    elapsed = clock() - start;
    float elapsed_s = (float) elapsed / CLOCKS_PER_SEC;
    float hz = iters / elapsed_s;
    printf("Processing took\t%f seconds\n", elapsed_s);
    printf("Estimated clock speed is\t%f GHz\n", hz / 1e9);
}
