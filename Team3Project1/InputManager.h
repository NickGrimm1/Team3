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

	int GetFrameRate() { return frameRate; }
private:
	InputManager() 
		: INPUT_TIME(1000.0f / 60)
	{ 
		frameRate = 0;
	}
	~InputManager() { }
	static InputManager* instance;

	const float INPUT_TIME;
	float lastFrameTimeStamp;
	int frameRate;
};