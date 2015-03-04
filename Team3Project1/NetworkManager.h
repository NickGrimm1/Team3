/**
<summary>Static Singleton.
The game's network manager.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once

#ifdef WINDOWS_BUILD

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")

#include <vector> // I imagine we'll probably need one eventually. It's just here so NULL is defined atm.
#include <string>

using namespace std;

class BaseSocket;
class ServerSocket;
class ClientSocket;

class NetworkManager
{
	// TODO: Implement Network Manager.
public:
	static bool Initialize(NetworkManager*& out) 
	{
		if (instance != NULL)
			return instance->HasInitialised();
		
		instance = new NetworkManager();
				
		out = instance;
		return instance->HasInitialised(); 
	}
	static void Destroy()
	{
		if (instance != NULL) {
			delete instance;
			instance = NULL;
		}
	}

	bool HasInitialised() const {return hasInitialised;}

	ServerSocket* StartServer(string portNum, unsigned int maxConnections = 4);
	BaseSocket* ConnectToServer(string ipAddress, string portNum);

private:
	NetworkManager();
	~NetworkManager();
	static NetworkManager* instance;
	bool hasInitialised;
	vector<BaseSocket*> connections;
};

#endif