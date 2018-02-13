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

//data, plus an int to pass around the length of the data in data[]
struct data {
    short opcode;
    short block_num;
    char  data[512];
    int   byte_count;
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

//wrapper for sockaddr_in to make get_socket() easier to use
struct server_socket {
    int sockfd;
    struct sockaddr_in addr;
};

//---------
//functions
//---------

//SIGALRM handler for timeouts
static void sig_alrm(int signo)
{
    return;
}

//get a socket on a random available port and return the info in a server_socket struct
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

//easy wrapper for sending error messages
void send_err(int sockfd, struct sockaddr_in cliaddr, short err_code, char *err_msg)
{
    struct error ERR;

    ERR.opcode =   htons(5);
    ERR.err_code = htons(err_code);
    strncpy(ERR.err_msg, err_msg, sizeof(ERR.err_msg));

    sendto(sockfd, (void *) &ERR, sizeof(ERR), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

//wrapper for sending acknowledgments
void send_ack(int sockfd, struct sockaddr_in cliaddr, short block_num)
{
    struct acknowledgment ACK;

    ACK.opcode = htons(4);
    ACK.block_num = htons(block_num);

    sendto(sockfd, (void *) &ACK, sizeof(ACK), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

//distinguish what failed in fopen and send the correct err_code
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

void Sendto(int sockfd, void *packet, int len, int flags, struct sockaddr *cliaddr, unsigned int cliaddr_len)
{
    if(sendto(sockfd, packet, len, flags, cliaddr, cliaddr_len) == -1)
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
}

//wrapper to send data in a data packet
void send_data(int sockfd, struct sockaddr_in cliaddr, char *data, int bytes_read, short block_num)
{
    struct data DATA;
    struct acknowledgment ACK;
    unsigned int cliaddr_len = sizeof(cliaddr);
    int timeout_count = 0;

    DATA.opcode = htons(3);
    DATA.block_num = htons(block_num);
    memcpy(DATA.data, data, bytes_read);

    Sendto(sockfd, (void *) &DATA, (4 + bytes_read), 0, (struct sockaddr *) &cliaddr, cliaddr_len);

    while(1)
    {
        alarm(1);

        //if /any/ packet arrives on our port, we continue past this
        if(recvfrom(sockfd, (void *) &ACK, sizeof(ACK), 0, (struct sockaddr *) &cliaddr, &cliaddr_len) == -1)
        {
            if(errno == EINTR)
            {
                timeout_count++;
                if(timeout_count < 10) //while it's been less than 10 seconds, we retransmit the data packet
                {
                    Sendto(sockfd, (void *) &DATA, (4 + bytes_read), 0, (struct sockaddr *) &cliaddr, cliaddr_len);
                }
                else if(timeout_count == 10) //once it's been 10 seconds, we timeout and close the connection by killing the process
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

        alarm(0);

        if(ACK.block_num == htons(block_num))
            return; 
    }
}

//handles read operations by opening files, sending data, and receiving acknowledgments
void handle_read(int sockfd, struct sockaddr_in cliaddr, char *filename)
{
    FILE *fp;
    char data[512] = {0};
    int bytes_read;
    int TID = cliaddr.sin_port;
    short block_num = 0;

    if((fp = fopen(filename, "r")) == NULL)
        handle_fopen_err(sockfd, cliaddr);

    while(feof(fp) == 0)
    {
        block_num++;
        bytes_read = fread(data, 1, 512, fp);

        send_data(sockfd, cliaddr, data, bytes_read, block_num);

        if(cliaddr.sin_port != TID)
            send_err(sockfd, cliaddr, 5, "Unknown transfer ID.");
    }
}

//essentially identical to wait_for_ack, but it calls send_ack instead of send_data
struct data wait_for_data(int sockfd, struct sockaddr_in cliaddr, short block_num)
{
    struct data DATA;
    int byte_count;
    int timeout_count = 0;
    unsigned int cliaddr_len = sizeof(cliaddr);

    while(1)
    {
        alarm(1);

        byte_count = recvfrom(sockfd, &DATA, sizeof(DATA), 0, (struct sockaddr *) &cliaddr, &cliaddr_len);
        if(byte_count == -1)
        {
            if(errno == EINTR)
            {
                if(timeout_count < 10)
                {
                    send_ack(sockfd, cliaddr, block_num);
                    timeout_count++;
                }
                else if(timeout_count == 10)
                {
                    printf("socket timed out while waiting for data\n");
                    exit(0);
                }
            }
            else
            {
                perror("recvfrom failed");
                exit(EXIT_FAILURE);
            }
        }
        
        DATA.byte_count = byte_count - 4;

        if(DATA.block_num == htons(block_num))
        {
            return DATA;
        } 
    }
}

//function to handle write operations by opening files, receving data, and sending acknowledgments
void handle_write(int sockfd, struct sockaddr_in cliaddr, char *filename)
{
    FILE   *fp;
    char   data[512] = {0};
    short  block_num = 0;
    struct data DATA;

    if(access(filename, F_OK) == 0) //check if the file we're trying to write already exists
    {
        send_err(sockfd, cliaddr, 6, "File already exists.");
        exit(0);
    }

    fp = fopen(filename, "w");
    if(fp == NULL)
        handle_fopen_err(sockfd, cliaddr);

    send_ack(sockfd, cliaddr, block_num);

    while(1)
    {
        block_num++;
        DATA = wait_for_data(sockfd, cliaddr, block_num);
        send_ack(sockfd, cliaddr, block_num);
        memcpy(&data, DATA.data, DATA.byte_count);
        fwrite(&data, 1, DATA.byte_count, fp);

        if(DATA.byte_count < 512)
        {
            fclose(fp);
            exit(0);
        }
    }
}

//handle both read and write requests. this is called on any incoming initial packet
void handle_request(int sockfd, struct sockaddr_in cliaddr, socklen_t cliaddr_len)
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

        opcode = ntohs(*((short *) packet)); //assuming well-formatted packets every time

        if((opcode == 1 || opcode == 2) && fork() == 0) //fork on read and write requests
        {
            struct server_socket newsock = get_socket();
            int  fname_len;
            char filename[512] = {0};
            char mode[20] = {0};

            strncat(filename, (packet + 2), sizeof(filename));
            fname_len = (int) strlen(filename);
            strncat(mode, (packet + 3 + fname_len), sizeof(mode));

            if(strcmp("octet", mode) != 0)
            {
                char err_msg[40];
                snprintf(err_msg, sizeof(err_msg), "%s not supported", mode);
                send_err(sockfd, cliaddr, 0, err_msg);
                exit(0);
            }
            else
                if(opcode == 1)
                    handle_read(newsock.sockfd, cliaddr, filename);
                else
                    handle_write(newsock.sockfd, cliaddr, filename);
        }
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in   cliaddr;  // this will hold port, ip, etc info for our client
    struct server_socket servsock; // this will hold the server socket's file descriptor and sockaddr_in
    struct sigaction act;

    act.sa_handler = sig_alrm;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);

    servsock = get_socket();

    printf("%d\n", ntohs(servsock.addr.sin_port));

    handle_request(servsock.sockfd, cliaddr, sizeof(cliaddr));
}
