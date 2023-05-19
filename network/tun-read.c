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
#include <sys/ioctl.h> // ioctl()
#include <fcntl.h> // open()
#include <linux/if.h> // struct ifreq
#include <linux/if_tun.h> // TUNSETIFF

// Headers
#define IP4_HDRLEN 20 // IPv4 header length
#define ICMP_HDRLEN 8 // ICMP header length

// # Create a TUN interface.
// ip tuntap add dev tun0 mode tun
//
// # Set the addresses.
// ip address add 192.168.0.1/24 dev tun0
//
// ip link set tun0 up
//
// Run ./a.out
// Run ping 192.168.0.2
//
// # Clear after testing
// ip addr del 192.168.0.1/24 dev tun0
// ip tuntap del dev tun0 mode tun

int tun_alloc(char *dev, int flags) {

    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";

    /* Arguments taken by the function:
     *
     * char *dev: the name of an interface (or '\0'). MUST have enough
     *   space to hold the interface name if '\0' is passed
     * int flags: interface flags (eg, IFF_TUN etc.)
     */

    /* open the clone device */
    if( (fd = open(clonedev, O_RDWR)) < 0 ) {
        return fd;
    }

    /* preparation of the struct ifr, of type "struct ifreq" */
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

    if (*dev) {
        /* if a device name was specified, put it in the structure; otherwise,
         * the kernel will try to allocate the "next" device of the
         * specified type */
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    /* try to create the device */
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        close(fd);
        return err;
    }

    /* if the operation was successful, write back the name of the
     * interface to the variable "dev", so the caller can know
     * it. Note that the caller MUST reserve space in *dev (see calling
     * code below) */
    strcpy(dev, ifr.ifr_name);

    /* this is the special file descriptor that the caller will use to talk
     * with the virtual interface */
    return fd;
}

int main(void)
{
    char buffer[1500];
    char tun_name[IFNAMSIZ];

    /* Connect to the device */
    strcpy(tun_name, "tun0");
    int tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI);  /* tun interface */

    if(tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }

    /* Now read data coming from the kernel */
    while(1) {
        /* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
        ssize_t nread = read(tun_fd, buffer, sizeof(buffer));
        if(nread < 0) {
            perror("Reading from interface");
            close(tun_fd);
            exit(1);
        }

        /* Do whatever with the data */
        printf("Read %ld bytes from device %s\n", nread, tun_name);
    }

    return 0;
}