/**
<summary>Static Singleton.
The game's network manager.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include <vector> // I imagine we'll probably need one eventually. It's just here so NULL is defined atm.

class NetworkManager
{
	// TODO: Implement Network Manager.
public:
	static bool Initialize(NetworkManager*& out) 
	{
		if (instance == NULL)
			instance = new NetworkManager();
		
		out = instance;
		return true; 
	}
	static void Destroy()
	{
		if (instance != NULL)
			delete instance;
		instance = NULL;
	}
private:
	NetworkManager() { }
	~NetworkManager() { }
	static NetworkManager* instance;
};