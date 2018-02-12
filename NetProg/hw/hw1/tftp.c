#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

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

static void sig_alrm(int signo)
{
    return;
}

struct server_socket get_socket() 
{
    struct server_socket servsock;
    unsigned int servaddr_len = sizeof(servsock.addr);

    if ((servsock.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) // ask for an ipv4 UDP socket 
    {
        perror("get_socket failed");                     // if we don't get it, print the error
        exit(EXIT_FAILURE);                              // and exit          
    }

    bzero(&servsock.addr, servaddr_len);

    // fill out sockaddr_in struct
    servsock.addr.sin_family      = AF_INET;           // we're using ipv4
    servsock.addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip addr to listen on
    servsock.addr.sin_port        = 0;                 // ask for a port from the kernel

    if ( bind(servsock.sockfd, (struct sockaddr *) &servsock.addr, servaddr_len) == -1) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE); 
    }

    if(getsockname(servsock.sockfd, (struct sockaddr *) &servsock.addr, &servaddr_len) == -1)
    {
        perror("getsockname failed");
        exit(EXIT_FAILURE);
    }

    return servsock;
}

void send_err(int sockfd, struct sockaddr_in cliaddr, short err_code, char *err_msg)
{
    struct error ERR;

    ERR.opcode =   htons(5);
    ERR.err_code = htons(err_code);
    strncpy(ERR.err_msg, err_msg, sizeof(ERR.err_msg));

    sendto(sockfd, (void *) &ERR, sizeof(ERR), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

void send_ack(int sockfd, struct sockaddr_in cliaddr, short block_num)
{
    struct acknowledgment ACK;

    ACK.opcode = htons(4);
    ACK.block_num = htons(block_num);

    sendto(sockfd, (void *) &ACK, sizeof(ACK), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

void handle_fopen_err(int sockfd, struct sockaddr_in cliaddr)
{
    short  err_code;
    char   *err_msg;
    
    if(errno == 13)
    {
        err_code = 2;
        err_msg  = "Access violation.";
    }
    else if(errno = 2)
    {
        err_code = 1;
        err_msg  = "File not found.";
    }
    else
    {
        err_code = 0;
        err_msg  = "fopen failed.";
    }

    send_err(sockfd, cliaddr, err_code, err_msg);
    exit(0);
}

void send_data(int sockfd, struct sockaddr_in cliaddr, char *data, int bytes_read, short block_num)
{
    struct data DATA;

    DATA.opcode = htons(3);
    DATA.block_num = htons(block_num);
    memcpy(DATA.data, data, bytes_read);

    sendto(sockfd, (void *) &DATA, (4 + bytes_read), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

void wait_for_ack(int sockfd, struct sockaddr_in cliaddr, char *data, int bytes_read, short block_num)
{
    struct acknowledgment ACK;
    unsigned int addrlen = sizeof(cliaddr);
    short recv_block_num;
    int timeout_count = 0;

    while(1) //waiting for the /correct/ acknowledgment
    {
        alarm(1);

        if(recvfrom(sockfd, (void *) &ACK, sizeof(ACK), 0, (struct sockaddr *) &cliaddr, &addrlen) == -1)
        {
            if(errno == EINTR)
            {
                timeout_count++;
                if(timeout_count < 10)
                {
                    send_data(sockfd, cliaddr, data, bytes_read, block_num);
                }
                else if(timeout_count == 10)
                { 
                    printf("timed out waiting for acknowledgement.\n");
                    exit(0);
                }
            }
            else
            {
                perror("recvfrom failed");
                exit(EXIT_FAILURE);
            }
        }

        recv_block_num = ntohs(ACK.block_num);

        if(recv_block_num == block_num) //we've recieved the ack we were waiting for
            return;
    }
}

void handle_read(int sockfd, struct sockaddr_in cliaddr, int TID, char *filename)
{
    FILE *fp;
    char data[512] = {0};
    int bytes_read;
    short block_num = 0;

    if((fp = fopen(filename, "r")) == NULL)
        handle_fopen_err(sockfd, cliaddr);

    while(feof(fp) == 0)
    {
        block_num++;
        bytes_read = fread(data, 1, 512, fp);

        send_data(sockfd, cliaddr, data, bytes_read, block_num);
        
        wait_for_ack(sockfd, cliaddr, data, bytes_read, block_num);

        if(cliaddr.sin_port != TID)
            send_err(sockfd, cliaddr, 5, "Unknown transfer ID.");
    }
}

void init_read(int sockfd, struct sockaddr_in cliaddr, char *packet)
{
    int  TID = cliaddr.sin_port;
    int  fname_len;
    char filename[512] = {0};
    char mode[20] = {0};

    strncat(filename, (packet + 2), sizeof(filename));

    fname_len = (int) strlen(filename);

    //printf("%s\n", filename);

    strncat(mode, (packet + 3 + fname_len), sizeof(mode));

    //printf("%s\n", mode);

    if(strcmp("octet", mode) != 0)
    {
        char err_msg[40];
        printf("mode: %s not supported\n", mode);
        snprintf(err_msg, sizeof(err_msg), "%s not supported", mode);
        send_err(sockfd, cliaddr, 0, err_msg);
        exit(0);
    }
    else
        handle_read(sockfd, cliaddr, TID, filename);
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

            struct server_socket newsock = get_socket();
            
            if(opcode == 1)
            {
                init_read(newsock.sockfd, cliaddr, packet);
            }
            else
            {
                handle_write(newsock.sockfd, cliaddr, packet);
            }
        }
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in   cliaddr;  // this will hold port, ip, etc info for our client
    struct server_socket servsock; // this will hold the server socket's file descriptor and sockaddr_in

    servsock = get_socket();

    printf("port: %d\n", ntohs(servsock.addr.sin_port));

    signal(SIGALRM, sig_alrm);

    handle_requests(servsock.sockfd, cliaddr, sizeof(cliaddr));
}
