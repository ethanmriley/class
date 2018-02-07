#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define MAXLINE 100

//re-implementation of dg_echo
void my_echo(int sockfd, struct sockaddr *cliaddr, socklen_t cliaddr_len)
{
    int mesg_len;
    char mesg[MAXLINE];

    while(1) {
        mesg_len = recvfrom(sockfd, mesg, MAXLINE, 0, cliaddr, (socklen_t *) &cliaddr_len);
        
        if(mesg_len == -1)
        {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        
        sendto(sockfd, mesg, mesg_len, 0, cliaddr, cliaddr_len); 
    }
}

int main(int argc, char **argv)
{
    int                sockfd;            // file descriptor for our socket
    struct sockaddr_in servaddr, cliaddr; // these will hold our server and client info (port, ip, etc)
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) // ask for an ipv4 UDP socket 
    {
        perror("socket failed");                     // if we don't get it, print the error
        exit(EXIT_FAILURE);                            // and exit          
    }

    bzero(&servaddr, sizeof(servaddr)); // zero out servaddr
    
    // fill out servaddr struct
    servaddr.sin_family      = AF_INET;           // we're using ipv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // big-endian 32-bit (network long) ip addr to listen on
    servaddr.sin_port        = htons(1337);       // big-endian 16-bit (network short) port # to listen on

    if ( bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE); 
    }

    my_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}
