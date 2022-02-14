#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/in.h>
// arpa/inet.h has the inet_pton function which creates structs
// from the "presentation" representation of an address - "192.168.1.0".

int main(int argc, char *argv[])
{
    int socket_fd;
    
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("Error creating socket.");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = 8888, // Don't provide port, have one chosen for us.
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
        .qdcount = 1
    };
    
    int sent;
    
    uint8_t *qname = malloc(2 + strlen(argv[2]));
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
    
    sent = sendto(socket_fd, argv[2], sizeof(char) * strlen(argv[2]), 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (sent == -1)
        perror("sendto");
    else
        printf("Sent %d bytes to %s:%d by way of file descriptor %d\n", sent, inet_ntoa(addr.sin_addr), addr.sin_port, socket_fd);
}
