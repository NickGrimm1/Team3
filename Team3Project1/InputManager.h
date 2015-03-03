/**
<summary>Static Singleton.
The games input manager.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/

// TODO: Check libpad for windows (How mnuch of a pain in the arse is it to get drivers for PS3 pads?

#pragma once
#include "Thread.h"
#include "GamePad.h"
#include <cell/pad.h>

class InputManager : public Thread
{
	#if PS3_BUILD
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
	// Gets a controller for a player (Press start or A type prompt...). Specify playerID if a controller needs re-connecting, otherwise assigns the first free playerID
	GamepadEvents::PlayerIndex GetActiveController(GamepadEvents::PlayerIndex playerID = GamepadEvents::PLAYERINDEX_MAX);
private:
	InputManager() 
	{
		gamePads = new GamePad*[GamepadEvents::PLAYERINDEX_MAX];
		for (int i = 0; i < GamepadEvents::PLAYERINDEX_MAX; ++i)
			gamePads[i] = NULL;

		if(cellPadInit(GamepadEvents::PLAYERINDEX_MAX) != CELL_PAD_OK) {
			std::cout << "cellPadInit failed!" << std::endl;	
	}
	}
	~InputManager() 
	{
		cellPadEnd();
	}
	static InputManager* instance;
	GamePad** gamePads;
	#endif
};
