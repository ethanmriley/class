// Skeleton of how to handle DNSServiceDiscovery events using a CFRunLoop

typedef struct MyDNSServiceState
	{
	DNSServiceRef		service;
	CFSocketRef			socket;
	CFRunLoopSourceRef	source;
	} MyDNSServiceState;

void HandleEvents(DNSServiceRef serviceRef)
	{
	// . . .
	// Access the underlying Unix domain socket and create a CFSocket
	sock = DNSServiceRefSockFD(ref->service);
	ref->socket = CFSocketCreateWithNative(NULL, sock,
										   kCFSocketReadCallBack,
										   MySocketCallBack, &context);
	// . . .
	// Create a CFRunLoopSource from the CFSocket, add to run loop and start.
	ref->source = CFSocketCreateRunLoopSource(NULL, ref->socket, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), ref->source, kCFRunLoopCommonModes);
	CFRunLoopRun();
	}
