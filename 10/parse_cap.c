#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// I don't know what the maximum length I might want to read
// as part of parsing a cap file. But 2048 seems like a reasonable start.
// This is the buffer for the data of a single packet.
#define BUF_LEN 2048

// We can't know how many packets are in a cap file until
// we finish parsing it, so we'll have buffer of arbitrary size for that too.
#define PACKETS_BUF_LEN 8192

char buffer[BUF_LEN] = {0};

// Captured packet header
typedef struct caph_t {
    uint32_t magic_number;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t snaplen;
    uint32_t linktype;
} caph_t;
// Packet record
typedef struct packetrec_t {
    uint32_t timestamp_seconds;
    uint32_t timestamp_fractional;
    uint32_t cap_len;
    uint32_t orig_len;
    uint8_t *data;
} packetrec_t;

int num_packets = 0;
packetrec_t packets[PACKETS_BUF_LEN] = {0};

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
    
    struct stat f_stat;
    fstat(fd, &f_stat);
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
    

    packetrec_t packet = {0};
    for (int i = 0; i < PACKETS_BUF_LEN && lseek(fd, 0, SEEK_CUR) < f_stat.st_size; i++) {
        read(fd, &packet.timestamp_seconds, 4);
        read(fd, &packet.timestamp_fractional, 4);
        read(fd, &packet.cap_len, 4);
        read(fd, &packet.orig_len, 4);
        packet.data = malloc(packet.cap_len * sizeof(uint8_t));
        read(fd, packet.data, packet.cap_len);
        memcpy(&packets[i], &packet, sizeof(packetrec_t));
        printf("Packet: %d \t Timestamp: %d \t Len: %d \t Orig: %d\n", i, packet.timestamp_seconds, packet.cap_len, packet.orig_len);
    }
}
