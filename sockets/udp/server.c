/*
 * Copyright (C) 2023
 *
 * Created by Pavel K
 */
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <string.h> // strcpy(), memset(), and memcpy()
#include <unistd.h> // close()
#include <sys/stat.h> // O_RDWR
#include <sys/types.h> // mode_t for 3 argument in open() function
#include <sys/socket.h> // socket()
#include <sys/ioctl.h> // ioctl()
#include <fcntl.h> // open()
#include <linux/if.h> // struct ifreq
#include <linux/if_tun.h> // TUNSETIFF
#include <netinet/in.h> // ntohs(), sockaddr_in
#include <arpa/inet.h> // inet_ntoa()

// # Run server on <ip> <port>.
// ./a.out 127.0.0.1 8000
//
// # Listen all interfaces
// ./a.out 0.0.0.0 8000
//
//
// # Send udp
// nc -nvu 127.0.0.1 8000
// echo -n "123" | nc -u -w1  127.0.0.1 8000

static int get_tunnel(char *ip, char *port)
{
    struct sockaddr_in server_address;
    
    int tunnel = socket(AF_INET, SOCK_DGRAM, 0);
    if (tunnel < 0) {
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(atoi(port));

    int bd = bind(tunnel, (struct sockaddr *) &server_address, sizeof(server_address));
    if(bd < 0) {
        return -1;
    }

    return tunnel;
}

int main(int argc, char **argv) 
{
    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);

    int tunnel = get_tunnel(argv[1], argv[2]);
    if(tunnel < 0) {
        perror("tunnel failed");
        exit(1);
    }

    while(1) {
        printf("Server waiting on %s port %s\n", argv[1], argv[2]);

        char buffer[1500];

        int nread = recvfrom(tunnel, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &addrlen);

        if(nread < 0) {
            perror("recvfrom failed");
            close(tunnel);
            exit(1);
        }

        printf("Buffer read %d\n", nread);
        printf("From ip %s\n", inet_ntoa(client_address.sin_addr));
        printf("Buffer: %s\n", buffer);
    }

    return 0;
}