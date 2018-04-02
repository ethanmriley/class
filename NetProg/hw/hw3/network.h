#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

struct server_socket {
    int sockfd;
    struct sockaddr_in sockaddr;
};

struct server_socket get_socket();
