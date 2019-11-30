#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *host;
    int sockfd, len, result;
    struct sockaddr_in address;
    struct servent *servinfo;
    struct hostent *hostinfo;
    char buffer[] = "GET / HTTP/1.0\r\nConnection: close\r\n\r\n";
    char readBuffer[512];

    if(argc == 1) {
        host = "localhost";
    } else {
        host = argv[1];
    }
    hostinfo = gethostbyname(host);
    if(!hostinfo) {
        fprintf(stderr, "no host: %s\n", host);
        exit(1);
    }

    servinfo = getservbyname("http", "tcp");
    if(!servinfo) {
        fprintf(stderr, "no http service\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = servinfo->s_port;
    address.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1) {
        perror("opps: http");
        exit(1);
    }

    write(sockfd, &buffer, sizeof(buffer));
    result = read(sockfd, &readBuffer, sizeof(readBuffer));
    readBuffer[result] = '\0';
    printf("%s", readBuffer);

    close(sockfd);

    return 0;
}