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
}

void GamePad::Update(float msec)
{
#if PS3_BUILD
	CellPadData gamePadState;
	if (cellPadGetData(playerID, &gamePadState) == CELL_PAD_OK)
	{
#endif
#if WINDOWS_BUILD
	XINPUT_STATE gamePadState;
	ZeroMemory(&gamePadState, sizeof(XINPUT_STATE));
	if (XInputGetState(playerID, &gamePadState) == ERROR_SUCCESS)
	{
#endif
	// query gamePadState for analogue movement
#pragma region Analogue
#pragma region Sticks
#pragma region Right Stick
#if PS3_BUILD
	T3Vector2 rightStick(gamePadState.button[4] / 255.0f - 0.5f, gamePadState.button[5] / 255.0f - 0.5f);
#endif
#if WINDOWS_BUILD
	T3Vector2 rightStick(gamePadState.Gamepad.sThumbRX / 32767.0f, gamePadState.Gamepad.sThumbRY / 32767.0f);
#endif
	if (rightStick > T3Vector2(DEADZONE, DEADZONE))
	{
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::RIGHT_STICK, rightStick);
		
		if (rightStick.x > 0.5f)
		{
			if (!previousRightStick.x > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_RIGHT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_RIGHT);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] += msec;
		}
		

		if (rightStick.x < -0.5f)
		{
			if (!previousRightStick.x < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_LEFT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_LEFT);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] += msec;
		}

		if (rightStick.y < -0.5f)
		{
			if (!previousRightStick.y < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_DOWN);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_DOWN);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] += msec;
		}

		if (rightStick.y > 0.5f)
		{
			if (!previousRightStick.y > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_UP);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_UP);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] += msec;
		}
	}

	if (previousRightStick.x > 0.5f && rightStick.x < 0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_RIGHT);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] < PRESSLIMIT)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_RIGHT);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] = 0;
	}
	if (previousRightStick.y > 0.5f && rightStick.y < 0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_UP);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] < PRESSLIMIT)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_UP);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] = 0;
	}
	if (previousRightStick.y < -0.5f && rightStick.y > -0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_DOWN);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] < PRESSLIMIT)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_DOWN);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] = 0;
	}
	if (previousRightStick.x < -0.5f && rightStick.x > -0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_LEFT);
				
		if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] < PRESSLIMIT)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_LEFT);
		}
		buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] = 0;
	}
#pragma endregion
#pragma region Left Stick
#if PS3_BUILD
	T3Vector2 leftStick(gamePadState.button[6] / 255.0f - 0.5f, gamePadState.button[7] / 255.0f - 0.5f);
#endif
#if WINDOWS_BUILD
	T3Vector2 leftStick(gamePadState.Gamepad.sThumbLX / 32767.0f, gamePadState.Gamepad.sThumbLY / 32767.0f);
#endif
	if (leftStick > T3Vector2(DEADZONE, DEADZONE))
	{
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::LEFT_STICK, leftStick);
		
		if (leftStick.x > 0.5f)
		{
			if (!previousLeftStick.x > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_RIGHT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_RIGHT);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] += msec;
		}

		if (leftStick.x < -0.5f)
		{
			if (!previousLeftStick.x < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_LEFT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_LEFT);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] += msec;
		}

		if (leftStick.y < -0.5f)
		{
			if (!previousLeftStick.y < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_DOWN);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_DOWN);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] += msec;
		}

		if (leftStick.y > 0.5f)
		{
			if (!previousLeftStick.y > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_UP);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_UP);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] += msec;
		}
	}

	if (previousLeftStick.x > 0.5f && leftStick.x < 0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_RIGHT);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] < PRESSLIMIT)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_RIGHT);
		}
		buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] = 0;
	}

	if (previousLeftStick.x < -0.5f && leftStick.x > -0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_LEFT);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] < PRESSLIMIT)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_LEFT);
		}
		buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] = 0;
	}

	if (previousLeftStick.y < -0.5f && leftStick.y > -0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_DOWN);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] < PRESSLIMIT)
		{
			GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_DOWN);
		}
		buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] = 0;
	}

	if (previousLeftStick.y > 0.5f && leftStick.y < 0.5f)
	{
		GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_UP);
				
		if (buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] < PRESSLIMIT)
		{
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
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::RIGHT_TRIGGER, rightTrigger);
		if (rightTrigger > 0.5f)
		{
			if (!previousRightTrigger > 0.5f)
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
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::LEFT_TRIGGER, leftTrigger);
		if (leftTrigger > 0.5f)
		{
			if (!previousLeftTrigger > 0.5f)
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
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, button);
			}
			else
			{
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
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, button);
				if (buttonHoldTimes[i] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, button);
				}
			}
			buttonHoldTimes[i] = 0;
		}

#if PS3_BUILD
		if(gamePadState.button[3] & (1 << i))
#endif
#if WINDOWS_BUILD
		if (previousGamePadState.Gamepad.wButtons & (1 << (i + halfInput)))
#endif
		{
			GamepadEvents::Button button = (GamepadEvents::Button)(halfInput + i);
			// Button is down, check for first
#if PS3_BUILD
			if (!(previousGamePadState.button[3] & (1 << i)))
#endif
#if WINDOWS_BUILD
			if (!(previousGamePadState.Gamepad.wButtons & (1 << (i + halfInput))))
#endif
			{
				// Special case - L2 & R2 are analogue
				if (button == GamepadEvents::INPUT_L2_TRIGGER || button == GamepadEvents::INPUT_R2_TRIGGER)
				{
				}
				else
				{
#if PS3_BUILD
					if (!(previousGamePadState.button[3] & (1 << i)))
#endif
#if WINDOWS_BUILD
					if (!(previousGamePadState.Gamepad.wButtons & (1 << (i + halfInput))))
#endif
					{			
						GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, button);
					}
					else
					{
						GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, button);
					}
					buttonHoldTimes[halfInput + i] += msec;
				}
			}
			else
			{
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
						GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, button);
						if (buttonHoldTimes[i] < PRESSLIMIT)
						{
							GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, button);
						}
					}
					buttonHoldTimes[halfInput + i] = 0;
				}
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
