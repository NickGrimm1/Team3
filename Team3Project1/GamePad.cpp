#include "GamePad.h"
#include "GameStateManager.h"

GamePad::GamePad(GamepadEvents::PlayerIndex playerID)
	: playerID(playerID)
{
	for (unsigned int i = 0; i < GamepadEvents::PADBUTTONS_MAX; ++i)
		buttonHoldTimes[i] = PRESSLIMIT + 20; // We initialise hold times to greater to prevent an accidental press event immediately after the controller is first registered.

	T3Vector2 previousRightStick = 0.0f;
	T3Vector2 previousLeftStick = 0.0f;
	float previousRightTrigger = 0;
	float previousLeftTrigger = 0;

#if PS3_BUILD
	previousGamePadState.button[2] = 0; // Initialise these because Sony can't be fucked.
	previousGamePadState.button[3] = 0;
	previousGamePadState.button[4] = 128;
	previousGamePadState.button[5] = 128;
	previousGamePadState.button[6] = 128;
	previousGamePadState.button[7] = 128;

	// Oh and of course we have to specifically enable the triggers. Not like they don't come as standard. Muppets.
	cellPadSetPortSetting(playerID, CELL_PAD_SETTING_PRESS_ON);
#endif
}

void GamePad::Update(float msec)
{
#if PS3_BUILD
	CellPadData stupidGamePadState; // FUCK YOU SONY, I WANT A DECENT ONE!!!!
	CellPadData gamePadState;
	gamePadState.button[2] = 0; // Initialise these because Sony can't be fucked.
	gamePadState.button[3] = 0;
	gamePadState.button[4] = 128;
	gamePadState.button[5] = 128;
	gamePadState.button[6] = 128;
	gamePadState.button[7] = 128;
	if (cellPadGetData(playerID, &stupidGamePadState) == CELL_PAD_OK)
	{
		// Hopefully this actually we still have a controller, but OK doesn't actually mean what it says in the documentation.
		if (stupidGamePadState.len == 0)
			gamePadState = previousGamePadState; // WHY do I not get any information if the user is still holding down a button?!?!?!?
		else if (stupidGamePadState.len < 7) // Doubly fuck you sony, even when the state has changed I still don't have a clue how much data you're going to send me. Twats.
		{
			unsigned int i = 2;
			while (i < stupidGamePadState.len)
			{
				gamePadState.button[i] = stupidGamePadState.button[i]; // Assign the data that Sony has deemed us worthy to recieve.
				++i;
			}
			while ( i < 8)
			{
				gamePadState.button[i] = previousGamePadState.button[i]; // And fill the rest up before we get a load of random crap that will fuck the entire thing. Uugh :(
				++i;
			}
		}
		else
			gamePadState = stupidGamePadState; // WooHoo! We could have had this all those lines of code ago if only Sony weren't such dicks.
#endif
#if WINDOWS_BUILD
	XINPUT_STATE gamePadState; // These three lines are what we need for Microsoft by comparison.
	ZeroMemory(&gamePadState, sizeof(XINPUT_STATE));
	if (XInputGetState(playerID, &gamePadState) == ERROR_SUCCESS)
	{
#endif
	// query gamePadState for analogue movement
#pragma region Analogue
#pragma region Sticks
#pragma region Right Stick
#if PS3_BUILD
	T3Vector2 rightStick(gamePadState.button[4] / 255.0f - 0.5f, -(gamePadState.button[5] / 255.0f - 0.5f));
	rightStick *= 2;
#endif
#if WINDOWS_BUILD
	T3Vector2 rightStick(gamePadState.Gamepad.sThumbRX / 32767.0f, gamePadState.Gamepad.sThumbRY / 32767.0f);
#endif
	if (rightStick > T3Vector2(DEADZONE, DEADZONE))
	{
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::RIGHT_STICK, rightStick);

		if (rightStick.x > 0.5f)
		{
			if (previousRightStick.x <= 0.5f)
			{
				std::cout << "Sending GP Event: RS R D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_RIGHT);
			}
			else
			{
				std::cout << "Sending GP Event: RS R H" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_RIGHT);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] += msec;
		}
		

		if (rightStick.x < -0.5f)
		{
			if (previousRightStick.x >= -0.5f)
			{
				std::cout << "Sending GP Event: RS L D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_LEFT);
			}
			else
			{
				std::cout << "Sending GP Event: RS L H" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_LEFT);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] += msec;
		}

		if (rightStick.y < -0.5f)
		{
			if (previousRightStick.y >= -0.5f)
			{
				std::cout << "Sending GP Event: RS R D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_DOWN);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_DOWN);
				std::cout << "Sending GP Event: RS R H" << std::endl;
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] += msec;
		}

		if (rightStick.y > 0.5f)
		{
			if (previousRightStick.y <= 0.5f)
			{
				std::cout << "Sending GP Event: RS U D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_UP);
			}
			else
			{
				std::cout << "Sending GP Event: RS U H" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_UP);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] += msec;
		}
	}

	if (previousRightStick.x > 0.5f && rightStick.x < 0.5f)
	{
		std::cout << "Sending GP Event: RS R U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_RIGHT);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: RS R P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_RIGHT);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] = 0;
	}
	if (previousRightStick.y > 0.5f && rightStick.y < 0.5f)
	{
		std::cout << "Sending GP Event: RS U U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_UP);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: RS U P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_UP);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] = 0;
	}
	if (previousRightStick.y < -0.5f && rightStick.y > -0.5f)
	{
		std::cout << "Sending GP Event: RS D U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_DOWN);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: RS D P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_DOWN);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] = 0;
	}
	if (previousRightStick.x < -0.5f && rightStick.x > -0.5f)
	{
		std::cout << "Sending GP Event: RS L U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_LEFT);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: RS L P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_LEFT);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] = 0;
	}
