#include <dns_sd.h>
#include <sys/time.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

struct sockaddr_in get_socket()
    {
    struct sockaddr_in rps_sock;
    unsigned int rps_sock_len = sizeof(rps_sock);
    int servfd;
    if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // ask for an ipv4 TCP socket 
        {
        perror("get_socket failed");
        exit(EXIT_FAILURE);
        }
    
    bzero(&rps_sock, rps_sock_len);

    rps_sock.sin_family      = AF_INET;
    rps_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    rps_sock.sin_port        = 0;
    
    if (bind(servfd, (struct sockaddr*)&rps_sock, rps_sock_len) == -1)
        {
        perror("bind failed");
        exit(EXIT_FAILURE);
        }

    if (getsockname(servfd, (struct sockaddr*)&rps_sock, &rps_sock_len) == -1)
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

int main (int argc, const char * argv[])
	{
    struct sockaddr_in rps_sock = get_socket();

    DNSServiceErrorType error;
    DNSServiceRef serviceRef;
	error = DNSServiceRegister(&serviceRef,
								0,                  // no flags
								0,                  // all network interfaces
								"riley4",  // name
								"_rps._tcp",       // service type
								"",                 // register in default domain(s)
								NULL,               // use default host name
								rps_sock.sin_port,        // port number
								0,                  // length of TXT record
								NULL,               // no TXT record
								MyRegisterCallBack, // call back function
								NULL);              // no context
    printf("DNSServiceRegister returned %d\n", error);

    int rpsfd = DNSServiceRefSockFD(serviceRef);
    int nfds = rpsfd + 1;
    fd_set readfds;
    struct timeval tv;
    int result;

    while(1) 
        {
        FD_ZERO(&readfds);
        FD_SET(rpsfd, &readfds);
        tv.tv_sec = 1000000;
        tv.tv_usec = 0;

        result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
        if (result > 0) 
            {
            DNSServiceErrorType err = kDNSServiceErr_NoError;
            if (FD_ISSET(rpsfd, &readfds))
                {
                err = DNSServiceProcessResult(serviceRef);
                }
            }
        else 
            {
            printf("select() returned %d\n", result);
            }
        }

	return 0;
	}
