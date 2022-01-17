#include <stdio.h>
#include <stdint.h>

void swap_bytes(void *val, char i, char j)
{
    uint64_t imask = 0xFF << i * 8;
    uint64_t jmask = 0xFF << j * 8;
    char itemp = *(uint64_t *)val & imask;
    char jtemp = *(uint64_t *)val & jmask;
    *(uint64_t *)val = *(uint64_t *)val & ~jmask | jtemp;
    *(uint64_t *)val = *(uint64_t *)val & ~imask | itemp;
}
void swap_endian(void *val, short size)
{
    for (char i = 0; i < size / 2; i++)
        swap_bytes(val, i, size - i - 1);
}

int main() {
    uint64_t i = 0xF0F0;
    swap_endian(&i, 2);
    printf("Swapped: %lx\n", i);
}
