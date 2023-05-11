#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
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

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <path to file>\n", argv[0]);
        exit(1);
    }

    int interface = open(argv[1], O_RDWR | O_NONBLOCK);

    if(interface < 0) {
        char err[100];
        sprintf(err, "Cannot open %s\n", argv[1]);
        perror(err);

        exit(1);
    } else {
        printf("Interface %s open\n", argv[1]);
    }

    close(interface);

    return 0;
};