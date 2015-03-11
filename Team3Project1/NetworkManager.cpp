#ifdef WINDOWS_BUILD

#include "NetworkManager.h"
#include "NetworkCommon.h"
#include "ServerSocket.h"
#include "TcpSocket.h"
#include <ws2tcpip.h>
#include <iostream>

NetworkManager* NetworkManager::instance = NULL;

NetworkManager::NetworkManager() {
	hasInitialised = false;

	WSAData wsaData;
	int error = WSAStartup(MAKEWORD(HIGHVERSION, LOWVERSION), &wsaData);
	if (error != 0) {
		return;
	}

	if (LOBYTE(wsaData.wVersion) != LOWVERSION || HIBYTE(wsaData.wVersion) != HIGHVERSION) {
		WSACleanup();
		return;
	}

	hasInitialised = true;
}

NetworkManager::~NetworkManager() {
	WSACleanup();
}

ServerSocket* NetworkManager::StartServer(string portNum, unsigned int maxConnections) {
	ServerSocket* server = new ServerSocket(portNum, maxConnections);
	if (!server->HasInitialised()) {
		delete server;
		return NULL;
	}

	return server;
}

BaseSocket* NetworkManager::ConnectToServer(string ipAddress, string portNum) {
	
	struct addrinfo hints;
	struct addrinfo* addressData;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //IPv4 or IPv6
	hints.ai_socktype = TCP;
	hints.ai_flags = AI_PASSIVE; // use localhost for address
	
	unsigned int addrResult = getaddrinfo(ipAddress.c_str(), portNum.c_str(), &hints, &addressData);
	if (addrResult != 0) {
		return NULL;
	}
		
	SOCKET connectionSocket = socket(addressData->ai_family, addressData->ai_socktype, addressData->ai_protocol);
	if (connectionSocket == INVALID_SOCKET) {
		return NULL;
	}

	string connTimeout(CONNECTION_TIMEOUT_MS);
	setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, connTimeout.c_str(), connTimeout.length());

	int connResult = connect(connectionSocket, addressData->ai_addr, addressData->ai_addrlen);
	if (connResult == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAETIMEDOUT) {// error occured
		} // not worth listing error if connection timedout
		return NULL;
	}

	freeaddrinfo(addressData);

	// Connection established - return socket object
	// TCP for time being

	unsigned long nonblocking = NON_BLOCKING;
	ioctlsocket(connectionSocket, FIONBIO, &nonblocking);

	TcpSocket* tcpSocket = new TcpSocket(connectionSocket);
	connections.push_back(tcpSocket);
	return tcpSocket;
}

#endif //WINDOWS_BUILD