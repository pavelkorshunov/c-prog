/*
 * Copyright (C) 2023
 *
 * Created by Pavel K
 */
#include <stdio.h> // printf()
#include <stdlib.h> // exit()

int main(int argc, char **argv) 
{
    // TODO socket(AF_INET6, SOCK_DGRAM, 0);
    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    return 0;
}