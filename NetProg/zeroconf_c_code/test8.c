// Simple example of how to handle DNSServiceDiscovery events using a CFRunLoop

#include <dns_sd.h>
#include <CoreFoundation/CoreFoundation.h>


// Structure to hold CFRunLoop-related state

typedef struct MyDNSServiceState
    {
    DNSServiceRef       service;    // Active DNSServiceDiscovery operation
    CFSocketRef         cfsocket;   // CFSocket for this operation
    CFRunLoopSourceRef  source;     // RunLoopSource for this CFSocket
    } MyDNSServiceState;


// Remove a DNSServiceDiscovery operation from a CFRunLoop's
// set of active event sources

static void DNSServiceRemoveSource(CFRunLoopRef rl, MyDNSServiceState *ref)
    {
    assert(rl != NULL);
    assert(ref != NULL);

    // Remove the CFRunLoopSource from the current run loop.
    CFRunLoopRemoveSource(rl, ref->source, kCFRunLoopCommonModes);
    CFRelease(ref->source);

    // Invalidate the CFSocket.
    CFSocketInvalidate(ref->cfsocket);
    CFRelease(ref->cfsocket);

    // Workaround to give time to CFSocket's select() thread
    // so it can remove the socket from its FD set before we
    // close the socket by calling DNSServiceRefDeallocate.
    usleep(1000);

    // Terminate the connection with the daemon, which cancels the operation.
    DNSServiceRefDeallocate(ref->service);
    free(ref);
    }


// Helper function: When CFRunLoop indicates an interesting event,
// this function calls DNSServiceProcessResult() to handle it

static void MySocketCallBack(CFSocketRef s, CFSocketCallBackType type,
                 CFDataRef address, const void *data, void *info)
    {
    #pragma unused(s)
    #pragma unused(type)
    #pragma unused(address)
    #pragma unused(data)

    DNSServiceErrorType err;
    MyDNSServiceState *ref = (MyDNSServiceState *)info;
    assert(ref != NULL);

    // Read a reply from the daemon, which will call the appropriate callback.
    err= DNSServiceProcessResult(ref->service);
    if (err != kDNSServiceErr_NoError)
        {
        fprintf(stderr, "DNSServiceProcessResult returned %d\n", err);
        // Terminate the discovery operation and release everything.
        DNSServiceRemoveSource(CFRunLoopGetCurrent(), ref);
        }
    }


// Add a DNSServiceDiscovery operation to a CFRunLoop's
// set of active event sources

static void DNSServiceAddSource(CFRunLoopRef rl, MyDNSServiceState *ref)
    {
    CFSocketContext context = { 0, ref, NULL, NULL, NULL };
    CFSocketNativeHandle sock = DNSServiceRefSockFD(ref->service);
    assert(sock != -1);

    // Create a CFSocket from the underlying Unix Domain socket.
    ref->cfsocket = CFSocketCreateWithNative(NULL, sock,
                    kCFSocketReadCallBack, MySocketCallBack, &context);

    // Prevent CFSocketInvalidate from closing DNSServiceRef's socket.
    CFOptionFlags f = CFSocketGetSocketFlags(ref->cfsocket);
    CFSocketSetSocketFlags(ref->cfsocket, f & ~kCFSocketCloseOnInvalidate);

    // Create a CFRunLoopSource from the CFSocket.
    ref->source = CFSocketCreateRunLoopSource(NULL, ref->cfsocket, 0);

    // Add the CFRunLoopSource to the current run loop.
    CFRunLoopAddSource(rl, ref->source, kCFRunLoopCommonModes);
    }


// Simple example: Here we just add a single DNSServiceDiscovery event source,
// and then call CFRunLoopRun() to handle the events. In a program that already
// has a main RunLoop, you'd just keep that as is, and use DNSServiceAddSource/
// DNSServiceRemoveSource to add and remove event sources from that RunLoop.

void HandleEvents(DNSServiceRef serviceRef)
    {
    MyDNSServiceState ref = { serviceRef };
    DNSServiceAddSource(CFRunLoopGetCurrent(), &ref);

    CFRunLoopRun();
    }
