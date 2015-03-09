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
#if PS3_BUILD
#include <cell/pad.h>
#endif
class InputManager : public Thread
{
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

#if WINDOWS_BUILD
	void Run();
#endif
#if PS3_BUILD
	static void Run(uint64_t arg);
#endif
	// Gets a controller for a player (Press start or A type prompt...). Specify playerID if a controller needs re-connecting, otherwise assigns the first free playerID
	GamepadEvents::PlayerIndex GetActiveController(GamepadEvents::PlayerIndex playerID = GamepadEvents::PLAYERINDEX_MAX);
	int GetFrameRate() { return frameRate; }
private:
#if WINDOWS_BUILD
	InputManager() 
		: INPUT_TIME(1000.0f / 180)
	{
		gamePads = new GamePad*[GamepadEvents::PLAYERINDEX_MAX];
		for (int i = 0; i < GamepadEvents::PLAYERINDEX_MAX; ++i)
			gamePads[i] = NULL;

		frameRate = 0;

		//if(cellPadInit(GamepadEvents::PLAYERINDEX_MAX) != CELL_PAD_OK) {
			//std::cout << "cellPadInit failed!" << std::endl;	
		//}
	}
#endif
#if PS3_BUILD
	InputManager() 
		: Thread(Run), INPUT_TIME(1000.0f / 60)
	{
		gamePads = new GamePad*[GamepadEvents::PLAYERINDEX_MAX];
		for (int i = 0; i < GamepadEvents::PLAYERINDEX_MAX; ++i)
			gamePads[i] = NULL;

		if(cellPadInit(GamepadEvents::PLAYERINDEX_MAX) != CELL_PAD_OK)
		{
			std::cout << "cellPadInit failed!" << std::endl;	
		}

		frameRate = 0;
	}
#endif
	~InputManager() 
	{
#if PS3_BUILD
		cellPadEnd();
#endif
	}
	static InputManager* instance;
	GamePad** gamePads;
	const float INPUT_TIME;
	float lastFrameTimeStamp;
	int frameRate;
	void ThreadRun();
};

