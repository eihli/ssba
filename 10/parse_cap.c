#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// I don't know what the maximum length I might want to read
// as part of parsing a cap file. But 2048 seems like a reasonable start.
#define BUF_LEN 2048

char buffer[BUF_LEN] = {0};

typedef struct caph_t {
    uint32_t magic_number;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t snaplen;
    uint32_t linktype;
} caph_t;

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
    caph_t header = {0};
    read(fd, &header.magic_number, 4);
    read(fd, &header.major_version, 2);
    read(fd, &header.minor_version, 2);
    // Skip 64 bits of "reserved" that MUST be ignored.
    lseek(fd, 8, SEEK_CUR);
    read(fd, &header.snaplen, 4);
    read(fd, &header.linktype, 4);
    printf("Magic Number: %X\n", header.magic_number);
    printf("Major Version: %hX\n", header.major_version);
    printf("Minor Version: %hX\n", header.minor_version);
    printf("Snaplen: %d\n", header.snaplen);
    printf("Linktype: %X\n", header.linktype);
    
}
