#include <dns_sd.h>
#include <sys/time.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

struct server_socket 
    {
    int sockfd;
    struct sockaddr_in sockaddr;
    };

struct server_socket get_socket()
    {
    struct server_socket rps_sock;
    unsigned int sockaddr_len = sizeof(rps_sock.sockaddr);
    if ((rps_sock.sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // ask for an ipv4 TCP socket 
        {
        perror("get_socket failed");
        exit(EXIT_FAILURE);
        }
    
    bzero(&rps_sock.sockaddr, sockaddr_len);

    rps_sock.sockaddr.sin_family      = AF_INET;
    rps_sock.sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    rps_sock.sockaddr.sin_port        = 0;
    
    if (bind(rps_sock.sockfd, (struct sockaddr*)&rps_sock.sockaddr, sockaddr_len) == -1)
        {
        perror("bind failed");
        exit(EXIT_FAILURE);
        }

    if (getsockname(rps_sock.sockfd, (struct sockaddr*)&rps_sock.sockaddr, &sockaddr_len) == -1)
        {
        perror("getsockname failed");
        exit(EXIT_FAILURE);
        }


    return rps_sock;
    }

static void
MyRegisterCallBack(DNSServiceRef service,
				   DNSServiceFlags flags,
				   DNSServiceErrorType errorCode,
				   const char * name,
				   const char * type,
				   const char * domain,
				   void * context)
	{
	#pragma unused(flags)
	#pragma unused(context)

	if (errorCode != kDNSServiceErr_NoError)
		fprintf(stderr, "MyRegisterCallBack returned %d\n", errorCode);
	else
		printf("%-15s %s.%s%s\n","REGISTER", name, type, domain);
	}

DNSServiceRef advertiseService(short port) 
    {
    DNSServiceErrorType error;
    DNSServiceRef serviceRef;
	error = DNSServiceRegister(&serviceRef,
								0,                  // no flags
								0,                  // all network interfaces
								"rileye4",          // name
								"_rps._tcp",        // service type
								"",                 // register in default domain(s)
								NULL,               // use default host name
								port,               // port number
								0,                  // length of TXT record
								NULL,               // no TXT record
								MyRegisterCallBack, // call back function
								NULL);              // no context
    if (error != 0) 
        {
        perror("DNSServiceRegister failed");
        exit(EXIT_FAILURE);
        }
    return serviceRef;
    }

void new_connection(int servfd)
    {

    signal(SIGPIPE, SIG_IGN);

    struct sockaddr_in client;
    int cliaddr_len = sizeof(client);
    int new_sock = accept(servfd, (struct sockaddr*)&client, &cliaddr_len);
    int err;

    while(1)//echo infinitely TODO change to select loop
        {
        char msg[40];
        bzero(&msg, sizeof(msg));
        recv(new_sock, msg, sizeof(msg), 0);
        //printf("recv: %s", msg);
        err = send(new_sock, &msg, sizeof(msg), 0);
        if (err == -1)
            break;
        //printf("send\n");
        }    
    printf("it doesn't die\n");
    }

int main (int argc, const char * argv[])
	{

    int daemonfd;
    int nfds;
    int result;
    struct timeval tv;
    struct server_socket rps_sock;
    DNSServiceRef serviceRef;
    fd_set readfds;
    
    rps_sock = get_socket(); 
    serviceRef = advertiseService(rps_sock.sockaddr.sin_port);

    daemonfd = DNSServiceRefSockFD(serviceRef);
    nfds = daemonfd + 1;

    while(1) 
        {
        FD_ZERO(&readfds);
        FD_SET(daemonfd, &readfds);
        FD_SET(rps_sock.sockfd, &readfds);
        tv.tv_sec = 1000000;
        tv.tv_usec = 0;

        listen(rps_sock.sockfd, 20);

        result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
        if (result > 0) 
            {
            if (FD_ISSET(daemonfd, &readfds))
                DNSServiceProcessResult(serviceRef);
            
            if (FD_ISSET(rps_sock.sockfd, &readfds))
                new_connection(rps_sock.sockfd);
            }
        else 
            perror("select failed"); 
        }

	return 0;
	}
