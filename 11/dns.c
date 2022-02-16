#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/in.h>
// arpa/inet.h has the inet_pton function which creates structs
// from the "presentation" representation of an address - "192.168.1.0".

// Arbitaritly chosen max receive buffer size
#define MAX_RECV 8192

#define A 0x01
#define NS 0x02
#define MD 0x03
#define MF 0x04
#define CNAME 0x05
#define SOA 0x06
#define MB 0x07
#define MX 0x15
#define TXT 0x16

#define IN 0x01
#define CS 0x02
#define CH 0x03
#define HS 0x04



/*
** Naming this after the inet_pton functions - presentation to network -> presentation to dns.
** Returne buffer will be length strlen(domain);
*/
uint8_t *ptodns(char *domain)
{
    uint8_t *result = malloc(strlen(domain));
    int i, j;
    int end = strlen(domain);
    for (i = 0, j = 0; i < end; i++, j++) {
        if (domain[i] == '.') {
            *result = j;
            memcpy(++result, &domain[i - j], j);
            result += j;
            j = -1;
        }
    }
    *result = j;
    memcpy(++result, &domain[i - j], j);
    result += j;
    result -= i;
    return result;
}

char *dnstop(uint8_t *dnsname)
{
    int len = 0;
    while (dnsname[len])
        len++;
    char *domain = malloc(len * sizeof(char));
    int read, toread;
    read = toread = *dnsname++;
    memcpy(domain, dnsname, toread);
    dnsname += toread;
    toread = *dnsname++;
    while (toread) {
        domain += read;
        read = toread;
        *domain = '.';
        domain++;
        memcpy(domain, dnsname, toread);
        dnsname += toread;
        toread = *dnsname;
    }
    return domain - len + read + 1;
}

int main(int argc, char *argv[])
{
    int socket_fd;
    
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Error creating socket.");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(53),
        .sin_addr = {0}, // Will create later with inet_pton.
        .sin_zero = {0}
    };
    
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) {
        printf("inet_pton error\n");
        exit(EXIT_FAILURE);
    }

    struct header_t {
        uint16_t id;
        uint16_t qr : 1;
        uint16_t opcode : 4;
        uint16_t aa : 1;
        uint16_t tc : 1;
        uint16_t rd : 1;
        uint16_t ra : 1;
        uint16_t z : 3;
        uint16_t rcode : 4;
        uint16_t qdcount;
        uint16_t ancount;
        uint16_t nscount;
        uint16_t arcount;
    } req_header, res_header;
    
    req_header = (struct header_t) {
        .id = 0,
        .qr = 0,
        .opcode = 0,
        .qdcount = htons(1)
    };
    
    int sent;
    
    struct question_t {
        char *qname;
        uint16_t qtype;
        uint16_t qclass;
    } question;
    
    int qname_len = 2 + strlen(argv[2]);
    char *qname = malloc(qname_len);
    int i, j, b;
    for (i = 0, j = 0, b = 0; i < strlen(argv[2]); i++, j++) {
        if (argv[2][i] == '.') {
            qname[b++] = j;
            memcpy(&qname[b], &argv[2][i - j], j);
            b += j;
            j = -1;
        }
    }
    qname[b++] = j;
    memcpy(&qname[b], &argv[2][i - j], j);
    qname[++i] = '\0';
    
    question.qname = qname;
    question.qtype = htons(A);
    question.qclass = htons(IN);
    struct request_t {
        struct header_t header;
        struct question_t question;
    } request;
    
    request = (struct request_t) {
        .header = req_header,
        .question = question
    };
    
    // Pack the header and question into a buffer to send.
    // Header + Question - Pointer + QNAME
    int message_size = sizeof(struct request_t) - sizeof(void *) + qname_len;
    uint8_t *request_msg = malloc(message_size);
    memcpy(request_msg, &req_header, sizeof(struct header_t));
    request_msg += sizeof(struct header_t);
    memcpy(request_msg, question.qname, qname_len);
    request_msg += qname_len;
    memcpy(request_msg, &question.qtype, 2 * sizeof(uint16_t));
    request_msg -= sizeof(struct header_t) + qname_len;
    
    sent = sendto(
        socket_fd,
        request_msg,
        sizeof(struct request_t) - sizeof(void *) + qname_len,
        0,
        (struct sockaddr *) &addr,
        sizeof(struct sockaddr_in)
    );
    if (sent == -1)
        perror("sendto");
    else
        printf("Sent %d bytes to %s:%d by way of file descriptor %d\n", sent, inet_ntoa(addr.sin_addr), addr.sin_port, socket_fd);

    uint8_t receive_buffer[MAX_RECV];
    
    struct sockaddr_in addr_response = {
        .sin_family = AF_INET,
        .sin_port = 0,
        .sin_addr = {0}, // Will create later with inet_pton.
        .sin_zero = {0}
    };
    socklen_t addrlen = sizeof(addr_response);
    
    int received_len = recvfrom(
        socket_fd,
        receive_buffer,
        MAX_RECV,
        0,
        (struct sockaddr *)&addr_response,
        &addrlen
    );
    
    printf("Received %d bytes from %s\n", received_len, inet_ntoa(addr_response.sin_addr));
    for (int i = 0; i < received_len; i++) {
        if (i % 16 == 0)
            printf("\n 0x%02X\t", i);
        printf("%02x ", receive_buffer[i]);
    }
    printf("\n");
    
    memcpy(&res_header, receive_buffer, 4);
    res_header.ancount = ntohs(*((uint16_t *) (receive_buffer + 6)));
    
    char *domain = dnstop(receive_buffer + 12);
    printf("Found answer for domain %s.\n", domain);
    uint16_t type = *(uint16_t *) receive_buffer + 12 + strlen(domain);
    type = ntohs(type);
    uint16_t class = *(uint16_t *) receive_buffer + 12 + strlen(domain) + 2;
    class = ntohs(class);
    uint32_t ttl = *(uint32_t *) receive_buffer + 12 + strlen(domain) + 4;
    ttl = ntohs(ttl);
    uint16_t rdlen = *(uint16_t *) receive_buffer + 12 + strlen(domain) + 8;
    rdlen = ntohs(rdlen);
    uint8_t *rdata = malloc(rdlen);
    memcpy(rdata, receive_buffer + 12 + strlen(domain) + 10, rdlen);
    
    printf(
        "IPV4 - Source: %hd.%hd.%hd.%hd\n",
        (unsigned char) (*rdata >> 24 & 0xFF),
        (unsigned char) ((*rdata >> 16) & 0xFF),
        (unsigned char) ((*rdata >> 8) & 0xFF),
        (unsigned char) (*rdata & 0xFF)
    );
    
    
}
