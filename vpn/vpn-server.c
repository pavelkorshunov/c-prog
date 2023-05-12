#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>

#ifdef __linux__

#include <net/if.h>
#include <linux/if_tun.h>

static int get_interface(char *name)
{
    struct ifreq ifr;

    int interface = open("/dev/net/tun", O_RDWR | O_NONBLOCK);

    if(interface < 0) {
        perror("Cannot open /dev/net/tun");
        exit(1);
    }
    
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

    strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));

    if (ioctl(interface, TUNSETIFF, &ifr)) {
        perror("Cannot get TUN interface");
        exit(1);
    }

    return interface;
}

#else
#error Sorry, you have to implement this part by yourself.
#endif

static int get_socket() {
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("Cannot open socket");
        exit(1);
    }

    return sockfd;
};

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <path to file>\n", argv[0]);
        exit(1);
    }

    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *address;
    char *if_name = argv[1];

    sockfd = get_socket();

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, sizeof(if_name));

    if (ioctl(sockfd, SIOCGIFADDR, &ifr)) {
        perror("Cannot get IP address");
        exit(1);
    }

    address = (struct sockaddr_in *) &ifr.ifr_addr;

    printf("Interface %s. IP address %s\n", if_name, inet_ntoa(address->sin_addr));

    close(sockfd);

    return 0;
};