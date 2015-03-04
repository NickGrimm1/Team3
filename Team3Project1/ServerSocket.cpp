#ifdef WINDOWS_BUILD
#include "ServerSocket.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#include "TcpSocket.h"

using namespace std;

ServerSocket::ServerSocket(string portNum, unsigned int maxConnections) : portNum(portNum), maxConnections(maxConnections)
{
	hasInitialised = false;
	serverSocket = NULL;

	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //IPv4 or IPv6
	hints.ai_socktype = TCP;
	hints.ai_flags = AI_PASSIVE; // use localhost for address
	
	unsigned int addrResult = getaddrinfo("127.0.0.1", portNum.c_str(), &hints, &addressData);
	if (addrResult != 0) {
		cout << "getaddrinfo() failed with error:" << endl;
		cout << addrResult << ": " << gai_strerror(WSAGetLastError()) << endl;
		return;
	}
		
	serverSocket = socket(addressData->ai_family, addressData->ai_socktype, addressData->ai_protocol);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Could not create server socket on port " << portNum << endl;
		cout << "Failed with error " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
		return;
	}

	// Make socket non-blocking
	unsigned long nonblocking = NON_BLOCKING;
	ioctlsocket(serverSocket, FIONBIO, &nonblocking);

	if (bind(serverSocket, addressData->ai_addr, addressData->ai_addrlen) != 0) {
		cout << "Could not bind server socket on port " << portNum << endl;
		cout << "Failed with error " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
		return;
	}

	freeaddrinfo(addressData);

	if (listen(serverSocket, BACKLOG) != 0) {
		cout << "Can not listen on server socket on port " << portNum << endl;
		cout << "Failed with error " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
		return;
	}

	hasInitialised = true;
}

ServerSocket::~ServerSocket(void)
{
	closesocket(serverSocket);
}

unsigned int ServerSocket::GetNewConnections() {
	unsigned int connectionsCount = 0;
	
	while(true) {
		SOCKET incomingConnection = NULL;
		struct sockaddr_storage incomingAddr;
		socklen_t addrSize = sizeof(incomingAddr);

		// accept new connection from a client
		incomingConnection = accept(serverSocket, (struct sockaddr *) &incomingAddr, &addrSize);
		if (incomingConnection == INVALID_SOCKET) {
			if (WSAGetLastError() != WSAEWOULDBLOCK) { // Ignore error if non-blocking and no connections exist to accept, its really an error
				cout << "Failed to accept new connection" << endl;
				cout << "Failed with error: " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
			}
			return connectionsCount;
		}

		// new connection accepted
		// Assume TCP for now
		connections.push_back(new TcpSocket(incomingConnection));
		connectionsCount++;
		numConnections++;
	}
}

BaseSocket* ServerSocket::GetConnection(unsigned int i) {
	return connections[i];
}

#endif // WINDOWS_BUILD