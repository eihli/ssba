#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argv, char *argc[])
{
    if (argv != 2) {
        printf("Usage: usemem <amount of memory to use, in megabytes>\n");
        exit(EXIT_SUCCESS);
    }

    int mem = atoi(argc[1]);
    printf("Allocating %d mb of memory\n", mem);
    uint8_t *m = malloc(mem * 1e6 * sizeof(uint8_t));
    uint8_t *o = m;
    int tmp, i;
    while (1) {
        for (i = 0; i < mem; i++) {
            *(m++) = 42;
        }
    }
}
