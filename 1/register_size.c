#include <bits/types.h>
#include <stdio.h>
#include <ctype.h>

int main() {
    int i = 0;
    printf("Or, register size: %lu\n", 1UL << 63);
    i += 5;
    printf("size of uintptr_t: %lu\n", sizeof(void*));
}
