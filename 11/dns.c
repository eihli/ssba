// arpa/inet.h has the inet_pton function which creates structs
// from the "presentation" representation of an address - "192.168.1.0".
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <linux/in.h>

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
    
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1)
        perror("inet_pton");

    ssize_t sent;
    sent = sendto(socket_fd, argv[2], sizeof(char) * strlen(argv[2]), 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (sent == -1)
        perror("sendto");
    else
        printf("Sent %d bytes to %s:%d by way of file descriptor %d\n", sent, inet_ntoa(addr.sin_addr), addr.sin_port, socket_fd);
}
