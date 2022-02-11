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
// Ethernet frame
typedef struct ethernet_frame_t {
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint8_t type_len[2];
    uint8_t *data;
    uint8_t fcs[4];
} ethernet_frame_t;
uint8_t F_8021Q[2] = {0x81, 0x00};
uint8_t F_IPV4[2] = {0x08, 0x00};
uint8_t F_IPV6[2] = {0x86, 0xDD};
uint8_t F_ARP[2] = {0x08, 0x06};
ethernet_frame_t *make_ethernet_frame(uint8_t *data, int size)
{
    ethernet_frame_t *frame = malloc(sizeof(ethernet_frame_t));
    memcpy(&(frame->dest_mac), data, 6);
    memcpy(&(frame->src_mac), data + 6, 6);
    memcpy(&(frame->type_len), data + 12, 2);
    if (memcmp(&(frame->type_len), F_IPV4, 2) == 0) {
        frame->data = malloc(size);
        memcpy(frame->data, data + 14, size);
    }
    memcpy(&(frame->fcs), data + 14 + size, 4);
    return frame;
}

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
    

    for (int i = 0; i < PACKETS_BUF_LEN && lseek(fd, 0, SEEK_CUR) < f_stat.st_size; i++) {
        read(fd, &packets[i].timestamp_seconds, 4);
        read(fd, &packets[i].timestamp_fractional, 4);
        read(fd, &packets[i].cap_len, 4);
        read(fd, &packets[i].orig_len, 4);
        packets[i].data = malloc(packets[i].cap_len * sizeof(uint8_t));
        read(fd, packets[i].data, packets[i].cap_len);
        printf("Packet: %d \t Timestamp: %d \t Len: %d \t Orig: %d\n", i, packets[i].timestamp_seconds, packets[i].cap_len, packets[i].orig_len);
        
        ethernet_frame_t *frame = make_ethernet_frame(packets[i].data, packets[i].cap_len);
        printf(
            "Eth Frame - Dest: %02X:%02X:%02X:%02X:%02X:%02X \t Src %02X:%02X:%02X:%02X:%02X:%02X \t Len %d \n",
            frame->dest_mac[0],
            frame->dest_mac[1],
            frame->dest_mac[2],
            frame->dest_mac[3],
            frame->dest_mac[4],
            frame->dest_mac[5],
            frame->src_mac[0],
            frame->src_mac[1],
            frame->src_mac[2],
            frame->src_mac[3],
            frame->src_mac[4],
            frame->src_mac[5],
            packets[i].cap_len
        );
    }
}
