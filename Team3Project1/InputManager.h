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
	static bool Initialize(InputManager* out);
	void Run();
};