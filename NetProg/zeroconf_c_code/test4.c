// DNSServiceResolve example

#include <dns_sd.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()

extern void HandleEvents(DNSServiceRef);

static void
MyResolveCallBack(DNSServiceRef serviceRef,
				  DNSServiceFlags flags,
				  uint32_t interface,
				  DNSServiceErrorType errorCode,
				  const char *fullname,
				  const char *hosttarget,
				  uint16_t port,
				  uint16_t txtLen,
				  const char *txtRecord,
				  void *context)
	{
	#pragma unused(flags)
	#pragma unused(fullname)

	if (errorCode != kDNSServiceErr_NoError)
		fprintf(stderr, "MyResolveCallBack returned %d\n", errorCode);
	else
		printf("RESOLVE: %s is at %s:%d\n", fullname, hosttarget, ntohs(port));
	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}


static DNSServiceErrorType MyDNSServiceResolve()
	{
	DNSServiceErrorType error;
	DNSServiceRef  serviceRef;
	
	error = DNSServiceResolve(&serviceRef,
							   0,  // no flags
							   0,  // all network interfaces
							   "Not a real page", //name
							   "_http._tcp", // service type
							   "local", //domain
							   MyResolveCallBack,
							   NULL);	 // no context					
	
	if (error == kDNSServiceErr_NoError)
		{
		HandleEvents(serviceRef);  // Add service to runloop to get callbacks
		DNSServiceRefDeallocate(serviceRef);
		}
	return error;
	}

int main (int argc, const char * argv[])
	{
	DNSServiceErrorType error = MyDNSServiceResolve();
	fprintf(stderr, "DNSServiceDiscovery returned %d\n", error);
									//if function returns print error
	return 0;
	}
