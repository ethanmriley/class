#include <dns_sd.h>
#include <stdio.h>	 // For stdout, stderr
#include <string.h>	 // For strlen(), strcpy(), bzero()

extern void HandleEvents(DNSServiceRef);

static void
MyBrowseCallBack(DNSServiceRef service,
				DNSServiceFlags flags,
				uint32_t interfaceIndex,
				DNSServiceErrorType errorCode,
				const char * name,
				const char * type,
				const char * domain,
				void * context)
	{
	#pragma unused(context)
	if (errorCode != kDNSServiceErr_NoError)
		fprintf(stderr, "MyBrowseCallBack returned %d\n", errorCode);
	else
		{
		char *addString  = (flags & kDNSServiceFlagsAdd) ? "ADD" : "REMOVE";
		char *moreString = (flags & kDNSServiceFlagsMoreComing) ? "MORE" : "    ";
		printf("%-7s%-5s %d%s.%s%s\n",
			addString, moreString, interfaceIndex, name, type, domain);
		}
	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static DNSServiceErrorType
MyDNSServiceBrowse()
	{
	DNSServiceErrorType error;
	DNSServiceRef  serviceRef;
	
	error = DNSServiceBrowse(&serviceRef,
							0,                // no flags
							0,                // all network interfaces
							"_http._tcp",     // service type
							"",               // default domains
							MyBrowseCallBack, // call back function
							NULL);            // no context
	if (error == kDNSServiceErr_NoError)
		{
		HandleEvents(serviceRef); // Add service to runloop to get callbacks
		DNSServiceRefDeallocate(serviceRef);
		}

	return error;
	}

int main (int argc, const char * argv[])
	{
	DNSServiceErrorType error = MyDNSServiceBrowse();
	if (error) fprintf(stderr, "DNSServiceDiscovery returned %d\n", error);
	return 0;
	}
