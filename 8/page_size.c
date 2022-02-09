#include "unistd.h"
#include "stdio.h"

int main()
{
    printf("Page size: %d\n", getpagesize());
}
