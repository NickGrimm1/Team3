/**
<summary>Static Singleton.
The game's physics engine.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include "Thread.h"

class PhysicsEngine : public Thread
{
	// TODO: Implement Physics Engine.
public:
	static bool Initialize(PhysicsEngine*& out) 
	{
		if (instance == NULL)
			instance = new PhysicsEngine();

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
	PhysicsEngine() { }
	~PhysicsEngine() { }
	static PhysicsEngine* instance;
};