#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <string.h> // strcpy(), memset(), and memcpy()
#include <unistd.h> // close()
#include <sys/stat.h> // O_RDWR
#include <sys/types.h> // mode_t for 3 argument in open() function
#include <sys/ioctl.h> // ioctl()
#include <sys/socket.h> // socket()
#include <fcntl.h> // open()
#include <linux/if.h> // struct ifreq
#include <linux/if_tun.h> // TUNSETIFF
#include <netinet/in.h> // ntohs(), sockaddr_in
#include <arpa/inet.h> // inet_ntoa()
#include <netinet/ip_icmp.h> // struct icmphdr

// # Run server on <port>.
// ./a.out
// 
// # Test.
// ping -c 1 127.0.0.1

static int get_tunnel()
{
    struct sockaddr_in server_address;
    
    int tunnel = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (tunnel < 0) {
        perror("socket failed");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;

    int bd = bind(tunnel, (struct sockaddr *) &server_address, sizeof(server_address));
    if(bd < 0) {
        perror("bind failed");
        exit(1);
    }

    return tunnel;
}

int main(int argc, char *argv[])
{
    char buffer[1500];
    // struct icmphdr *ihdr;
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);

    int tunnel = get_tunnel();

    while(1) {
        printf("Server waiting icmp\n");

        int nread = recvfrom(tunnel, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &addrlen);

        if(nread < 0) {
            perror("recvfrom failed");
            close(tunnel);
            exit(1);
        }

        printf("Buffer read %d\n", nread);
        printf("From ip %s\n", inet_ntoa(client_address.sin_addr));
    }

    return 0;
}