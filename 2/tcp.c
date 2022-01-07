#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <bits/byteswap.h>

typedef uint8_t byte;
typedef struct Option {
    byte kind;
    byte length;
    byte *data;
} option;


void filecopy(FILE *ifp, FILE *ofp);
void read_bytes(FILE *ifp, byte *buffer, long offset, int n);
uint8_t get8(FILE *fp)
{
    uint8_t v;
    fread(&v, sizeof(v), 1, fp);
    return v;
};
uint16_t get16(FILE *fp)
{
    uint16_t v;
    fread(&v, sizeof(v), 1, fp);
    return ntohs(v);
}
uint32_t get32(FILE *fp)
{
    uint32_t v;
    fread(&v, sizeof(v), 1, fp);
    return ntohl(v);
}
uint64_t get64(FILE *fp)
{
    uint64_t v;
    fread(&v, sizeof(v), 1, fp);
    return __bswap_64(v);
}
void read_option(FILE *fp, byte *buffer);
void swap_endian_l(uint32_t *val);
option *read_options(FILE *fp, option *options);

#define BUF_SIZE = 60; // Maximum number of bytes we'd possibly read from a TCP header.
byte buffer[60];


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <tcpheader_file>\n", argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("%s - Error opening %s", strerror(errno), argv[1]);
    }

    // Source port
    read_bytes(fp, buffer, 0, 2);
    int src_prt = (buffer[0] << 8) + buffer[1];

    // Destination port
    read_bytes(fp, buffer, 2, 2);
    uint16_t dst_prt = (buffer[0] << 8) + buffer[1];

    // Sequence #
    fseek(fp, 4 ,SEEK_SET);
    uint32_t seq = get32(fp);
    /* read_bytes(fp, buffer, 4, 4); */
    /* uint32_t seq = ((uint32_t *)buffer)[0]; */
    /* swap_endian_l(&seq); */

    // Acknowledgement #
    read_bytes(fp, buffer, 8, 4);
    uint32_t ack = ((uint32_t *)buffer)[0];
    swap_endian_l(&ack);

    // Data offset
    read_bytes(fp, buffer, 12, 1);
    byte data_offset = buffer[0] >> 4 << 2;

    printf("%10s | %10s | %20s | %20s\n", "src", "dst", "seq", "ack");
    printf("%10d | %10d | %20lu | %20lu\n", src_prt, dst_prt, seq, ack);
    printf("\n\n");
    printf("%10s | \n", "data");
    printf("%10d | \n", data_offset);

    option options[20];

    read_options(fp, options);
    for (int i = 0; options[i].kind != 0; i++) {
        printf("%10s | %10s\n", "kind", "length");
        printf("%10d | %10d\n", options[i].kind, options[i].length);
    }
}

void filecopy(FILE *ifp, FILE *ofp)
{
    byte c;
    while ((c = getc(ifp)) != EOF) {
        putc(c, ofp);
    }
}

void read_bytes(FILE *fp, byte *buffer, long offset, int n)
{
    uint32_t orig = ftell(fp);
    fseek(fp, offset, SEEK_SET);
    int r = fread(buffer, sizeof(byte), n, fp);
    fseek(fp, orig, SEEK_SET);
}

option *read_options(FILE *fp, option *options)
{
    // Save original seek position so we can restore it when done.
    uint32_t orig = ftell(fp);

    // Seek to the offset where we can find the size of the TCP header
    fseek(fp, 12, SEEK_SET);
    const byte header_size = getc(fp) >> 4 << 2; // in bytes

    // Seek to the options
    fseek(fp, 20, SEEK_SET);
    byte kind, length, **data, i = 0;
    while (ftell(fp) < header_size) {
        kind = get8(fp);
        if (kind == 0 || kind == 1) {
            options[i] = (option) { kind, 0, NULL };
        } else {
            length = get8(fp);
            for (int i = 2; i < length; i++)
                get8(fp);
            options[i] = (option) { kind, length, NULL };
        }
        i++;
    }
    options[i] = (option) { 0, 0, NULL };

    fseek(fp, orig, SEEK_SET);
    return options;
}

void swap_endian_l(uint32_t *val)
{
    uint32_t temp = 0;
    temp = (
        ((*val & 0xFF) << 24)
        | ((*val & 0xFF00) << 8)
        | ((*val & 0xFF0000) >> 8)
        | ((*val & 0xFF000000) >> 24)
    );
    *val = temp;
}
