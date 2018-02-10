#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 100

//tftp server for netprog
//ethan riley, 2017

//--------------
//packet structs
//--------------

//rrq/wrq have the same structure
struct read_write_request { 
    int  opcode:16;     //bit field ensures 16 bits
    char filename[256]; //assuming this is max filename length
    char mode[6];       //always going to be "octet"
};

struct data {
    int  opcode:16;
    int  block_num:16;
    char data[512];
};

struct acknowledgment {
    int opcode:16;
    int block_num:16;
};

struct error {
    int  opcode:16;
    int  err_code:16;
    char err_msg[40];
};


//---------
//functions
//---------


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
        exit(EXIT_FAILURE);                          // and exit          
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
