#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef uint8_t byte;
typedef struct Option {
    byte kind;
    byte length;
    byte *data;
} option;


void filecopy(FILE *ifp, FILE *ofp);
void read_bytes(FILE *ifp, byte *buffer, long offset, int n);
void read_option(FILE *fp, byte *buffer);
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
    int16_t dst_prt = ((int16_t *) buffer)[0];

    // Sequence #
    read_bytes(fp, buffer, 4, 4);
    long seq = ((long *)buffer)[0];

    // Acknowledgement #
    read_bytes(fp, buffer, 8, 4);
    long ack = ((long *)buffer)[0];

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
    int i;
    option current;
    for (i = 0, current = options[i]; current.length != 0; i++) {
        printf("%10s | %10s\n", "kind", "length");
        printf("%10d | %10d\n", current.kind, current.length);
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
    long orig = ftell(fp);
    printf("Original seek position: %lu\n", orig);
    fseek(fp, offset, SEEK_SET);
    printf("New seek position: %lu\n", offset);
    fread(buffer, 1, n, fp);
    fseek(fp, orig, SEEK_SET);
}

void read_option(FILE *fp, byte *buffer)
{
    const byte OPT_OFST = 20;
    long orig = ftell(fp);
    fseek(fp, OPT_OFST, SEEK_SET);

    byte option_kind = getc(fp);
    printf("Kind: %d\n", option_kind);

    byte option_len = getc(fp);
    printf("Len:  %x\n", option_len);

    fread(buffer, 1, option_len, fp);
    printf("Data: ");
    for (int i = 0; i < option_len; i++)
        printf("%c ", (byte) buffer[i]);
    printf("\n");
    fseek(fp, orig, SEEK_SET);
}

option *read_options(FILE *fp, option *options)
{
    // Save original seek position so we can restore it when done.
    long orig = ftell(fp);

    // Seek to the offset where we can find the size of the TCP header
    fseek(fp, 12, SEEK_SET);
    const byte header_size = getc(fp) >> 4 << 2;

    // Seek to the options
    fseek(fp, 20, SEEK_SET);
    byte kind, length, *data;
    while (ftell(fp) < header_size) {
        kind = getc(fp);
        length = getc(fp);
        data = (byte *)malloc(sizeof(byte) * length);
        for (byte i = 0; i < length; i++)
            data[i] = getc(fp);
        options[0] = (option){ kind, length, data };
    }
    options[ftell(fp)] = (option) { 0, 0, NULL };

    fseek(fp, orig, SEEK_SET);
    return options;
}
