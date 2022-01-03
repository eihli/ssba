#include <stdio.h>

void filecopy(FILE *ifp, FILE *ofp);
void read_bytes(FILE *ifp, char *buffer, long offset, int n);

int main()
{
    unsigned char b[10];
    unsigned char n;
    unsigned int src_prt, dst_prt = 0;
    size_t sz;
    FILE *fp = fopen("tcpheader", "rb");
    fread(&n, 1, sizeof(n), fp);
    src_prt = n;
    fread(&n, 1, sizeof(n), fp);
    src_prt = src_prt << 8;
    src_prt = src_prt + n;

    char seq[4];
    read_bytes(fp, seq, 4, 4);
    unsigned long sequence_number = 0;
    for (int i = 0; i < 4; i++)
        sequence_number = (sequence_number << 8) + seq[i];

    printf("%lu", sequence_number);

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

void read_bytes(FILE *fp, char *buffer, long offset, int n)
{
    long orig = ftell(fp);
    fseek(fp, offset, SEEK_SET);
    fread(buffer, 1, n, fp);
    fseek(fp, orig, SEEK_SET);
}
