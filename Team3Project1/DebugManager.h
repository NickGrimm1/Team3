/**
<summary>Static Singleton.
The game's debug manager. Holds debug information.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include <vector> // I imagine we'll probably need one eventually. It's just here so NULL is defined atm.

class DebugManager
{
	// TODO: Implement Debug Manager.
public:
	static bool Initialize(DebugManager*& out) 
	{
		if (instance == NULL)
			instance = new DebugManager();
		
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
	DebugManager() { }
	~DebugManager() { }
	static DebugManager* instance;
};