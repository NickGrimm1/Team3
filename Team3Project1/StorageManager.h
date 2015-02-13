/**
<summary>Static Singleton.
The game's storage manager. Allows the game to save/load files.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include <vector> // I imagine we'll probably need one eventually. It's just here so NULL is defined atm.

class StorageManager
{
	// TODO: Implement Physics Engine.
public:
	static bool Initialize(StorageManager*& out) 
	{
		if (instance == NULL)
			instance = new StorageManager();
		
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
	StorageManager() { }
	~StorageManager() { }
	static StorageManager* instance;
};