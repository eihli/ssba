#include <stdio.h>
#include <math.h>
#include <stdint.h>

int main()
{
    uint32_t buz;
    float bar = NAN;
    uint8_t foo[4] = {0x01, 0x00, 0xc0, 0x7f};
    uint8_t baz[4] = {0x02, 0x00, 0xc0, 0x7f};

    printf("  %x\n", *((unsigned int *) &foo));
    printf("+ %x\n", *((unsigned int *) &baz));
    printf("----------\n");
    float biz = *((float *) &foo) + *((float *) &baz);
    printf("= %x\n", *((unsigned int *) &biz));
    printf("= %f\n", *((float *) &biz));
}
