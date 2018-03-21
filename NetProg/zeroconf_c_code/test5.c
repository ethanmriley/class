// DNSServiceRegister example

#include <dns_sd.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()

extern void HandleEvents(DNSServiceRef);

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

static DNSServiceErrorType MyDNSServiceRegister()
	{
	DNSServiceErrorType error;
	DNSServiceRef serviceRef;
	
	error = DNSServiceRegister(&serviceRef,
								0,                  // no flags
								0,                  // all network interfaces
								"Not a real page",  // name
								"_http._tcp",       // service type
								"",                 // register in default domain(s)
								NULL,               // use default host name
								htons(9092),        // port number
								0,                  // length of TXT record
								NULL,               // no TXT record
								MyRegisterCallBack, // call back function
								NULL);              // no context
	
	if (error == kDNSServiceErr_NoError)
		{
		HandleEvents(serviceRef);
		DNSServiceRefDeallocate(serviceRef);
		}
	
	return error;
	}

int main (int argc, const char * argv[])
	{
	DNSServiceErrorType error = MyDNSServiceRegister();
	fprintf(stderr, "DNSServiceDiscovery returned %d\n", error);
	return 0;
	}
