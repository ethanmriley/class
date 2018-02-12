#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXPACKET 1000

//tftp server for netprog
//ethan riley, 2017

//--------------
//packet structs
//--------------


struct data {
    short opcode;
    short block_num;
    char  data[512];
};

struct acknowledgment {
    short opcode;
    short block_num;
};

struct error {
    short opcode;
    short err_code;
    char  err_msg[40];
};

struct server_socket {
    int sockfd;
    struct sockaddr_in addr;
};

//---------
//functions
//---------

void send_ack(int sockfd, struct sockaddr_in cliaddr, short block_num)
{
    struct acknowledgment ACK;

    ACK.opcode = htons(4);
    ACK.block_num = htons(block_num);

    sendto(sockfd, (void *) &ACK, 4, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

void handle_read(int sockfd, struct sockaddr_in cliaddr, char *packet)
{
}

void handle_write(int sockfd, struct sockaddr_in cliaddr, char *packet)
{
    send_ack(sockfd, cliaddr, 0);
}

void handle_requests(int sockfd, struct sockaddr_in cliaddr, socklen_t cliaddr_len)
{

    int   mesg_len;
    char  packet[MAXPACKET];
    short opcode;

    while(1)
    {
        printf("waiting\n");
        mesg_len = recvfrom(sockfd, &packet, MAXPACKET, 0, (struct sockaddr *) &cliaddr, &cliaddr_len);

        if(mesg_len == -1)
        {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        opcode = ntohs(*((short *) packet));

        if((opcode == 1 || opcode == 2) && fork() == 0) //fork on read and write requests
        {
            printf("forked\n");
            
            if(opcode == 1)
            {
                handle_read(sockfd, cliaddr, packet);
            }
            else if(opcode == 2)
            {
                handle_write(sockfd, cliaddr, packet);
            }
        }
    }
}

struct server_socket get_socket() 
{
    struct server_socket servsock;

    if ((servsock.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) // ask for an ipv4 UDP socket 
    {
        perror("get_socket failed");                     // if we don't get it, print the error
        exit(EXIT_FAILURE);                              // and exit          
    }

    bzero(&servsock.addr, sizeof(servsock.addr));

    // fill out sockaddr_in struct
    servsock.addr.sin_family      = AF_INET;           // we're using ipv4
    servsock.addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip addr to listen on
    servsock.addr.sin_port        = 0;                 // ask for a port from the kernel

    if ( bind(servsock.sockfd, (struct sockaddr *) &servsock.addr, sizeof(servsock.addr)) == -1) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE); 
    }

    return servsock;
}

int main(int argc, char **argv)
{
    unsigned int servaddr_len;
    struct sockaddr_in   cliaddr;  // this will hold port, ip, etc info for our client
    struct server_socket servsock; // this will hold the server socket's file descriptor and sockaddr_in

    servsock = get_socket();

    servaddr_len = sizeof(servsock.addr);

    if(getsockname(servsock.sockfd, (struct sockaddr *) &servsock.addr, &servaddr_len) == -1)
    {
        perror("getsockname failed");
        exit(EXIT_FAILURE);
    }

    printf("port: %d\n", ntohs(servsock.addr.sin_port));

    handle_requests(servsock.sockfd, cliaddr, sizeof(cliaddr));
}
