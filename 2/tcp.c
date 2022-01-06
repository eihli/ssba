#include <stdio.h>

void filecopy(FILE *ifp, FILE *ofp);
void read_bytes(FILE *ifp, unsigned char *buffer, long offset, int n);
void read_option(FILE *fp, char *buffer);

int main()
{
    char b[10];
    unsigned char n;
    unsigned int src_prt, dst_prt = 0;
    size_t sz;
    FILE *fp = fopen("tcpheader", "rb");
    fread(&n, 1, sizeof(n), fp);
    src_prt = n;
    fread(&n, 1, sizeof(n), fp);
    src_prt = src_prt << 8;
    src_prt = src_prt + n;

    unsigned char seq[4];
    read_bytes(fp, seq, 4, 4);
    unsigned long sequence_number = 0;
    for (int i = 0; i < 4; i++)
        sequence_number = (sequence_number << 8) + seq[i];

    unsigned char data_offset;

    /* fseek(fp, 12, SEEK_SET); */
    /* data_offset = getc(fp); */
    /* printf("Data offset: %d", data_offset >> 4); */

    read_bytes(fp, &data_offset, 12, 1);
    data_offset = data_offset >> 4 << 2;
    printf("Sequence #: %lu\n", sequence_number);
    printf("Data offset. Bytes: %d. 32-bit words: %d.\n", data_offset, data_offset >> 2);

    read_option(fp, b);
    /* while ((sz = fread(b, 1, sizeof(b), fp)) > 0) { */
    /*     for (int i = 0; i < sz; i++) { */
    /*         printf("%02x ", b[i]); */
    /*     } */
    /* } */
}

void filecopy(FILE *ifp, FILE *ofp)
{
    char c;
    while ((c = getc(ifp)) != EOF) {
        putc(c, ofp);
    }
}

void read_bytes(FILE *fp, unsigned char *buffer, long offset, int n)
{
    long orig = ftell(fp);
    printf("Original seek position: %lu\n", orig);
    fseek(fp, offset, SEEK_SET);
    printf("New seek position: %lu\n", offset);
    fread(buffer, 1, n, fp);
    fseek(fp, orig, SEEK_SET);
}

void read_option(FILE *fp, char *buffer)
{
    const char OPT_OFST = 20;
    long orig = ftell(fp);
    fseek(fp, OPT_OFST, SEEK_SET);

    unsigned char option_kind = getc(fp);
    printf("Kind: %d\n", option_kind);

    unsigned char option_len = getc(fp);
    printf("Len:  %x\n", option_len);

    fread(buffer, 1, option_len, fp);
    printf("Data: ");
    for (int i = 0; i < option_len; i++)
        printf("%c ", (unsigned char) buffer[i]);
    printf("\n");
    fseek(fp, orig, SEEK_SET);
}
