#ifdef WINDOWS_BUILD

#include "NetworkManager.h"
#include "NetworkCommon.h"
#include "ServerSocket.h"
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




#endif //WINDOWS_BUILD