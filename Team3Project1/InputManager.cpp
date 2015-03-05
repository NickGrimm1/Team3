#include "InputManager.h"
#include "GameStateManager.h"

InputManager* InputManager::instance = NULL;

#if WINDOWS_BUILD
void InputManager::Run()
{
	ThreadRun();
}
#endif
#if PS3_BUILD
void InputManager::Run(uint64_t arg)
{
	std::cout << "Input Thread Started" << std::endl;
	instance->ThreadRun();
	std::cout << "Input Thread Ended" << std::endl;
	sys_ppu_thread_exit (0);
}
#endif

void InputManager::ThreadRun()
{
	isRunning = true;
	while(isRunning)
	{
#if WINDOWS_BUILD
		while (Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp < INPUT_TIME) { ; } // Fix the timestep
		float msec = Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp;
		lastFrameTimeStamp = Window::GetWindow().GetTimer()->GetMS();
		frameRate = (int)(1000.0f / msec);
#endif
#if PS3_BUILD
		frameRate = 16.6f;
#endif

#if PS3_BUILD
		// Check for disconnected player pads
		CellPadInfo2 info;
		cellPadGetInfo2(&info);
		for (int i = 0; i <  GamepadEvents::PLAYERINDEX_MAX; ++i)
		{
			if (gamePads[i]) // if this gamepad slot is null, the controller wasn't in use anyway so we can ignore it.
			{
				if(!(info.port_status[gamePads[i]->GetPlayerID()] & 0x1)) // Get the first bit of this - if it is 0 the controller is disconnected
				{
					GamepadEvents::PlayerIndex p = gamePads[i]->GetPlayerID();
					gamePads[i] = NULL; // Set to NULL, we will re-assign this when the game calls GetActiveController(playerID)
					GameStateManager::Instance()->GamepadDisconnect(p);
				}
			}
		}

		// Update game pads
		for (int i = 0; i <  GamepadEvents::PLAYERINDEX_MAX; ++i)
		{
			if (gamePads[i])
			{
				CellPadData data;
				if (cellPadGetData(i, &data) == CELL_PAD_OK);
					gamePads[i]->Update(data, 16.6f);
			}
		}
#endif
	}
}


GamepadEvents::PlayerIndex InputManager::GetActiveController(GamepadEvents::PlayerIndex playerID)
{
	unsigned int freePad;

	if (playerID != GamepadEvents::PLAYERINDEX_MAX)
	{
		// Check this is a valid ID - if it has already been assigned, return error
		for (int i = 0; i < GamepadEvents::PLAYERINDEX_MAX; ++i)
			if (gamePads[i] && gamePads[i]->GetPlayerID() == playerID)
				return GamepadEvents::PLAYERINDEX_MAX;

		// Find the first free playerID
		for (int i = 0; i < GamepadEvents::PLAYERINDEX_MAX + 1; ++i)
		{
			bool inUse = false;
			for (int j = 0; j < GamepadEvents::PLAYERINDEX_MAX; ++j)
			{
				if (gamePads[j]->GetPlayerID() == i)
					inUse = true;
			}
			if (!inUse)
			{
				playerID = (GamepadEvents::PlayerIndex)i;
				break;
			}
		}
		if (playerID = GamepadEvents::PLAYERINDEX_MAX)
			return GamepadEvents::PLAYERINDEX_MAX; // No free playerIDs left!
	}
	// Check at least one pad is free.
	for (int i = 0; i < GamepadEvents::PLAYERINDEX_MAX + 1; ++i)
	{
		if (!gamePads[i]) // This pad is free
		{
			freePad = i;
			break;
		}
	}
	if (freePad = GamepadEvents::PLAYERINDEX_MAX)
		return GamepadEvents::PLAYERINDEX_MAX; // No free pads left!

#if PS3_BUILD
	// Check all unused pads for start of X
	for (int i = 0; i < GamepadEvents::PLAYERINDEX_MAX; ++i)
	{
		if (!gamePads[i]) // NULL pads are not in use.
		{
			CellPadData data;
			if (cellPadGetData(i, &data) == CELL_PAD_OK);
			{
				if(data.button[2] & (1 << 3)) // Start
				{
					gamePads[freePad] = new GamePad(playerID);
					return playerID;
				}
				if(data.button[3] & (1 << 6)) // X 
				{
					gamePads[freePad] = new GamePad(playerID);
					return playerID;
				}
			}
		}
	}
#endif

	return GamepadEvents::PLAYERINDEX_MAX; // No active controllers - the game should carry on polling.
}
