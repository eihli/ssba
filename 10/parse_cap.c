#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// I don't know what the maximum length I might want to read
// as part of parsing a cap file. But 2048 seems like a reasonable start.
#define BUF_LEN 2048

char buffer[BUF_LEN] = {0};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: parse_cap <filename>.\n");
        return 1;
    }

    int fd = 0;
    
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "open failed %m\n");
        return 1;
    }
    ssize_t sz;
    sz = read(fd, buffer, 4);
    
    printf("Magic Number: %X\n", ntohl(*((uint32_t *) buffer)));
    
}
