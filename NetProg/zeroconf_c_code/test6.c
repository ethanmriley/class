// DNSServiceEnumerateDomains example

#include <dns_sd.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()

extern void HandleEvents(DNSServiceRef);

static void
MyEnumerateBrowseDomainsCallBack(DNSServiceRef sdRef,
								 DNSServiceFlags flags,
								 uint32_t interface,
								 DNSServiceErrorType errorCode,
								 const char *replyDomain,
								 void *context)
	{
	#pragma unused(context)

	if (errorCode != kDNSServiceErr_NoError)
		fprintf(stderr, "MyEnumerateBrowseDomainsCallBack returned %d\n", errorCode);
	else
		{
		char *moreString = (flags & kDNSServiceFlagsMoreComing) ? "MORE" : "";
		char *addString  = "REMOVE";
		if (flags & kDNSServiceFlagsAdd)
			addString = (flags & kDNSServiceFlagsDefault) ? "DEFAULT" : "ADD";
		printf("%-8s%-5s%s\n", addString, moreString, replyDomain);
		}

	if (!(flags & kDNSServiceFlagsMoreComing)) fflush(stdout);
	}

static DNSServiceErrorType MyDNSServiceEnumerateBrowse()
	{
	DNSServiceErrorType error;
	DNSServiceRef  serviceRef;
	
	error = DNSServiceEnumerateDomains(
					  &serviceRef,
					  kDNSServiceFlagsBrowseDomains,  // browse domains
					  0,  // all network interfaces
					  MyEnumerateBrowseDomainsCallBack, //callback function
					  NULL);	// no context
	
	if (error == kDNSServiceErr_NoError)
		{
		HandleEvents(serviceRef);  // Add service to runloop to get callbacks
		DNSServiceRefDeallocate(serviceRef);
		}
	return error;
	}

int main (int argc, const char * argv[])
	{
	DNSServiceErrorType error = MyDNSServiceEnumerateBrowse();
	if (error) fprintf(stderr, "DNSServiceDiscovery returned %d\n", error);
	return 0;
	}