#pragma endregion
#pragma region Left Stick
#if PS3_BUILD
	T3Vector2 leftStick(gamePadState.button[6] / 255.0f - 0.5f, -(gamePadState.button[7] / 255.0f - 0.5f)); 
	leftStick *= 2;
#endif
#if WINDOWS_BUILD
	T3Vector2 leftStick(gamePadState.Gamepad.sThumbLX / 32767.0f, gamePadState.Gamepad.sThumbLY / 32767.0f);
#endif
	if (leftStick > T3Vector2(DEADZONE, DEADZONE))
	{
		std::cout << leftStick << std::endl;
		std::cout << gamePadState.button[6] << ", " << gamePadState.button[7] << std::endl;
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::LEFT_STICK, leftStick);
		
		if (leftStick.x > 0.5f)
		{
			if (previousLeftStick.x <= 0.5f)
			{
				std::cout << "Sending GP Event: LS R D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_RIGHT);
			}
			else
			{
				std::cout << "Sending GP Event: LS R H" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_RIGHT);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] += msec;
		}

		if (leftStick.x < -0.5f)
		{
			if (previousLeftStick.x >= -0.5f)
			{
				std::cout << "Sending GP Event: LS L D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_LEFT);
			}
			else
			{
				std::cout << "Sending GP Event: LS L H" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_LEFT);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] += msec;
		}

		if (leftStick.y < -0.5f)
		{
			if (previousLeftStick.y >= -0.5f)
			{
				std::cout << "Sending GP Event: LS D D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_DOWN);
			}
			else
			{
				std::cout << "Sending GP Event: LS D H" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_DOWN);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] += msec;
		}

		if (leftStick.y > 0.5f)
		{
			if (previousLeftStick.y <= 0.5f)
			{
				std::cout << "Sending GP Event: LS U D" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_UP);
			}
			else
			{
				std::cout << "Sending GP Event: LS U H" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_UP);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] += msec;
		}
	}

	if (previousLeftStick.x > 0.5f && leftStick.x < 0.5f)
	{
		std::cout << "Sending GP Event: LS R U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_RIGHT);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: LS R P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_RIGHT);
		}
		buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] = 0;
	}

	if (previousLeftStick.x < -0.5f && leftStick.x > -0.5f)
	{
		std::cout << "Sending GP Event: LS L U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_LEFT);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: LS L P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_LEFT);
		}
		buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] = 0;
	}

	if (previousLeftStick.y < -0.5f && leftStick.y > -0.5f)
	{
		std::cout << "Sending GP Event: LS D U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_DOWN);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: LS D P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_DOWN);
		}
		buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] = 0;
	}

	if (previousLeftStick.y > 0.5f && leftStick.y < 0.5f)
	{
		std::cout << "Sending GP Event: LS U U" << std::endl;
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_UP);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] < PRESSLIMIT)
		{
			std::cout << "Sending GP Event: LS U P" << std::endl;
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_UP);
		}
		buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] = 0;
	}
#pragma endregion
#pragma endregion
#pragma region Triggers
#pragma region Right Trigger
#if PS3_BUILD
	T3Vector2 rightTrigger = gamePadState.button[19] / 255.0f; 
#endif
#if WINDOWS_BUILD
	T3Vector2 rightTrigger = gamePadState.Gamepad.bRightTrigger / 255.0f;
#endif
	if (rightTrigger > DEADZONE)
	{
		std::cout << rightTrigger << std::endl;
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::RIGHT_TRIGGER, rightTrigger);
		if (rightTrigger > 0.5f)
		{
			if (previousRightTrigger <= 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::INPUT_R2_TRIGGER);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::INPUT_R2_TRIGGER);
			}
			buttonHoldTimes[GamepadEvents::INPUT_R2_TRIGGER] += msec;
		}
	}
	else
	{
		if (previousRightTrigger > 0.5f)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::INPUT_R2_TRIGGER);
				
			if (buttonHoldTimes[GamepadEvents::INPUT_R2_TRIGGER] < PRESSLIMIT)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::INPUT_R2_TRIGGER);
			}
			buttonHoldTimes[GamepadEvents::INPUT_R2_TRIGGER] = 0;
		}
	}
