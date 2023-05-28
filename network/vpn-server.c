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
    struct sockaddr_in addr;
    
    int tunnel = socket(AF_INET, SOCK_DGRAM, 0);
    if (tunnel < 0) {
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    int bd = bind(tunnel, (struct sockaddr *)&addr, sizeof(addr));
    if(bd < 0) {
        return -1;
    }

    char packet[1024];
    socklen_t addrlen = sizeof(addr);
    int n = recvfrom(tunnel, packet, sizeof(packet), 0, (struct sockaddr *)&addr, &addrlen);
    if (n <= 0) {
        return -1;
    }

    printf("Buffer: %s\n", packet);

    // Connect to the client as we only handle one client at a time.
    connect(tunnel, (struct sockaddr *)&addr, sizeof(addr));

    return tunnel;
}

int main(int argc, char **argv) 
{
    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    printf("Server waiting on %s port %s\n", argv[1], argv[2]);

    int tunnel = get_tunnel(argv[1], argv[2]);
    if(tunnel < 0) {
        perror("tunnel failed");
        exit(1);
    }

    for (int i = 0; i < 3; ++i) {
        char *message = "upd message";
        send(tunnel, message, sizeof(message), MSG_NOSIGNAL);
    }

    return 0;
}