/**
<summary>Static Singleton.
The games input manager.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include "Thread.h"

class InputManager : public Thread
{
	// TODO: Implement Input Manager.
public:
	static bool Initialize(InputManager*& out) 
	{
		if (instance == NULL)
			instance = new InputManager();
		
		out = instance;
		return true; 
	}
	static void Destroy()
	{
		if (instance != NULL)
			delete instance;
		instance = NULL;
	}

	void Run();
private:
	InputManager() { }
	~InputManager() { }
	static InputManager* instance;
};