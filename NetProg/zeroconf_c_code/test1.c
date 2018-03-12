// Skeleton of how to handle DNSServiceDiscovery events using a select() loop

#include "dns_sd.h"
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <errno.h>          // For errno, EINTR
#include <time.h>
#include <sys/time.h>		// For struct timeval
#include <unistd.h>         // For getopt() and optind
#include <arpa/inet.h>		// For inet_addr()

int stopNow = 0;
DNSServiceErrorType err;

void HandleEvents(DNSServiceRef serviceRef)
	{
	int dns_sd_fd = DNSServiceRefSockFD(serviceRef);
	int nfds = dns_sd_fd + 1;
	fd_set readfds;
	struct timeval tv;
	// . . .
	while (!stopNow)
		{
		// . . .
		int result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
		if (result > 0)
			{
			// . . .
			if (FD_ISSET(dns_sd_fd, &readfds))
				err = DNSServiceProcessResult(serviceRef);
			if (err) stopNow = 1;
			}		
		}
	}

main()
	{
	DNSServiceRef ref;
	HandleEvents(ref);
	}
