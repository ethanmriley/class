#include "network.h"

struct server_socket get_socket() {
    struct server_socket serv;
    unsigned int sockaddr_len = sizeof(serv.sockaddr);
    if ((serv.sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { // ask for an ipv4 TCP socket 
        perror("get_socket failed");
        exit(EXIT_FAILURE);
    }
    
    bzero(&serv.sockaddr, sockaddr_len);

    serv.sockaddr.sin_family      = AF_INET;
    serv.sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sockaddr.sin_port        = 0; //ask for a port from the OS
    
    if (bind(serv.sockfd, (struct sockaddr*)&serv.sockaddr, sockaddr_len) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //fill in serv.sockaddr.sin_port with the port the OS assigned us
    if (getsockname(serv.sockfd, (struct sockaddr*)&serv.sockaddr, &sockaddr_len) == -1) {
        perror("getsockname failed");
        exit(EXIT_FAILURE);
    }

    return serv;
}

int Send(int sockfd, const void *buf, unsigned int len, int flags) {
    int err;

    err = send(sockfd, buf, len, flags);
    if(err == -1) {
        perror("send failed");
        return -1;
    }
    
    return err;
}

int Accept(int sockfd, struct sockaddr *addr, unsigned int* addrlen) {
    int client_sock;

    client_sock = accept(sockfd, addr, addrlen);
    if (client_sock == -1) {
        perror("accept failed");
        return 1;
    }

    return client_sock;
}

int Recv(int sockfd, void *buf, unsigned int len, int flags) {
    int bytes_recv;

    bytes_recv = recv(sockfd, buf, len, flags);
    if(bytes_recv == -1) {
        perror("recv failed");
        return -1;
    }

    return bytes_recv;
}
