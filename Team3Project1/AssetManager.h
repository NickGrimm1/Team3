/**
<summary>Static Singleton.
The game's asset manager. Loads & Unloads assets.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include <vector> // I imagine we'll probably need one eventually. It's just here so NULL is defined atm.

class AssetManager
{
	// TODO: Implement Asset Manager.
public:
	static bool Initialize(AssetManager*& out) 
	{ 
		if (instance == NULL)
			instance = new AssetManager();

		out = instance;
		return true; 
	}
	static bool Destroy()
	{
		if (instance != NULL)
			delete instance;
		instance = NULL;
	}
private:
	AssetManager()
	{ }
	~AssetManager()
	{ }

	static AssetManager* instance;
};