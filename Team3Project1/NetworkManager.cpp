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
		cout << "WSAStartup failed with error: " << error << endl;
		return;
	}

	if (LOBYTE(wsaData.wVersion) != LOWVERSION || HIBYTE(wsaData.wVersion) != HIGHVERSION) {
		printf("The version requested cannot be supported.\n");
		printf("Version set is %d.%d\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
		WSACleanup();
		return;
	}
	
	cout << "The Winsock API has been successfully initialised." << endl;
	cout << "You are using version " << HIBYTE(wsaData.wVersion) << "." << LOBYTE(wsaData.wVersion) << endl;
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
	
	unsigned int addrResult = getaddrinfo(LOCALHOST, portNum.c_str(), &hints, &addressData);
	if (addrResult != 0) {
		cout << "getaddrinfo() failed with error:" << endl;
		cout << addrResult << ": " << gai_strerror(WSAGetLastError()) << endl;
		return NULL;
	}
		
	SOCKET connectionSocket = socket(addressData->ai_family, addressData->ai_socktype, addressData->ai_protocol);
	if (connectionSocket == INVALID_SOCKET) {
		cout << "Could not create socket for address " << ipAddress << " on port " << portNum << endl;
		cout << "Failed with error " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
		return NULL;
	}

	string connTimeout(CONNECTION_TIMEOUT_MS);
	setsockopt(connectionSocket, SOL_SOCKET, SO_SNDTIMEO, connTimeout.c_str(), connTimeout.length());

	int connResult = connect(connectionSocket, addressData->ai_addr, addressData->ai_addrlen);
	if (connResult == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAETIMEDOUT) {// error occured
			cout << "An error occurred connecting with port " << portNum << " at " << ipAddress << endl;
			cout << "Failed with error " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
		} // not worth listing error if connection timedout
		return NULL;
	}

	// Connection established - return socket object
	// TCP for time being

	TcpSocket* tcpSocket = new TcpSocket(connectionSocket);
	connections.push_back(tcpSocket);
	return tcpSocket;
}

#endif //WINDOWS_BUILD