#pragma once
#ifdef WINDOWS_BUILD

#include "NetworkCommon.h"
#include <WinSock2.h>
#include <vector>

using namespace std;

class BaseSocket;

class ServerSocket
{
public:
	ServerSocket(string port, unsigned int maxConnections = 4); // listen on port num
	virtual ~ServerSocket(void);

	unsigned int GetNewConnections();

	bool HasInitialised() {return hasInitialised;}
	unsigned int GetNumberConnections() const {return numConnections;}
	BaseSocket* GetConnection(unsigned int i);


protected:
	bool hasInitialised;
	string portNum;
	unsigned int numConnections;
	unsigned int maxConnections;
	struct addrinfo* addressData;
	SOCKET serverSocket;
	vector<BaseSocket*> connections;
};

#endif