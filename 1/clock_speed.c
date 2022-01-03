#include <stdio.h>
#include <time.h>

int main() {
    clock_t start = clock(), diff;
    unsigned long i = 0;
    while (i < 4e9) {
        i++;
    }
    diff = clock() - start;
    printf("CLOCKS_PER_SECOND: %lu\n", CLOCKS_PER_SEC);
    printf("Processing took %f ms\n", 1.0 * diff * 1000 / CLOCKS_PER_SEC);
}