#pragma endregion
#pragma region Left Trigger
#if PS3_BUILD
	T3Vector2 leftTrigger = gamePadState.button[18] / 255.0f; 
#endif
#if WINDOWS_BUILD
	T3Vector2 leftTrigger = gamePadState.Gamepad.bLeftTrigger / 255.0f;
#endif
	if (leftTrigger > DEADZONE)
	{
		std::cout << leftTrigger << std::endl;
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::LEFT_TRIGGER, leftTrigger);
		if (leftTrigger > 0.5f)
		{
			if (previousLeftTrigger <= 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::INPUT_L2_TRIGGER);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::INPUT_L2_TRIGGER);
			}
			buttonHoldTimes[GamepadEvents::INPUT_L2_TRIGGER] += msec;
		}
	}
	else
	{
		if (previousLeftTrigger > 0.5f)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::INPUT_L2_TRIGGER);
				
			if (buttonHoldTimes[GamepadEvents::INPUT_L2_TRIGGER] < PRESSLIMIT)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::INPUT_L2_TRIGGER);
			}
			buttonHoldTimes[GamepadEvents::INPUT_L2_TRIGGER] = 0;
		}
	}
#pragma endregion
#pragma endregion
#pragma endregion
	// query gamePadState for button action
#pragma region Buttons
	int halfInput = 8;
	for(int i = 0; i < halfInput; ++i)
	{
#if PS3_BUILD
		if(gamePadState.button[2] & (1 << i)) 
#endif
#if WINDOWS_BUILD
		if (gamePadState.Gamepad.wButtons & (1 << i))
#endif
		{
			GamepadEvents::Button button = (GamepadEvents::Button)i;
			// Button is down, check for first
#if PS3_BUILD
			if (!(previousGamePadState.button[2] & (1 << i)))
#endif
#if WINDOWS_BUILD
			if (!(previousGamePadState.Gamepad.wButtons & (1 << i)))
#endif
			{	
				std::cout << "Sending GP Event" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, button);
			}
			else
			{
				std::cout << "Sending GP Event" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, button);
			}
			buttonHoldTimes[i] += msec;
		}
		else
		{
			// button is up, check if it was down
#if PS3_BUILD
			if (previousGamePadState.button[2] & (1 << i))
#endif
#if WINDOWS_BUILD
			if (previousGamePadState.Gamepad.wButtons & (1 << i))
#endif
			{
				GamepadEvents::Button button = (GamepadEvents::Button)i;
				std::cout << "Sending GP Event" << std::endl;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, button);
				if (buttonHoldTimes[i] < PRESSLIMIT)
				{
					std::cout << "Sending GP Event" << std::endl;
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, button);
				}
			}
			buttonHoldTimes[i] = 0;
		}

#ifdef PS3_BUILD
		if(gamePadState.button[3] & (1 << i))
#endif
#ifdef WINDOWS_BUILD
		if (gamePadState.Gamepad.wButtons & (1 << (i + halfInput)))
#endif
		{
			GamepadEvents::Button button = (GamepadEvents::Button)(halfInput + i);
			// Button is down, check for first

			// Special case - L2 & R2 are analogue
			if (button == GamepadEvents::INPUT_L2_TRIGGER || button == GamepadEvents::INPUT_R2_TRIGGER)
			{
			}
			else
			{
#ifdef PS3_BUILD
				if (!(previousGamePadState.button[3] & (1 << i)))
#endif
#ifdef WINDOWS_BUILD
				if (!(previousGamePadState.Gamepad.wButtons & (1 << (i + halfInput))))
#endif
				{			
					std::cout << "Sending GP Event" << std::endl;
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, button);
				}
				else
				{
					std::cout << "Sending GP Event" << std::endl;
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, button);
				}
				buttonHoldTimes[halfInput + i] += msec;
			}
		}
		else
		{
			GamepadEvents::Button button = (GamepadEvents::Button)(halfInput + i);

			// Special case - L2 & R2 are analogue
			if (button == GamepadEvents::INPUT_L2_TRIGGER || button == GamepadEvents::INPUT_R2_TRIGGER)
			{
			}
			else
			{
				// button is up, check if it was down
#if PS3_BUILD
				if (previousGamePadState.button[3] & (1 << i))
#endif
#if WINDOWS_BUILD
				if (previousGamePadState.Gamepad.wButtons & (1 << (i + halfInput)))
#endif
				{
					GamepadEvents::Button button = (GamepadEvents::Button)(halfInput +i);
					std::cout << "Sending GP Event" << std::endl;
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, button);
					if (buttonHoldTimes[halfInput + i] < PRESSLIMIT)
					{
						std::cout << "Sending GP Event" << std::endl;
						GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, button);
					}
				}
				buttonHoldTimes[halfInput + i] = 0;
			}
		}
	}
#pragma endregion
	// Set for next frame
	previousGamePadState = gamePadState;
	previousLeftStick = leftStick;
	previousRightStick = rightStick;
	previousLeftTrigger = leftTrigger.x;
	previousRightTrigger = rightTrigger.x;
	}
}
